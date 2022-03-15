project(telescope)

cmake_minimum_required(VERSION 3.13)

set(CMAKE_MACOSX_RPATH 1)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include(GNUInstallDirs)

add_library(telescope SHARED telescope.cpp)

target_link_libraries(telescope PRIVATE SDL2 SDL2_image SDL2_mixer SDL2_net SDL2_ttf shaderc_shared)

install(FILES telescope.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(TARGETS
telescope
LIBRARY DESTINATION lib
ARCHIVE DESTINATION lib
RUNTIME DESTINATION lib)

# uninstall target
if(NOT TARGET uninstall)
  configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

  add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
endif()
