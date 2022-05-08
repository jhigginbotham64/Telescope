# Telescope
An Open-Source Toolkit for Interactive Multimedia Applications
## Dependencies
To build telescope on your system, you'll need several SDKs:
* [bullet3](https://github.com/bulletphysics/bullet3)
    * `apt install libbullet-dev`
* [Vulkan](https://www.vulkan.org/tools#download-these-essential-development-tools) via [LunarG](https://vulkan.lunarg.com/sdk/home)
    ```
    wget -qO - http://packages.lunarg.com/lunarg-signing-key-pub.asc | sudo apt-key add -
    sudo wget -qO /etc/apt/sources.list.d/lunarg-vulkan-focal.list http://packages.lunarg.com/vulkan/lunarg-vulkan-focal.list
    sudo apt update
    sudo apt install vulkan-sdk
    ```
* [SDL2](https://www.libsdl.org/index.php)
    * `apt install libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev libsdl2-net-dev`
* [GLM](https://github.com/g-truc/glm)
    * `apt install libglm-dev`
* [shaderc](https://github.com/google/shaderc)
    * `apt install shaderc`
## Build
`cmake . && make clean && make`
