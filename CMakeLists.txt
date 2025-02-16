cmake_minimum_required(VERSION 3.10)
project(VulkanWindow)

set(CMAKE_CXX_STANDARD 17)

include_directories(external/glfw/include/include external/vulkan/include/Include)
link_directories(external/glfw/lib/src external/vulkan/lib/Lib)

find_package(Vulkan REQUIRED)
add_executable(VulkanWindow src/main.cpp)

target_link_libraries(VulkanWindow glfw3 vulkan-1)