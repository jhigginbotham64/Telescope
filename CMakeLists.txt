project(telescope)

cmake_minimum_required(VERSION 3.13)

set(CMAKE_MACOSX_RPATH 1)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_library(telescope SHARED telescope.cpp /usr/include/SDL2/SDL.h /usr/include/SDL2/SDL_mixer.h /usr/include/SDL2/SDL_image.h /usr/include/SDL2/SDL_net.h /usr/include/SDL2/SDL_ttf.h /usr/include/SDL2/SDL_vulkan.h /usr/include/shaderc/shaderc.hpp /usr/include/glm/glm.hpp /usr/include/vulkan/vulkan.hpp)

target_include_directories(telescope SYSTEM PRIVATE /usr/include)
target_link_directories(telescope PRIVATE /usr/lib)
target_link_libraries(telescope SDL2 SDL2_image SDL2_mixer SDL2_net SDL2_ttf vulkan shaderc_shared)

install(TARGETS
telescope
LIBRARY DESTINATION lib
ARCHIVE DESTINATION lib
RUNTIME DESTINATION lib)
