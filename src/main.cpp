#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <vector>

const int WIDTH = 800;
const int HEIGHT = 600;

class HelloTriangleApplication {
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

    void initWindow() {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan Window", nullptr, nullptr);
    }

    void initVulkan() {
        createInstance();
        listPhysicalDevices();
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

        // Get required extensions (e.g., for GLFW)
        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
        createInfo.enabledExtensionCount = glfwExtensionCount;
        createInfo.ppEnabledExtensionNames = glfwExtensions;

        // Create Vulkan instance
        if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create Vulkan instance!");
        }

        // Create a Vulkan window surface
        VkResult err = glfwCreateWindowSurface(instance, window, NULL, &surface);
        if (err)
        {
            // Window surface creation failed
            throw std::runtime_error("Failed to create window surface!");
        }


    }

    void listPhysicalDevices() {
        // Enumerate physical devices (GPUs)
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

        if (deviceCount == 0) {
            throw std::runtime_error("No Vulkan-supported GPUs found!");
        }

        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

        // Log device info
        std::cout << "Available Vulkan devices:" << std::endl;
        for (const auto& device : devices) {
            VkPhysicalDeviceProperties deviceProperties;
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
                case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
                    std::cout << "Integrated GPU" << std::endl;
                    break;
                case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
                    std::cout << "Discrete GPU" << std::endl;
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
    }

    void mainLoop() {
        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();
        }
    }

    void cleanup() {
        vkDestroySurfaceKHR(instance, surface, nullptr);
        vkDestroyInstance(instance, nullptr);
        glfwDestroyWindow(window);
        glfwTerminate();
    }
};

int main() {
    HelloTriangleApplication app;

    try {
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}