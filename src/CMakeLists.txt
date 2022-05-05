add_library(telescope SHARED telescope.cpp)

find_package(Bullet REQUIRED)

target_include_directories(telescope PRIVATE ${BULLET_INCLUDE_DIRS})
target_link_libraries(telescope PRIVATE 
  SDL2 SDL2_image SDL2_mixer SDL2_net SDL2_ttf 
  ${BULLET_LIBRARIES}
  shaderc_shared)