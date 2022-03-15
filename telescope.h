#include <vulkan/vulkan.hpp>

vk::ImageView TS_VkCreateImageView(vk::Image img, vk::Format fmt, vk::ImageAspectFlagBits flags);

vk::Bool32 TS_VkGetSupportedDepthFormat();

uint32_t TS_VkFindMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlagBits properties);

void TS_VkCreateImage(uint32_t width, uint32_t height, vk::Format fmt, vk::ImageTiling tiling,
                      vk::ImageUsageFlagBits usage, vk::MemoryPropertyFlagBits properties,
                      vk::Image& img, vk::DeviceMemory& imageMemory);

extern "C" const char * TS_GetSDLError();

extern "C" void TS_VkCmdDrawRect(float r, float g, float b, float a, int x, int y, int w, int h);

extern "C" void TS_VkCmdDrawSprite(const char * img, float a, int rx, int ry, int rw, int rh, int cx, int cy, int ci, int cj, int px, int py, int sx, int sy);

extern "C" void TS_VkCmdClearColorImage(float r, float g, float b, float a);

extern "C" void TS_VkAcquireNextImage();

extern "C" void TS_VkResetCommandBuffer();

extern "C" void TS_VkBeginCommandBuffer();

extern "C" void TS_VkBeginRenderPass(float r, float g, float b, float a);

extern "C" void TS_VkEndRenderPass();

extern "C" void TS_VkEndCommandBuffer();

extern "C" void TS_VkQueueSubmit();

extern "C" void TS_VkQueuePresent();

extern "C" void TS_VkCreateInstance();

extern "C" void TS_VkCreateSurface();

extern "C" void TS_VkSelectPhysicalDevice();

void TS_VkSelectQueueFamily();

extern "C" void TS_VkCreateDevice();

#define CLAMP(x, lo, hi)    ((x) < (lo) ? (lo) : (x) > (hi) ? (hi) : (x))
extern "C" void TS_VkCreateSwapchain();

extern "C" void TS_VkCreateImageViews();

extern "C" void TS_VkSetupDepthStencil();

extern "C" void TS_VkCreateRenderPass();

extern "C" void TS_VkCreateTrianglePipeline();

extern "C" void TS_VkCreateFramebuffers();

extern "C" void TS_VkCreateCommandPool();

extern "C" void TS_VkAllocateCommandBuffers();

extern "C" void TS_VkCreateSemaphores();

extern "C" void TS_VkCreateFences();

extern "C" void TS_VkInit();

extern "C" void TS_VkDestroyFences();

extern "C" void TS_VkDestroySemaphores();

extern "C" void TS_VkFreeCommandBuffers();

extern "C" void TS_VkDestroyCommandPool();

extern "C" void TS_VkDestroyFramebuffers();

extern "C" void TS_VkDestroyTrianglePipeline();

extern "C" void TS_VkDestroyRenderPass();

extern "C" void TS_VkTeardownDepthStencil();

extern "C" void TS_VkDestroyImageViews();

extern "C" void TS_VkDestroySwapchain();

extern "C" void TS_VkDestroyDevice();

extern "C" void TS_VkDestroySurface();

extern "C" void TS_VkDestroyInstance();

extern "C" void TS_VkQuit();

extern "C" void TS_Init(const char * ttl, int wdth, int hght);

extern "C" void TS_Quit();

extern "C" void TS_PlaySound(const char* sound_file, int loops, int ticks);