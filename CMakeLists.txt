project(telescope)

cmake_minimum_required(VERSION 3.13)

set(CMAKE_MACOSX_RPATH 1)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_library(telescope SHARED telescope.cpp)

target_link_libraries(telescope SDL2 SDL2_image SDL2_mixer SDL2_net SDL2_ttf vulkan shaderc)

install(TARGETS
telescope
LIBRARY DESTINATION lib
ARCHIVE DESTINATION lib
RUNTIME DESTINATION lib)
