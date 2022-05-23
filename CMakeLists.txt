#[=======================================================================[.rst:

Telescope
-----------

Build the telescope shared library, unit tests, documentation.

Copyright Joshua Higginbotham, 2022

Targets
^^^^^^^

``telescope``
    telescope shared C library
``pre_build_docs``, ``build_docs``
    targets needed to generate documentation
``uninstall``
    uninstall telescope from your machine
``test_*``
    various CTest routines

Options
^^^^^^^

``BUILD_TESTS``
    build the test suite. On by default
``BUILD_DOCS``
    enable the docs build targets. Off by default

Usage: Docs
^^^^^^^^^^^

To build the documentation, first turn ON the BUILD_DOCS option.
Then, first build the `pre_build_docs` target. This
generates files into `docs/.doxygen`. Afterwards, build the `build_docs`
target, which will use sphinx to generate the documentation as a html
website, in `docs/out`.

Usage: Tests
^^^^^^^^^^^^

With BUILD_TESTS turned ON, all test targets become available.
Build them, then, in the CMake output directory (usually Telescope/cmake-build-debug),
run:

    make test

This will run all tests, printing their output to the console.

#]=======================================================================]

cmake_minimum_required(VERSION 3.13)

project(telescope VERSION 0.2.0)

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

    include/vulkan_interface.hpp
    include/bullet_interface.hpp
    include/common.hpp
    include/physics_object.hpp
    include/vertex.hpp
    src/src.cpp
    include/collision_event.hpp

    include/logging.hpp
    src/logging.inl

    include/exceptions.hpp
    src/exceptions.cpp

    include/input_handler.hpp
    src/input_handler.cpp

    include/vector.hpp

    include/key_or_button.hpp

    include/time.hpp
    src/time.cpp

    include/window.hpp
    src/window.cpp

    include/audio.hpp

    include/music.hpp
    src/music.cpp
        include/music_handler.hpp src/music_handler.cpp)

set_target_properties(telescope PROPERTIES
  LINKER_LANGUAGE CXX
  CXX_STANDARD 20
  MACOSX_RPATH 1
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
  POSITION_INDEPENDENT_CODE ON
)

target_include_directories(telescope PUBLIC
  "${CMAKE_SOURCE_DIR}"
  "${BULLET_INCLUDE_DIRS}"
  "${CMAKE_SOURCE_DIR}/VulkanMemoryAllocator-Hpp"
  "${SHADERC_INCLUDE_DIRS}"
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

# TODO: REMOVE THIS
add_executable(debug_main main.cpp)
target_link_libraries(debug_main PRIVATE telescope)
set_target_properties(telescope PROPERTIES
  CXX_STANDARD 20
)
# TODO REMOVE THIS

### TESTS ####

option(BUILD_TESTS "build telescope unit tests" ON)
if (BUILD_TESTS)

    # \brief: declare a test
    # \param: test_name, has to be equal to the name of the actual .cpp inside ./test
    function(declare_test test_name)

        add_executable(
            ${test_name}
            "${PROJECT_SOURCE_DIR}/test/${test_name}.cpp"
            "${PROJECT_SOURCE_DIR}/test/test.hpp"
        )

        target_link_libraries(${test_name} PRIVATE
            telescope
            ${shaderc_shared}
        )

        target_include_directories(${test_name} PRIVATE
            ${CMAKE_SOURCE_DIR}
            ${BULLET_INCLUDE_DIRS}
            ${SHADERC_INCLUDE_DIRS}
            "${CMAKE_SOURCE_DIR}/VulkanMemoryAllocator-Hpp/include"
        )

        set_target_properties(${test_name} PROPERTIES
            LINKER_LANGUAGE CXX
        )

        add_test(NAME ${test_name} COMMAND ${test_name})
    endfunction()

    #declare_test(template_test)
    declare_test(time)
    declare_test(input_handler)
endif()

### GENERATE DOCS ###

option(BUILD_DOCS "build telescope documentation" OFF)
if (BUILD_DOCS)
    include("docs/CMakeLists.txt")
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

