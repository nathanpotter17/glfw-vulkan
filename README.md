# Minimal GLFW & Vulkan for Windows

This repository contains a simple template for using GLFW with Vulkan on Windows only. The template is informed by the [Vulkan Guide by GLFW](https://www.glfw.org/docs/3.3/vulkan_guide.html), the [Vulkan Tutorial](https://vulkan-tutorial.com/Introduction) by Alexander Overvoorde, and many other resources, like OGLDEV on YouTube.

## Dependencies

- [GLFW](https://www.glfw.org/)
- [Vulkan SDK](https://vulkan.lunarg.com/)
- Windows SDK (Only for Windows)
- CMake
- VS Code

## Environment Setup

I recommend using Ninja as the build system for this project. It is by far the fastest and easiest to use.

First ensure you install the Vulkan SDK for Windows. Add the VULKAN_SDK environment variable to your system if desired. Check install with `vulkaninfo` in the command prompt.

Next, grab glfw from https://github.com/glfw/glfw/releases or simply clone it into your C or Root directory. Then build the glfw library using CMake & Ninja. The following commands should be used to build the library:

```bash
cd glfw
mkdir build
cd build
cmake -G "Ninja" ..
cmake --build ..
ninja install
```

That should ensure that the glfw library is built and installed on your system, and links correctly with other required windows libraries.

## Building the Template

To build the example, simply clone the repository and run the following commands in the command prompt:

```cmd
cd glfw-vulkan
build.bat
```

Use run.bat to run the example, or re-compile with build.bat.
