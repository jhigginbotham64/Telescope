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

project(telescope VERSION 0.3.0 LANGUAGES C CXX)

include(GNUInstallDirs)
include(CTest)

### FIND DEPENDENCIES ###

find_library(box2d REQUIRED NAMES box2d)

find_library(SDL2 REQUIRED NAMES SDL2)
find_library(SDL2_image REQUIRED NAMES SDL2_image)
find_library(SDL2_mixer REQUIRED NAMES SDL2_mixer)
find_library(SDL2_ttf REQUIRED NAMES SDL2_ttf) # unused
find_library(SDL2_net REQUIRED NAMES SDL2_net) # unused

find_library(vulkan REQUIRED NAMES vulkan)
# find_package(shaderc_shared REQUIRED)      # unused

include(CheckIncludeFileCXX)
CHECK_INCLUDE_FILE_CXX("glm/glm.hpp" GLM_FOUND)
if(NOT GLM_FOUND)
    message(FATAL_ERROR "Missing Dependency: glm")
endif()

CHECK_INCLUDE_FILE_CXX("box2d/box2d.h" BOX2D_FOUND)
if(NOT BOX2D_FOUND)
    message(FATAL_ERROR "Missing Dependency: box2d")
endif()

### TELESCOPE ###

set(CMAKE_CXX_STANDARD 17)
add_library(telescope SHARED

    telescope.hpp

    include/logging.hpp
    src/logging.inl

    include/exceptions.hpp
    src/exceptions.cpp

    include/input_handler.hpp
    src/input_handler.cpp

    include/vector.hpp

    include/key_or_button.hpp
    src/key_or_button.cpp

    include/time.hpp
    src/time.cpp

    include/window.hpp
    src/window.cpp

    include/music.hpp
    src/music.cpp

    include/music_handler.hpp
    src/music_handler.cpp
    src/music.inl

    include/sound.hpp
    src/sound.cpp

    include/sound_handler.hpp
    src/sound_handler.cpp

    include/color.hpp
    src/color.cpp

    include/texture.hpp
    src/texture.cpp

    include/render_texture.hpp
    src/render_texture.cpp

    include/static_texture.hpp
    src/static_texture.cpp

    include/vertex.hpp
    src/vertex.cpp

    include/renderable.hpp
    src/renderable.cpp

    include/render_target.hpp

    include/shape.hpp
    src/shape.cpp

    include/rectangle_shape.hpp
    src/rectangle_shape.cpp

    include/triangle_shape.hpp
    src/triangle_shape.cpp

    include/circle_shape.hpp
    src/circle_shape.cpp

    include/angle.hpp
    src/angle.cpp

    include/geometric_shapes.hpp

    include/polygon_shape.hpp
    src/polygon_shape.cpp

    include/common.hpp
    src/common.cpp

    include/transform.hpp
    src/transform.cpp

    include/camera.hpp
    src/camera.cpp

    include/c_interface.hpp
    src/c_interface.cpp

    include/physics.hpp

    include/physics_world.hpp
    src/physics_world.cpp

    include/collision_shape.hpp
    src/collision_shape.cpp

    include/collision_circle.hpp
    src/collision_circle.cpp

    include/collision_polygon.hpp
    src/collision_polygon.cpp

    include/collision_line.hpp
    src/collision_line.cpp

    include/collision_wireframe.hpp
    src/collision_wireframe.cpp

    include/collision_circle_shape.hpp
    include/collision_rectangle_shape.hpp
    include/collision_triangle_shape.hpp
    include/collision_line_shape.hpp
    include/collision_wireframe_shape.hpp
    src/collision_x_shape.cpp
)

set_target_properties(telescope PROPERTIES
  LINKER_LANGUAGE C
  CXX_STANDARD 17
  MACOSX_RPATH 1
  LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}# TODO "${CMAKE_BINARY_DIR}/lib"
  POSITION_INDEPENDENT_CODE ON
)

target_include_directories(telescope PUBLIC
  "${CMAKE_SOURCE_DIR}"
)

target_link_libraries(telescope PUBLIC
    ${SDL2}
    ${SDL2_image}
    ${SDL2_mixer}
    # ${SDL2_net} # unused
    # ${SDL2_ttf} # unused
    # ${vulkan}   # unused
    ${box2d}
)

# TODO: REMOVE THIS vvv

add_executable(debug_main main.cpp)
target_link_libraries(debug_main PUBLIC telescope)
set_target_properties(telescope PROPERTIES
    CXX_STANDARD 17
    LINKER_LANGUAGE C
)

# TODO REMOVE THIS ^^

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

