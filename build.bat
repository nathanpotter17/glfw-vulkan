@echo off

REM Navigate to the build directory (create if it doesn't exist)
if not exist build mkdir build
cd build

REM Run CMake to generate the build system
cmake -G "Ninja" ..

REM Build the project
cmake --build .

REM Navigate back to the root directory
cd ..

REM Run the executable
build\VulkanWindow.exe