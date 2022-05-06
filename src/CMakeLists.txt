add_library(telescope SHARED telescope.cpp telescope.h)

target_include_directories(telescope PUBLIC .)

find_package(SDL2 REQUIRED)

target_include_directories(telescope PRIVATE
    "../bullet3"
    "../bullet3/src"
    "../VulkanMemoryAllocator-Hpp/include"
)

target_link_libraries(telescope PRIVATE
    SDL2
    SDL2_image
    SDL2_mixer
    SDL2_net
    SDL2_ttf
    # UUUUUUUGH
    BulletFileLoader
    Bullet2FileLoader
    Bullet3Collision
    Bullet3Common
    Bullet3Dynamics
    Bullet3Geometry
    Bullet3OpenCL_clew
    BulletCollision
    BulletDynamics
    BulletInverseDynamics
    BulletInverseDynamicsUtils
    BulletWorldImporter
    BulletRobotics
    BulletRoboticsGUI
    OpenGLWindow
    BulletSoftBody
    ConvexDecomposition
    GIMPACTUtils
    HACD
    LinearMath
    gwen
    BussIK
    clsocket
    shaderc_shared
)
