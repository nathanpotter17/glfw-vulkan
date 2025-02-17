#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <vector>

const int WIDTH = 800;
const int HEIGHT = 600;

class VlkGlfwWindow {
public:
    void run() {
        initWindow();
        initVulkan();
        mainLoop();
        cleanup();
    }

private:
    GLFWwindow* window;
    VkInstance instance;
    VkSurfaceKHR surface;
    VkDevice device = VK_NULL_HANDLE;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkPhysicalDeviceProperties deviceProperties;
    VkQueue graphicsQueue;

    void initWindow() {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan Window", nullptr, nullptr);
    }

    void initVulkan() {
        createInstance();
        pickPhysicalDevice();
        createLogicalDevice();
    }

    void createInstance() {
        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Vulkan Device Info";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        // Get required extensions for GLFW
        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
        createInfo.enabledExtensionCount = glfwExtensionCount;
        createInfo.ppEnabledExtensionNames = glfwExtensions;

        if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create Vulkan instance!");
        }

        // Create window surface
        if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create window surface!");
        }
    }

    void pickPhysicalDevice() {
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

        if (deviceCount == 0) {
            throw std::runtime_error("No Vulkan-supported GPUs found!");
        }

        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
        std::cout << "===================================" << std::endl;
        std::cout << "Compatible Vulkan Devices On-Board:" << std::endl;

        VkPhysicalDevice bestDevice = VK_NULL_HANDLE;
        for (const auto& device : devices) {
            
            vkGetPhysicalDeviceProperties(device, &deviceProperties);

            std::cout << "Device Name: " << deviceProperties.deviceName << std::endl;
            std::cout << "API Version: " << VK_VERSION_MAJOR(deviceProperties.apiVersion) << "."
                      << VK_VERSION_MINOR(deviceProperties.apiVersion) << "."
                      << VK_VERSION_PATCH(deviceProperties.apiVersion) << std::endl;
            std::cout << "Driver Version: " << deviceProperties.driverVersion << std::endl;
            std::cout << "Vendor ID: " << deviceProperties.vendorID << std::endl;
            std::cout << "Device ID: " << deviceProperties.deviceID << std::endl;
            

            std::cout << "Device Type: ";
            switch (deviceProperties.deviceType) {
                case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
                    std::cout << "Discrete GPU" << std::endl;
                    bestDevice = device;  // Prefer discrete GPU
                    break;
                case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
                    std::cout << "Integrated GPU" << std::endl;
                    if (bestDevice == VK_NULL_HANDLE) bestDevice = device;  // Fallback
                    break;
                case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
                    std::cout << "Virtual GPU" << std::endl;
                    break;
                case VK_PHYSICAL_DEVICE_TYPE_CPU:
                    std::cout << "CPU" << std::endl;
                    break;
                default:
                    std::cout << "Other" << std::endl;
                    break;
            }
            std::cout << std::endl;
        }

        if (bestDevice == VK_NULL_HANDLE) {
            throw std::runtime_error("No suitable Vulkan GPU found!");
        }

        physicalDevice = bestDevice;
    }

    void createLogicalDevice() {
        if (physicalDevice == VK_NULL_HANDLE) {
            throw std::runtime_error("Physical device not selected before creating logical device!");
        }

        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

        int graphicsQueueFamilyIndex = -1;
        for (int i = 0; i < queueFamilies.size(); ++i) {
            if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                graphicsQueueFamilyIndex = i;
                break;
            }
        }

        if (graphicsQueueFamilyIndex == -1) {
            throw std::runtime_error("No graphics queue family found!");
        }

        float queuePriority = 1.0f;
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = graphicsQueueFamilyIndex;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;

        VkPhysicalDeviceFeatures deviceFeatures{};

        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.pQueueCreateInfos = &queueCreateInfo;
        createInfo.queueCreateInfoCount = 1;
        createInfo.pEnabledFeatures = &deviceFeatures;

        if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create logical device!");
        }

        vkGetDeviceQueue(device, graphicsQueueFamilyIndex, 0, &graphicsQueue);

        vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);

        std::cout << "===================================" << std::endl;
        std::cout << "Logical Device Created Successfully!" << std::endl;
        std::cout << "Selected GPU: " << deviceProperties.deviceName << std::endl;
        std::cout << "Queue Family Index: " << graphicsQueueFamilyIndex << std::endl;
        std::cout << "API Version: " << VK_VERSION_MAJOR(deviceProperties.apiVersion) << "."
                << VK_VERSION_MINOR(deviceProperties.apiVersion) << "."
                << VK_VERSION_PATCH(deviceProperties.apiVersion) << std::endl;
        std::cout << "Driver Version: " << deviceProperties.driverVersion << std::endl;
        std::cout << "Vendor ID: " << deviceProperties.vendorID << std::endl;
        std::cout << "Device ID: " << deviceProperties.deviceID << std::endl;
        std::cout << "===================================" << std::endl;
    }

    void mainLoop() {
        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();
        }
    }

    void cleanup() {
        if (device != VK_NULL_HANDLE) {
            vkDestroyDevice(device, nullptr);
        }
        vkDestroySurfaceKHR(instance, surface, nullptr);
        vkDestroyInstance(instance, nullptr);
        glfwDestroyWindow(window);
        glfwTerminate();
    }
};

int main() {
    VlkGlfwWindow app;

    try {
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
