//
// Copyright 2022, Joshua Higginbotham
//

#pragma once

#include <vulkan/vulkan.hpp>
#include <vk_mem_alloc.hpp>

#include <glm/glm.hpp>
#include <shaderc/shaderc.hpp>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_net.h>
#include <SDL2/SDL_vulkan.h>

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <queue>
#include <utility>
#include <cmath>

#include "telescope.h"

/// \brief texture object
struct TS_Texture {

  /// \brief image
  std::pair<vk::Image, vma::Allocation> img;

  /// \brief current view
  vk::ImageView view;

  /// \brief size along x-dimension
  uint32_t width;
  /// \brief size along y-dimension
  uint32_t height;

  /// \brief file name
  std::string fname;
};

/// \brief vertex object
struct TS_Vertex {

  /// \brief position in 2D space
  glm::vec2 pos;

  /// \brief uv coordinate
  glm::vec2 uv;

  /// \brief color, in RGBA
  glm::vec4 col;

  /// \brief texture id
  int tex;

  /// \brief ctor
  /// \param x: x position
  /// \param y: y position
  /// \param r: red component
  /// \param g: green component
  /// \param b: blue component
  /// \param a: transparency component
  /// \param u: u-coordinate
  /// \param v: v-coordinate
  /// \param t: texture id
  TS_Vertex(float x, float y, float r, float g, float b, float a, float u = 0, float v = 0, int t = -1);

  /// \brief get vertices vulkan binding description
  /// \returns description
  static vk::VertexInputBindingDescription getBindingDescription();

  /// \brief get vertices vulkan attribute description
  /// \returns 4-array of descriptions
  static std::array<vk::VertexInputAttributeDescription, 4> getAttributeDescriptions();
};

/// \brief phyiscs object
struct TS_PhysicsObject {

  /// \brief internal bullet collision object
  btCollisionObject * cobj;

  /// \brief collision shape
  btCollisionShape * cshape;

  /// \brief rigid body
  btRigidBody * rbody;

  /// \brief default motion state
  btDefaultMotionState * dmstate;

  /// \brief ctor
  /// \param s: bullet collision shape
  /// \param mass: [optional] mass
  /// \param isKinematic: [optional] is a kinematic object
  /// \param isTrigger: [optional] is a trigger object
  /// \param initPos: [optional] initial position in 3d space
  /// \param initRot: [optional] initial quaternion
  TS_PhysicsObject(btCollisionShape * s,
    float mass = 0.0f,
    bool isKinematic = false,
    bool isTrigger = false,
    const btVector3 &initPos = btVector3(0,0,0),
    const btQuaternion &initRot = btQuaternion(0,0,1,1));

  /// \brief dtor
  ~TS_PhysicsObject();

  /// \brief access transforms
  /// \returns btTransform object
  btTransform getTransform();
};

/// \brief trigger debug callback
/// \param messageSeverity: severity
/// \param messageType: type of message
/// \param pCallbackData: vulkan supplied callback data
/// \param pUserData: user supplied callback data
/// \returns always false
static VKAPI_ATTR vk::Bool32 VKAPI_CALL debugCallback(
  vk::DebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
  vk::DebugUtilsMessageTypeFlagsEXT messageType,
  const vk::DebugUtilsMessengerCallbackDataEXT* pCallbackData,
  void* pUserData);

/// \brief create vulkan image view
/// \brief img: vulkan image
/// \brief fmt: vulkan format
/// \brief flags: image aspect flags
/// \returns created image view
vk::ImageView TS_VkCreateImageView(vk::Image img, vk::Format fmt, vk::ImageAspectFlagBits flags);

/// \brief get vulkan supported depth format
/// \returns true if format supported, false otherwise
vk::Bool32 TS_VkGetSupportedDepthFormat();

/// \brief TODO: idk what this does
/// \param size: vulkan device size
/// \param usage: vulkan buffer usage flags
/// \param properties: vulkan memory properties
/// \param allocFlags: [optional] allocation flags
/// \returns pair where .first is the vulkan buffer, .second is the vma::Allcation object
std::pair<vk::Buffer, vma::Allocation> TS_VmaCreateBuffer(
    vk::DeviceSize size, vk::Flags<vk::BufferUsageFlagBits> usage,
    vk::Flags<vk::MemoryPropertyFlagBits> properties,
    vma::AllocationCreateFlags allocFlags = vma::AllocationCreateFlags());

/// \brief create a vma image
/// \param width: size along x-dimension
/// \param height: size along y-dimension
/// \param fmt: vulkan format
/// \param tiling: vulkan image tiling
/// \param usage: vulkan image usage flags
/// \param properties: vulkan memory properties
/// \param allocFlags: [optional] vma allocation flags
/// \returns pair where .first is the vulkan buffer, .second is the vma::Allcation object
std::pair<vk::Image, vma::Allocation> TS_VmaCreateImage(
  uint32_t width,
  uint32_t height,
  vk::Format fmt,
  vk::ImageTiling tiling,
  vk::Flags<vk::ImageUsageFlagBits> usage,
  vk::Flags<vk::MemoryPropertyFlagBits> properties,
  vma::AllocationCreateFlags allocFlags = vma::AllocationCreateFlags());

/// \brief get sdl error
/// \returns error message
const char * TS_SDLGetError();

/// \brief get normalized device coordinates along the x and y axes
/// \param x: x coordinate
/// \returns normalized y coordinate
float TS_NDCX(float x);

/// \brief get normalized device coordinates along the x and y axes
/// \param y: y coordinate
/// \returns normalized x coordinate
float TS_NDCY(float y);

/// \brief normalize rectangle
/// \param x: x-coordinate of the top left corner
/// \param y: y-coordinate of the top left corner
/// \param w: size in x-dimension
/// \param h: size in y-dimension
/// \returns rectangle as 4-array of floats
std::array<float, 4> TS_NDCRect(float x, float y, float w, float h);

/// \brief normalize texture coordinates along the u axis
/// \param x: x-coordinate
/// \param w: width
/// \returns normalized y-coordinate
float TS_NTCU(int x, int w);

/// \brief normalize texture coordinates along the v axis
/// \param y: y-coordinate
/// \param h: height
/// \returns normalized x-coordinate
float TS_NTCV(int y, int h);

/// \brief normalize texture coordinate rectangle
/// \param x: x-coordinate of top left corner
/// \param y: y-coordinate of top left corner
/// \param w: width
/// \param h: height
/// \param w2: new width
/// \param h2: new height
/// \returns rect as 4-array
std::array<float, 4> TS_NTCRect(int x, int y, int w, int h, int w2, int h2);

/// \brief TODO: idk what this does
void TS_Add4Indices();

/// \brief begin vulkan scratch buffer
/// \returns vulkan command buffer
vk::CommandBuffer TS_VkBeginScratchBuffer();

/// \brief submit vulkan scratch buffer
/// \param tmp: command buffer
void TS_VkSubmitScratchBuffer(vk::CommandBuffer &tmp);

/// \brief transition the layout of an image
/// \param img: image
/// \param oldLayout: previous layout
/// \param newLayout: new layout
void TS_VkTransitionImageLayout(vk::Image img, vk::ImageLayout oldLayout, vk::ImageLayout newLayout);

/// \brief copy vulkan buffer to image
/// \param buf: buffer
/// \param img: image
/// \param wdth: width of the TODO: buffer or image?
/// \param hght: height of the TODO: buffer or image?
void TS_VkCopyBufferToImage(vk::Buffer buf, vk::Image img, uint32_t wdth, uint32_t hght);

/// \brief TODO idk what this does
void TS_VkWriteDescriptorSet();

/// \brief load texture
/// \param img: path to image on disk
int TS_VkLoadTexture(const char * img);

/// \brief unload texture
/// \param img: texture path
void TS_VkUnloadTexture(const char * img);

/// \param draw a colored rectangle
/// \param r: red component of the color
/// \param g: green component of the color
/// \param b: blue component of the color
/// \param a: transparency component of the color
/// \param x: x-coordinate of the top left corner
/// \param y: y-coordinate of the top left corner
/// \param w: width
/// \param h: height
void TS_VkCmdDrawRect(float r, float g, float b, float a, float x, float y, float w, float h);

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
void TS_VkCmdDrawSprite(const char * img, float r, float g, float b, float a, int rx, int ry, int rw, int rh, int cw, int ch, int ci, int cj, float px, float py, float sx, float sy);

/// \brief clear the render window with a color
/// \param r: red component of the color (in RGBA)
/// \param g: green component of the color (in RGBA)
/// \param b: blue component of the color (in RGBA)
/// \param alpha: transparency component of the color (in RGBA)
void TS_VkCmdClearColorImage(float r, float g, float b, float a);

/// \brief acquire next vulkan image
void TS_VkAcquireNextImage();

/// \brief reset the vulkan command buffer
void TS_VkResetCommandBuffer();

/// \brief begin the vulkan command buffer
void TS_VkBeginCommandBuffer();

/// \brief fill the render area with a color
/// \param r: red component of the color
/// \param g: green component of the color
/// \param b: blue component of the color
/// \param a: transparency component of the color
void TS_VkDraw(float r, float g, float b, float a);

/// \brief end vulkan command buffer
void TS_VkEndCommandBuffer();

/// \brief queue vulkan submit info
void TS_VkQueueSubmit();

/// \brief queue vulkan present
void TS_VkQueuePresent();

/// \brief set the vulkan debug messenger
/// \param dbmci: debug messenger create info
void TS_VkPopulateDebugMessengerCreateInfo(vk::DebugUtilsMessengerCreateInfoEXT& dbmci);

/// \brief create vulkan instance
void TS_VkCreateInstance();

/// \brief create vulkan debug messenger
void TS_VkCreateDebugMessenger();

/// \brief create vulkan surface
void TS_VkCreateSurface();

/// \brief select the physical device
void TS_VkSelectPhysicalDevice();

/// \brief select vulkan queue family
void TS_VkSelectQueueFamily();

/// \brief created device
void TS_VkCreateDevice();

/// \brief create the vma allocator object
void TS_VmaCreateAllocator();

/// \brief create the vma buffers
void TS_VmaCreateBuffers();

/// \brief create the vulkan swapchain
void TS_VkCreateSwapchain();

/// \brief create the vulkan image views
void TS_VkCreateImageViews();

/// \brief setup the vulkan depth stencil
void TS_VkSetupDepthStencil();

/// \brief create the vulkan render pass
void TS_VkCreateRenderPass();

/// \brief initialize the vulkan shader module
vk::ShaderModule TS_VkCreateShaderModule(std::string code, shaderc_shader_kind kind, bool optimize = false);

/// \brief create the vulkan descriptor set
void TS_VkCreateDescriptorSet();

/// \brief create the vulkan triangle pipeline
void TS_VkCreateTrianglePipeline();

/// \brief create the vulkan framebuffer
void TS_VkCreateFramebuffers();

/// \brief create the vulkan command pool
void TS_VkCreateCommandPool();

/// \brief allocate the vulkan command buffer
void TS_VkAllocateCommandBuffers();

/// \brief create the vulkan semaphores
void TS_VkCreateSemaphores();

/// \brief create the vulkan fences
void TS_VkCreateFences();

/// \brief initialize the vulkan state
void TS_VkInit();

/// \brief being the vulkan draw pass
void TS_VkBeginDrawPass();

/// \brief finish the drawing pass by rendering a solid color
/// \param r: red component of the color (in RGBA)
/// \param g: green component of the color (in RGBA)
/// \param b: blue component of the color (in RGBA)
/// \param alpha: transparency component of the color (in RGBA)
void TS_VkEndDrawPass(float r, float g, float b, float a);

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
void TS_BtAddRigidBox(int id, float hx, float hy, float hz, float m, float px, float py, float pz, bool isKinematic = false);

/// \brief add a static, axis-aligned collision box to the state
/// \param id: id of the newly created object
/// \param size_x: size along the x-dimension
/// \param size_y: size along the y-dimension
/// \param size_z: size along the z-dimension
/// \param position_x: x-coordinate of the position of the box
/// \param position_y: y-coordinate of the position of the box
/// \param position_z: z-coordinate of the position of the box
void TS_BtAddStaticBox(int id, float hx, float hy, float hz, float px, float py, float pz);

/// \brief add a box-shaped trigger to the state
/// \param id: id of the newly created object
/// \param size_x: size along the x-dimension
/// \param size_y: size along the y-dimension
/// \param size_z: size along the z-dimension
/// \param position_x: x-coordinate of the position of the box
/// \param position_y: y-coordinate of the position of the box
/// \param position_z: z-coordinate of the position of the box
void TS_BtAddTriggerBox(int id, float hx, float hy, float hz, float px, float py, float pz);

/// \brief remove a physics object from the state
/// \param id: id of the object
void TS_BtRemovePhysicsObject(int id);

/// \brief set the linear velocity of a physics object
/// \param id: id of the object
/// \param velocity_x: velocity along the x-dimension
/// \param velocity_y: velocity along the y-dimension
/// \param velocity_z: velocity along the z-dimension
void TS_BtSetLinearVelocity(int id, float vx, float vy, float vz);

/// \brief get the linear velocity of a physics object
/// \param id: id of the object
/// \returns TS_VelocityInfo object describing the velocity along each dimension
TS_VelocityInfo TS_BtGetLinearVelocity(int id);

/// \brief advance the physics simulation by one step
void TS_BtStepSimulation();

/// \brief query the next collision event
/// \returns TS_CollisionEvent, contains two ids of colliding objects
TS_CollisionEvent TS_BtGetNextCollision();

/// \brief get the position of an object
/// \returns TS_PositionInfo, position in 3d space
TS_PositionInfo TS_BtGetPosition(int id);

/// \brief set the global gravity
/// \param gravity_x: acceleration along the x-dimension
/// \param gravity_y: acceleration along the y-dimension
/// \param gravity_z: acceleration along the z-dimension
void TS_BtSetGravity(float gx, float gy, float gz);

/// \brief set the outer margin of a specific collision object
/// \param id: id of the object
/// \param margin: distance between the surface and the outer margin of the object
void TS_BtSetCollisionMargin(int id, float margin);

/// \brief initialize the physics state
void TS_BtInit();

/// \brief deallocate the vulkan fences
void TS_VkDestroyFences();

/// \brief deallocate the vulkan semaphores
void TS_VkDestroySemaphores();

/// \brief free the vulkan command buffers
void TS_VkFreeCommandBuffers();

/// \brief destroy the vulkan command pool
void TS_VkDestroyCommandPool();

/// \brief destroy the vulkan framebuffers
void TS_VkDestroyFramebuffers();

/// \brief destroy the vulkan triangle pipeline
void TS_VkDestroyTrianglePipeline();

/// \brief destroy the vulkan descriptor set
void TS_VkDestroyDescriptorSet();

/// \brief destroy the vulkan render pass
void TS_VkDestroyRenderPass();

/// \brief destroy the vulkan depth stencil
void TS_VkTeardownDepthStencil();

/// \brief destroy the vulkan image view
void TS_VkDestroyImageViews();

/// \brief destroy the vulkan swap chain
void TS_VkDestroySwapchain();

/// \brief destroy the vma buffer
void TS_VmaDestroyBuffers();

/// \brief deallocate all vulkan textures
void TS_VkDestroyTextures();

/// \brief destroy the vulkan allocator
void TS_VmaDestroyAllocator();

/// \brief destroy the vulkan device
void TS_VkDestroyDevice();

/// \brief destroy the vulkan surface
void TS_VkDestroySurface();

/// \brief destroy the vulkan debug messenger
void TS_VkDestroyDebugMessenger();

/// \brief destroy the vulkan instance
void TS_VkDestroyInstance();

/// \brief quit the vulkan state
void TS_VkQuit();

/// \brief quit the bullet state
void TS_BtQuit();

/// \brief initialize the state
void TS_Init(const char * ttl, int wdth, int hght);

/// \brief quit the state
void TS_Quit();

/// \brief play a sound
/// \param path: path to the sound file
/// \param loops: [optional] number of loops, -1 for infinite
/// \param ticks: [optional] maximum time to play a sample, in milliseconds, -1 for infinite
void TS_PlaySound(const char* sound_file, int loops, int ticks);