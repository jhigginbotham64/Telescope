project(telescope)

cmake_minimum_required(VERSION 3.7)
set(CMAKE_MACOSX_RPATH 1)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

add_library(telescope SHARED telescope.cpp)

target_link_libraries(telescope SDL2 SDL2_ttf SDL2_mixer SDL2_image)
target_link_libraries(telescope vulkan)

install(TARGETS
telescope
LIBRARY DESTINATION lib
ARCHIVE DESTINATION lib
RUNTIME DESTINATION lib)
