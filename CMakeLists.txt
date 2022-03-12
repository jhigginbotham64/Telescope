project(telescope)

cmake_minimum_required(VERSION 3.7)
set(CMAKE_MACOSX_RPATH 1)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_subdirectory(SDL2)
add_subdirectory(SDL2_image)
add_subdirectory(SDL2_mixer)
add_subdirectory(SDL2_ttf)
add_subdirectory(glm)
add_subdirectory(vk)
add_subdirectory(shaderc)

add_library(telescope SHARED telescope.cpp)

target_link_libraries(telescope SDL2 SDL2_image SDL2_mixer SDL2_ttf glm vulkan shaderc)

install(TARGETS
telescope
LIBRARY DESTINATION lib
ARCHIVE DESTINATION lib
RUNTIME DESTINATION lib)
