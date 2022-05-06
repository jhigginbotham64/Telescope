### DECLARE LIBRARY ###

add_library(telescope SHARED telescope.cpp telescope.h)

### FIND DEPENDENCIES ###

find_package(Bullet REQUIRED)
find_package(SDL2 REQUIRED)
find_library(shaderc_shared REQUIRED NAMES libshaderc_shared.so PATHS /home/clem/Desktop/shaderc/lib)

### LIBRARY INCLUDE DIRECTORIES ###

target_include_directories(telescope PUBLIC .
    "${CMAKE_SOURCE_DIR}"
)

target_include_directories(telescope PRIVATE
    "${BULLET_INCLUDE_DIRS}"
    "../VulkanMemoryAllocator-Hpp"
    /home/clem/Desktop/shaderc/include
)

### LINKING ###

target_link_libraries(telescope PUBLIC
    SDL2
    SDL2_image
    SDL2_mixer
    SDL2_net
    SDL2_ttf
    ${BULLET_LIBRARIES}
    ${shader_c_shared}
)

### TESTS ###

option(BUILD_TESTING "build telescope test suite" ON)
if (BUILD_TESTING)

    # \brief: declare a test
    # \param: test_name, has to be equal to the name of the actual .cpp inside ./test
    function(declare_test test_name)
        add_executable(
            "${test_name}"
            "../test/${test_name}.cpp"
            "../test/test.hpp"
        )

        target_link_libraries("${test_name}" telescope)
        add_test(NAME "${test_name}" COMMAND "${test_name}")
    endfunction()

    declare_test(template_test)
endif()
