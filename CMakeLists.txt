cmake_minimum_required(VERSION 3.13)

project(telescope VERSION 0.2.0 LANGUAGES CXX)

include(GNUInstallDirs)

include(CTest)
file(GLOB INCLUDE_FILES src/*.h)

### FIND DEPENDENCIES ###

find_package(Bullet REQUIRED)
find_package(SDL2 REQUIRED)
find_library(SDL2_image REQUIRED NAMES SDL2_image)
find_library(SDL2_mixer REQUIRED NAMES SDL2_mixer)
find_library(SDL2_net REQUIRED NAMES SDL2_net)
find_library(SDL2_ttf REQUIRED NAMES SDL2_ttf)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
find_package(shaderc_shared REQUIRED)

### TELESCOPE ###

add_library(telescope SHARED
  src/telescope.cpp
  src/telescope.h
)

set_target_properties(telescope PROPERTIES
    CXX_STANDARD 11
    MACOSX_RPATH 1
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    POSITION_INDEPENDENT_CODE ON
)

target_include_directories(telescope PUBLIC
    "${CMAKE_SOURCE_DIR}/src"
)

target_include_directories(telescope PRIVATE
    "${BULLET_INCLUDE_DIRS}"
    "${CMAKE_SOURCE_DIR}/VulkanMemoryAllocator-Hpp"
    /home/clem/Desktop/shaderc/include
)

target_link_libraries(telescope PUBLIC
    SDL2
    SDL2_image
    SDL2_mixer
    SDL2_net
    SDL2_ttf
    ${BULLET_LIBRARIES}
    ${shader_c_shared}
)

### TESTS ####

option(BUILD_TESTING "build telescope test suite" ON)
if (BUILD_TESTING)

    # \brief: declare a test
    # \param: test_name, has to be equal to the name of the actual .cpp inside ./test
    function(declare_test test_name)
        add_executable(
            ${test_name}
            "${PROJECT_SOURCE_DIR}/test/${test_name}.cpp"
            "${PROJECT_SOURCE_DIR}/test/test.hpp"
        )

        target_link_libraries(${test_name} telescope)
        target_include_directories(${test_name} PRIVATE ${CMAKE_SOURCE_DIR})
        add_test(NAME ${test_name} COMMAND ${test_name})
    endfunction()

    declare_test(template_test)
endif()

### INSTALL ###

install(FILES ${INCLUDE_FILES} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(TARGETS telescope
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION lib
)

### UNINSTALL ###

if(NOT TARGET uninstall)
  configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake/cmake_uninstall.cmake"
    IMMEDIATE @ONLY
  )

  add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P
    "${CMAKE_CURRENT_BINARY_DIR}/cmake/cmake_uninstall.cmake"
  )
endif()

