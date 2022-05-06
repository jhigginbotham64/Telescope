add_library(telescope SHARED telescope.cpp telescope.h)

target_include_directories(telescope PUBLIC .)

find_package(Bullet REQUIRED)
find_package(SDL2 REQUIRED)

target_include_directories(telescope PRIVATE
    ${BULLET_INCLUDE_DIRS}
    "../VulkanMemoryAllocator-Hpp"
)

target_link_libraries(telescope PRIVATE
    SDL2
    SDL2_image
    SDL2_mixer
    SDL2_net
    SDL2_ttf
    ${BULLET_LIBRARIES}
    shaderc_shared
)
