//
// Copyright 2022, Joshua Higginbotham
//

#pragma once

#include <vulkan/vulkan.hpp>

#include <vk_mem_alloc.hpp>

#include <shaderc/shaderc.hpp>

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

#ifdef __cplusplus
    extern "C" {
#endif

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
void TS_VkEndDrawPass(float r, float g, float b, float alpha);

}

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
/// \param img: vulkan image
/// \param fmt: vulkan format
/// \param flags: image aspect flags
/// \returns created image view
vk::ImageView TS_VkCreateImageView(vk::Image img, vk::Format fmt, vk::ImageAspectFlagBits flags);

/// \brief get vulkan supported depth format
/// \returns true if format supported, false otherwise
vk::Bool32 TS_VkGetSupportedDepthFormat();

/// \brief create a vma buffer
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

/// \brief write updated descriptor set info to device
void TS_VkWriteDescriptorSet();

/// \brief load texture
/// \param img: path to image on disk
int TS_VkLoadTexture(const char * img);

/// \brief unload texture
/// \param img: texture path
void TS_VkUnloadTexture(const char * img);

/// \brief clear an image with a color
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
/// \param alpha: transparency component of the color
void TS_VkDraw(float r, float g, float b, float alpha);

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

/// \brief finish the drawing pass by rendering a solid color
/// \param r: red component of the color (in RGBA)
/// \param g: green component of the color (in RGBA)
/// \param b: blue component of the color (in RGBA)
/// \param alpha: transparency component of the color (in RGBA)
void TS_VkEndDrawPass(float r, float g, float b, float a);

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