//
// Copyright 2022, Joshua Higginbotham
//

//
//  this file contains the C interface used to generate
//  wrappers in other programming languages. not all
//  functions are exposed due to use of C++ arguments
//  and return types. To access all Telescope functionality
//  use the telescope.hpp header instead
//

#pragma once

#include <include/physics_object.hpp>
#include <include/collision_event.hpp>

#ifdef __cplusplus
extern "C" {
#endif

/// \brief add a rigid, axis-aligned collision box to the state
/// \param id: id of the newly created object
/// \param size_x: size along the x-dimension
/// \param size_y: size along the y-dimension
/// \param size_z: size along the z-dimension
/// \param mass: mass of the box
/// \param position_x: x-coordinate of the position of the box
/// \param position_y: y-coordinate of the position of the box
/// \param position_z: z-coordinate of the position of the box
/// \param is_kinematic: should the box be a kinematic object
void TS_BtAddRigidBox(
    int id,
    float size_x, float size_y, float size_z,
    float mass,
    float position_x, float position_y, float position_z,
    bool is_kinematic
);

/// \brief add a static, axis-aligned collision box to the state
/// \param id: id of the newly created object
/// \param size_x: size along the x-dimension
/// \param size_y: size along the y-dimension
/// \param size_z: size along the z-dimension
/// \param position_x: x-coordinate of the position of the box
/// \param position_y: y-coordinate of the position of the box
/// \param position_z: z-coordinate of the position of the box
void TS_BtAddStaticBox(
    int id,
    float size_x, float size_y, float size_z,
    float position_x, float position_y, float position_z
);

/// \brief add a box-shaped trigger to the state
/// \param id: id of the newly created object
/// \param size_x: size along the x-dimension
/// \param size_y: size along the y-dimension
/// \param size_z: size along the z-dimension
/// \param position_x: x-coordinate of the position of the box
/// \param position_y: y-coordinate of the position of the box
/// \param position_z: z-coordinate of the position of the box
void TS_BtAddTriggerBox(
    int id,
    float size_x, float size_y, float size_z,
    float position_x, float position_y, float position_z
);

/// \brief remove a physics object from the state
/// \param id: id of the object
void TS_BtRemovePhysicsObject(int id);

/// \brief set the linear velocity of a physics object
/// \param id: id of the object
/// \param velocity_x: velocity along the x-dimension
/// \param velocity_y: velocity along the y-dimension
/// \param velocity_z: velocity along the z-dimension
void TS_BtSetLinearVelocity(
    int id,
    float velocity_x, float velocity_y, float velocity_z
);

/// \brief get the linear velocity of a physics object
/// \param id: id of the object
/// \returns TS_VelocityInfo object describing the velocity along each dimension
struct TS_VelocityInfo TS_BtGetLinearVelocity(int id);

/// \brief set the global gravity
/// \param gravity_x: acceleration along the x-dimension
/// \param gravity_y: acceleration along the y-dimension
/// \param gravity_z: acceleration along the z-dimension
void TS_BtSetGravity(float gravity_x, float gravity_y, float gravity_z);

/// \brief set the outer margin of a specific collision object
/// \param id: id of the object
/// \param margin: distance between the surface and the outer margin of the object
void TS_BtSetCollisionMargin(int id, float margin);

/// \brief advance the physics simulation by one step
void TS_BtStepSimulation();

/// \brief query the next collision event
/// \returns TS_CollisionEvent, contains two ids of colliding objects
struct TS_CollisionEvent TS_BtGetNextCollision();

/// \brief get the position of an object
/// \returns TS_PositionInfo, position in 3d space
struct TS_PositionInfo TS_BtGetPosition(int id);

/// \brief get current SDL error description
/// \return C-string containing the error message
const char * TS_SDLGetError();

/// \brief draw a rectangle
/// \param r: red component of the color (in RGBA)
/// \param g: green component of the color (in RGBA)
/// \param b: blue component of the color (in RGBA)
/// \param alpha: transparency component of the color (in RGBA)
/// \param x: x-position of the upper left corner of the rectangle
/// \param y: y-position of the upper left corner of the rectangle
/// \param width: size along the x-dimension
/// \param height: size along the y-dimension
void TS_VkCmdDrawRect(
    float r, float g, float b, float alpha,
    float x, float y,
    float width, float height
);

/// \brief draw a sprite
/// \param image_path: path to image on disk
/// \param r: red component of the color (in RGBA)
/// \param g: green component of the color (in RGBA)
/// \param b: blue component of the color (in RGBA)
/// \param alpha: transparency component of the color (in RGBA)
/// \param region_x: x-coordinate of the top left corner of the subregion
/// \param region_y: y-coordinate of the top left corner of the subregion
/// \param region_width: size of the subregion along the x-dimension
/// \param region_height: size of the subregion along the x-dimension
/// \param cell_w: width of each cell of the grid
/// \param cell_h: height of each cell of the grid
/// \param cell_index_i: x-index of the cell
/// \param cell_index_j: y-index of the cell
/// \param position_x: x-coordinate of the top left corner of the sprite
/// \param position_y: y-coordinate of the top left corner of the sprite
/// \param scale_x: scale along the x-dimension
/// \param scale_y: scale along the y-dimension
void TS_VkCmdDrawSprite(
    const char * image_path,
    float r, float g, float b, float alpha,
    int region_x, int region_y, int region_width, int region_height,
    int cell_w, int cell_h, int cell_index_i, int cell_index_j,
    float position_x, float position_y,
    float scale_x, float scale_y
);

/// \brief clear the render window with a color
/// \param r: red component of the color (in RGBA)
/// \param g: green component of the color (in RGBA)
/// \param b: blue component of the color (in RGBA)
/// \param alpha: transparency component of the color (in RGBA)
void TS_VkCmdClearColorImage(float r, float g, float b, float alpha);

/// \brief start the drawing pass
void TS_VkBeginDrawPass();

/// \brief finish the drawing pass by rendering a solid color
/// \param r: red component of the color (in RGBA)
/// \param g: green component of the color (in RGBA)
/// \param b: blue component of the color (in RGBA)
/// \param alpha: transparency component of the color (in RGBA)
void TS_VkEndDrawPass(float r, float g, float b, float a);

/// \brief initialize the state
/// \param title: title of the window
/// \param width: width of the window, in pixels
/// \param height: height of the window, in pixels
void TS_Init(const char * title, int width, int height);

/// \brief shutdown the state
void TS_Quit();

/// \brief play a sound
/// \param path: path to the sound file
/// \param loops: [optional] number of loops, -1 for infinite
/// \param ticks: [optional] maximum time to play a sample, in milliseconds, -1 for infinite
void TS_PlaySound(const char* path, int loops=0, int ticks=-1);

#ifdef __cplusplus
}
#endif