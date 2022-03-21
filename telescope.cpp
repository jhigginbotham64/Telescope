#include <glm/glm.hpp>
#include <shaderc/shaderc.hpp>

#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1
#define VULKAN_HPP_STORAGE_SHARED 1
#define VULKAN_HPP_STORAGE_SHARED_EXPORT 1
#include <vulkan/vulkan.hpp>
VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_net.h>
#include <SDL2/SDL_vulkan.h>

#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <set>
#include <utility>
#include <cmath>

#define VMA_IMPLEMENTATION
#define VMA_STATIC_VULKAN_FUNCTIONS 0
#define VMA_DYNAMIC_VULKAN_FUNCTIONS 1
#include "vma/vk_mem_alloc.hpp"

#include "telescope.h"

const char *window_name = NULL;
SDL_Window *win = NULL;
const vk::DeviceSize defaultBufferSize = 1024 * 64; // 64 kb
vk::Instance inst;
VkSurfaceKHR srf;
vk::PhysicalDevice pdev;
vk::Device dev;
uint32_t graphicsQueueFamilyIndex = -1;
uint32_t presentQueueFamilyIndex = -1;
vk::Queue gq;
vk::Queue pq;
vk::SwapchainKHR swapchain;
vk::SurfaceCapabilitiesKHR surfaceCapabilities;
vk::SurfaceFormatKHR surfaceFormat;
vk::Extent2D swapchainSize;
std::vector<vk::Image> swapchainImages;
uint32_t swapchainImageCount;
vk::PipelineLayout trianglePipelineLayout;
vk::Pipeline trianglePipeline;
std::vector<vk::ImageView> swapchainImageViews;
vk::Format depthFormat;
std::pair<vk::Image, vma::Allocation> depthImage;
std::pair<vk::Buffer, vma::Allocation> vertexStaging;
std::pair<vk::Buffer, vma::Allocation> indexStaging;
std::pair<vk::Buffer, vma::Allocation> vertexBuffer;
std::pair<vk::Buffer, vma::Allocation> indexBuffer;
std::vector<uint32_t> indices;
vk::ImageView depthImageView;
vk::RenderPass rp;
std::vector<vk::Framebuffer> swapchainFramebuffers;
vk::CommandPool cp;
std::vector<vk::CommandBuffer> cmdbufs;
vk::Semaphore imageAvailableSemaphore;
vk::Semaphore renderingFinishedSemaphore;
std::vector<vk::Fence> fences;
uint32_t frameIndex;
vma::Allocator al;
vk::DebugUtilsMessengerEXT dbm;

const std::vector<const char*> validationLayers = {
  "VK_LAYER_KHRONOS_validation"
};

#ifdef NDEBUG
  const bool enableValidationLayers = false;
#else
  const bool enableValidationLayers = true;
#endif

static VKAPI_ATTR vk::Bool32 VKAPI_CALL debugCallback(
    vk::DebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    vk::DebugUtilsMessageTypeFlagsEXT messageType,
    const vk::DebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData)
{
  if (messageSeverity >= vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning)
  {
    std::cerr << "debug callback: " << pCallbackData->pMessage << std::endl;
  }

  return VK_FALSE;
}

struct Vertex {
  glm::vec2 pos;
  glm::vec3 color;

  static vk::VertexInputBindingDescription getBindingDescription()
  {
    vk::VertexInputBindingDescription bindingDescription;
    bindingDescription.binding = 0;
    bindingDescription.stride = sizeof(Vertex);
    bindingDescription.inputRate = vk::VertexInputRate::eVertex;

    return bindingDescription;
  }

  static std::array<vk::VertexInputAttributeDescription, 2> getAttributeDescriptions()
  {
    std::array<vk::VertexInputAttributeDescription, 2> attributeDescriptions;

    attributeDescriptions[0].binding = 0;
    attributeDescriptions[0].location = 0;
    attributeDescriptions[0].format = vk::Format::eR32G32Sfloat;
    attributeDescriptions[0].offset = offsetof(Vertex, pos);

    attributeDescriptions[1].binding = 0;
    attributeDescriptions[1].location = 1;
    attributeDescriptions[1].format = vk::Format::eR32G32B32Sfloat;
    attributeDescriptions[1].offset = offsetof(Vertex, color);

    return attributeDescriptions;
  }
};

vk::ImageView TS_VkCreateImageView(vk::Image img, vk::Format fmt, vk::ImageAspectFlagBits flags)
{
  vk::ImageViewCreateInfo viewInfo;
  viewInfo.viewType = vk::ImageViewType::e2D;
  viewInfo.image = img;
  viewInfo.format = fmt;
  viewInfo.subresourceRange.aspectMask = flags;
  viewInfo.subresourceRange.baseMipLevel = 0;
  viewInfo.subresourceRange.levelCount = 1;
  viewInfo.subresourceRange.baseArrayLayer = 0;
  viewInfo.subresourceRange.layerCount = 1;

  return dev.createImageView(viewInfo);
}

vk::Bool32 TS_VkGetSupportedDepthFormat()
{
  std::vector<vk::Format> depthFormats = {
    vk::Format::eD32SfloatS8Uint,
    vk::Format::eD32Sfloat,
    vk::Format::eD24UnormS8Uint,
    vk::Format::eD16UnormS8Uint,
    vk::Format::eD16Unorm
  };

  for (auto& format : depthFormats)
  {
    vk::FormatProperties formatProps = pdev.getFormatProperties(format);
    if (formatProps.optimalTilingFeatures & vk::FormatFeatureFlagBits::eDepthStencilAttachment)
    {
      depthFormat = format;
      return true;
    }
  }

  return false;
}

std::pair<vk::Buffer, vma::Allocation> TS_VmaCreateBuffer(vk::DeviceSize size, vk::Flags<vk::BufferUsageFlagBits> usage, 
                      vk::Flags<vk::MemoryPropertyFlagBits> properties, 
                      vma::AllocationCreateFlags allocFlags = vma::AllocationCreateFlags())
{
  vk::BufferCreateInfo bufferInfo;
  bufferInfo.size = size;
  bufferInfo.usage = usage;
  bufferInfo.sharingMode = vk::SharingMode::eExclusive;

  vma::AllocationCreateInfo allocInfo {
    allocFlags,
    vma::MemoryUsage::eUnknown,
    properties
  };

  return al.createBuffer(bufferInfo, allocInfo);
}

std::pair<vk::Image, vma::Allocation> TS_VmaCreateImage(uint32_t width, uint32_t height, vk::Format fmt, vk::ImageTiling tiling,
                      vk::Flags<vk::ImageUsageFlagBits> usage, vk::Flags<vk::MemoryPropertyFlagBits> properties, 
                      vma::AllocationCreateFlags allocFlags = vma::AllocationCreateFlags())
{
  vk::ImageCreateInfo imageInfo;
  imageInfo.imageType = vk::ImageType::e2D;
  imageInfo.extent.width = width;
  imageInfo.extent.height = height;
  imageInfo.extent.depth = 1;
  imageInfo.mipLevels = 1;
  imageInfo.arrayLayers = 1;
  imageInfo.format = fmt;
  imageInfo.tiling = tiling;
  imageInfo.initialLayout = vk::ImageLayout::eUndefined;
  imageInfo.usage = usage;
  imageInfo.samples = vk::SampleCountFlagBits::e1;
  imageInfo.sharingMode = vk::SharingMode::eExclusive;

  vma::AllocationCreateInfo allocInfo {
    allocFlags,
    vma::MemoryUsage::eUnknown,
    properties
  };

  return al.createImage(imageInfo, allocInfo);
}

const char * TS_GetSDLError()
{
  return SDL_GetError();
}

void TS_VkCmdDrawRect(float r, float g, float b, float a, int x, int y, int w, int h)
{
  
}

void TS_VkCmdDrawSprite(const char * img, float a, int rx, int ry, int rw, int rh, int cx, int cy, int ci, int cj, int px, int py, int sx, int sy)
{
  
}

void TS_VkCmdClearColorImage(float r, float g, float b, float a)
{
  vk::ClearColorValue clearColor(std::array<float, 4>({r, g, b, a}));
  vk::ImageSubresourceRange imageRange(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1);

  cmdbufs[frameIndex].clearColorImage(swapchainImages[frameIndex], vk::ImageLayout::eGeneral, &clearColor, 1U, &imageRange);
}

void TS_VkAcquireNextImage()
{
  frameIndex = dev.acquireNextImageKHR(swapchain, UINT64_MAX, imageAvailableSemaphore).value;
  dev.waitForFences(1, &fences[frameIndex], VK_FALSE, UINT64_MAX);
  dev.resetFences(1, &fences[frameIndex]);
}

void TS_VkResetCommandBuffer()
{
  cmdbufs[frameIndex].reset();
}

void TS_VkBeginCommandBuffer()
{
  cmdbufs[frameIndex].begin({vk::CommandBufferUsageFlagBits::eSimultaneousUse});
}

void TS_VkDraw(float r, float g, float b, float a)
{
  // copy buffers
  vk::BufferCopy bfcpy;
  bfcpy.srcOffset = 0;
  bfcpy.dstOffset = 0;
  bfcpy.size = defaultBufferSize;
  cmdbufs[frameIndex].copyBuffer(vertexStaging.first, vertexBuffer.first, 1, &bfcpy);
  cmdbufs[frameIndex].copyBuffer(indexStaging.first, indexBuffer.first, 1, &bfcpy);

  // bind vertex buffer
  vk::Buffer vertexBuffers[] = {vertexBuffer.first};
  vk::DeviceSize offsets[] = {0};
  cmdbufs[frameIndex].bindVertexBuffers(0, 1, vertexBuffers, offsets);

  // bind index buffer
  cmdbufs[frameIndex].bindIndexBuffer(indexBuffer.first, 0, vk::IndexType::eUint32);

  // bind triangle pipeline
  cmdbufs[frameIndex].bindPipeline(vk::PipelineBindPoint::eGraphics, trianglePipeline);


  // begin render pass
  vk::RenderPassBeginInfo rpi {
    rp, swapchainFramebuffers[frameIndex]
  };
  rpi.renderArea.offset = vk::Offset2D();
  rpi.renderArea.extent = swapchainSize;

  std::vector<vk::ClearValue> clearValues(2);
  clearValues[0] = vk::ClearColorValue(std::array<float, 4>({r, g, b, a}));
  clearValues[1] = vk::ClearDepthStencilValue(VkClearDepthStencilValue({1.0f, 0}));

  rpi.clearValueCount = static_cast<uint32_t>(clearValues.size());
  rpi.pClearValues = clearValues.data();

  cmdbufs[frameIndex].beginRenderPass(rpi, vk::SubpassContents::eInline);

  // draw indexed
  cmdbufs[frameIndex].drawIndexed(static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);

  // end render pass
  cmdbufs[frameIndex].endRenderPass();
}

void TS_VkEndCommandBuffer()
{
  cmdbufs[frameIndex].end();
}

void TS_VkQueueSubmit()
{
  vk::PipelineStageFlags waitDestStageMask = vk::PipelineStageFlags(vk::PipelineStageFlagBits::eTransfer);
  vk::SubmitInfo submitInfo(1, &imageAvailableSemaphore, &waitDestStageMask, 1, &cmdbufs[frameIndex], 1, &renderingFinishedSemaphore);
  gq.submit(1, &submitInfo, fences[frameIndex]);
}

void TS_VkQueuePresent()
{
  vk::PresentInfoKHR pInfo(1, &renderingFinishedSemaphore, 1, &swapchain, &frameIndex);
  pq.presentKHR(pInfo);
  pq.waitIdle();
}

void TS_VkPopulateDebugMessengerCreateInfo(vk::DebugUtilsMessengerCreateInfoEXT& dbmci)
{
  dbmci.messageSeverity = vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | 
                            vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo | 
                            vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | 
                            vk::DebugUtilsMessageSeverityFlagBitsEXT::eError;
  dbmci.messageType = vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
                        vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
                        vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation;
  dbmci.pfnUserCallback = (PFN_vkDebugUtilsMessengerCallbackEXT)debugCallback;
}

void TS_VkCreateInstance()
{
  VULKAN_HPP_DEFAULT_DISPATCHER.init((PFN_vkGetInstanceProcAddr)SDL_Vulkan_GetVkGetInstanceProcAddr());

  unsigned int extensionCount = 0;
  SDL_Vulkan_GetInstanceExtensions(win, &extensionCount, nullptr);
  std::vector<const char *> extensionNames(extensionCount);
  SDL_Vulkan_GetInstanceExtensions(win, &extensionCount, extensionNames.data());
  
  vk::ApplicationInfo appInfo {
    window_name, 
    VK_MAKE_VERSION(0, 1, 0), 
    "Telescope", 
    VK_MAKE_VERSION(0, 1, 0),
    VK_API_VERSION_1_0
  };

  if (enableValidationLayers)
  {
    extensionNames.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
  }

  vk::InstanceCreateInfo ici {
    vk::InstanceCreateFlags(),
    &appInfo,
    0, NULL,
    static_cast<uint32_t>(extensionNames.size()), extensionNames.data()
  };

  vk::DebugUtilsMessengerCreateInfoEXT dbmci;
  if (enableValidationLayers)
  {
    ici.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
    ici.ppEnabledLayerNames = validationLayers.data();

    TS_VkPopulateDebugMessengerCreateInfo(dbmci);
    
    ici.pNext = &dbmci;
  }

  inst = vk::createInstance(ici);

  VULKAN_HPP_DEFAULT_DISPATCHER.init(inst);
}

void TS_VkCreateDebugMessenger()
{
  if (!enableValidationLayers) return;

  vk::DebugUtilsMessengerCreateInfoEXT dbmci;
  TS_VkPopulateDebugMessengerCreateInfo(dbmci);

  dbm = inst.createDebugUtilsMessengerEXT(dbmci);
}

void TS_VkCreateSurface()
{
  SDL_Vulkan_CreateSurface(win, inst, &srf);
}

void TS_VkSelectPhysicalDevice()
{
  pdev = inst.enumeratePhysicalDevices()[0]; // TODO improve selection
}

void TS_VkSelectQueueFamily()
{
  int graphicIndex = -1;
  int presentIndex = -1;
  int i = 0;
  for (const auto& qf : pdev.getQueueFamilyProperties())
  {
    if (qf.queueCount > 0 && qf.queueFlags & vk::QueueFlagBits::eGraphics) graphicIndex = i;
    bool presentSupport = pdev.getSurfaceSupportKHR(i, srf);
    if (qf.queueCount > 0 && presentSupport) presentIndex = i;
    if (graphicIndex != -1 && presentIndex != -1) break;
    ++i;
  }
  graphicsQueueFamilyIndex = graphicIndex;
  presentQueueFamilyIndex = presentIndex;
}

void TS_VkCreateDevice()
{
  const std::vector<const char*> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
  const float queue_priority[] = { 1.0f };
  float queuePriority = queue_priority[0];
  
  std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
  
  vk::DeviceQueueCreateInfo gr {
    vk::DeviceQueueCreateFlags(),
    graphicsQueueFamilyIndex, 
    1, 
    &queuePriority
  };

  vk::DeviceQueueCreateInfo pr {
    vk::DeviceQueueCreateFlags(),
    presentQueueFamilyIndex, 
    1, 
    &queuePriority
  };

  queueCreateInfos.push_back(gr);
  if (graphicsQueueFamilyIndex != presentQueueFamilyIndex)
  {
    queueCreateInfos.push_back(pr);
  }

  vk::PhysicalDeviceFeatures deviceFeatures = {};
  deviceFeatures.samplerAnisotropy = VK_TRUE;
  vk::DeviceCreateInfo deviceCreateInfo {
    vk::DeviceCreateFlags(),
    static_cast<uint32_t>(queueCreateInfos.size()), queueCreateInfos.data(),
    0, nullptr,
    static_cast<uint32_t>(deviceExtensions.size()), deviceExtensions.data(),
    &deviceFeatures
  };

  dev = pdev.createDevice(deviceCreateInfo);
  VULKAN_HPP_DEFAULT_DISPATCHER.init(dev);
  gq = dev.getQueue(graphicsQueueFamilyIndex, 0);
  pq = dev.getQueue(presentQueueFamilyIndex, 0);
}

void TS_VmaCreateAllocator()
{
  vma::VulkanFunctions vf {
    VULKAN_HPP_DEFAULT_DISPATCHER.vkGetInstanceProcAddr,
    VULKAN_HPP_DEFAULT_DISPATCHER.vkGetDeviceProcAddr
  };

  vma::AllocatorCreateInfo aci = {};
  aci.vulkanApiVersion = VK_API_VERSION_1_0;
  aci.physicalDevice = pdev;
  aci.device = dev;
  aci.instance = inst;
  aci.pVulkanFunctions = &vf;

  al = vma::createAllocator(aci);
}

void TS_VmaCreateBuffers()
{
  vertexStaging = TS_VmaCreateBuffer(defaultBufferSize, vk::BufferUsageFlagBits::eTransferSrc, 
                                    vk::MemoryPropertyFlagBits::eHostCoherent | vk::MemoryPropertyFlagBits::eHostVisible,
                                    vma::AllocationCreateFlagBits::eMapped);
  indexStaging = TS_VmaCreateBuffer(defaultBufferSize, vk::BufferUsageFlagBits::eTransferSrc, 
                                    vk::MemoryPropertyFlagBits::eHostCoherent | vk::MemoryPropertyFlagBits::eHostVisible,
                                    vma::AllocationCreateFlagBits::eMapped);
  vertexBuffer = TS_VmaCreateBuffer(defaultBufferSize, vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer,
                                    vk::MemoryPropertyFlagBits::eDeviceLocal);
  indexBuffer = TS_VmaCreateBuffer(defaultBufferSize, vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer,
                                    vk::MemoryPropertyFlagBits::eDeviceLocal);
}

void TS_VkCreateSwapchain()
{
  surfaceCapabilities = pdev.getSurfaceCapabilitiesKHR(srf);
  std::vector<vk::SurfaceFormatKHR> surfaceFormats = pdev.getSurfaceFormatsKHR(srf);
  surfaceFormat = surfaceFormats[0];
  int width,height = 0;
  SDL_Vulkan_GetDrawableSize(win, &width, &height);
  width = CLAMP(width, surfaceCapabilities.minImageExtent.width, surfaceCapabilities.maxImageExtent.width);
  height = CLAMP(height, surfaceCapabilities.minImageExtent.height, surfaceCapabilities.maxImageExtent.height);
  swapchainSize.width = width;
  swapchainSize.height = height;
  uint32_t imageCount = surfaceCapabilities.minImageCount + 1;
  if (surfaceCapabilities.maxImageCount > 0 && imageCount > surfaceCapabilities.maxImageCount)
  {
    imageCount = surfaceCapabilities.maxImageCount;
  }
  
  vk::SwapchainCreateInfoKHR createInfo;
  createInfo.surface = srf;
  createInfo.minImageCount = surfaceCapabilities.minImageCount;
  createInfo.imageFormat = surfaceFormat.format;
  createInfo.imageColorSpace = surfaceFormat.colorSpace;
  createInfo.imageExtent = swapchainSize;
  createInfo.imageArrayLayers = 1;
  createInfo.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;
  uint32_t queueFamilyIndices[] = {graphicsQueueFamilyIndex, presentQueueFamilyIndex};
  if (graphicsQueueFamilyIndex != presentQueueFamilyIndex)
  {
    createInfo.imageSharingMode = vk::SharingMode::eConcurrent;
    createInfo.queueFamilyIndexCount = 2;
    createInfo.pQueueFamilyIndices = queueFamilyIndices;
  }
  else
  {
    createInfo.imageSharingMode = vk::SharingMode::eExclusive;
  }
  createInfo.preTransform = surfaceCapabilities.currentTransform;
  createInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
  createInfo.presentMode = vk::PresentModeKHR::eFifo;
  createInfo.clipped = VK_TRUE;
  swapchain = dev.createSwapchainKHR(createInfo);
  swapchainImages = dev.getSwapchainImagesKHR(swapchain);
  swapchainImageCount = static_cast<uint32_t>(swapchainImages.size());
}

void TS_VkCreateImageViews()
{
  for (int i = 0; i < swapchainImages.size(); ++i)
  {
    swapchainImageViews.push_back(TS_VkCreateImageView(swapchainImages[i], surfaceFormat.format, vk::ImageAspectFlagBits::eColor));
  }
}

void TS_VkSetupDepthStencil()
{
  TS_VkGetSupportedDepthFormat();
  depthImage = TS_VmaCreateImage(swapchainSize.width, swapchainSize.height,
                  vk::Format::eD32SfloatS8Uint, vk::ImageTiling::eOptimal,
                  vk::ImageUsageFlagBits::eDepthStencilAttachment, vk::MemoryPropertyFlagBits::eDeviceLocal);
  
  depthImageView = TS_VkCreateImageView(depthImage.first, vk::Format::eD32SfloatS8Uint, vk::ImageAspectFlagBits::eDepth);
}

void TS_VkCreateRenderPass()
{
  std::vector<vk::AttachmentDescription> attachments(1);
  // std::vector<vk::AttachmentDescription> attachments(2);

  attachments[0].format = surfaceFormat.format;
  attachments[0].samples = vk::SampleCountFlagBits::e1;
  attachments[0].loadOp = vk::AttachmentLoadOp::eClear;
  attachments[0].storeOp = vk::AttachmentStoreOp::eStore;
  attachments[0].stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
  attachments[0].stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
  attachments[0].initialLayout = vk::ImageLayout::eUndefined;
  attachments[0].finalLayout = vk::ImageLayout::ePresentSrcKHR;

  // attachments[1].format = depthFormat;
  // attachments[1].samples = vk::SampleCountFlagBits::e1;
  // attachments[1].loadOp = vk::AttachmentLoadOp::eClear;
  // attachments[1].storeOp = vk::AttachmentStoreOp::eStore;
  // attachments[1].stencilLoadOp = vk::AttachmentLoadOp::eClear;
  // attachments[1].stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
  // attachments[1].initialLayout = vk::ImageLayout::eUndefined;
  // attachments[1].finalLayout = vk::ImageLayout::eDepthStencilAttachmentOptimal;

  vk::AttachmentReference colorReference {
    0, vk::ImageLayout::eColorAttachmentOptimal
  };

  // vk::AttachmentReference depthReference {
  //   1, vk::ImageLayout::eDepthStencilAttachmentOptimal
  // };

  vk::SubpassDescription subpassDescription;
  subpassDescription.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
  subpassDescription.colorAttachmentCount = 1;
  subpassDescription.pColorAttachments = &colorReference;
  // subpassDescription.pDepthStencilAttachment = &depthReference;
  subpassDescription.inputAttachmentCount = 0;
  subpassDescription.pInputAttachments = nullptr;
  subpassDescription.preserveAttachmentCount = 0;
  subpassDescription.pPreserveAttachments = nullptr;
  subpassDescription.pResolveAttachments = nullptr;

  std::vector<vk::SubpassDependency> dependencies(1);

  dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
  dependencies[0].dstSubpass = 0;
  dependencies[0].srcStageMask = vk::PipelineStageFlagBits::eBottomOfPipe;
  dependencies[0].dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
  dependencies[0].srcAccessMask = vk::AccessFlagBits::eMemoryRead;
  dependencies[0].dstAccessMask = vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentRead;
  dependencies[0].dependencyFlags = vk::DependencyFlagBits::eByRegion;

  vk::RenderPassCreateInfo renderPassInfo;
  renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
  renderPassInfo.pAttachments = attachments.data();
  renderPassInfo.subpassCount = 1;
  renderPassInfo.pSubpasses = &subpassDescription;
  renderPassInfo.dependencyCount = static_cast<uint32_t>(dependencies.size());
  renderPassInfo.pDependencies = dependencies.data();

  rp = dev.createRenderPass(renderPassInfo);
}

vk::ShaderModule TS_VkCreateShaderModule(std::string code, shaderc_shader_kind kind, bool optimize = false)
{
  shaderc::Compiler compiler;
  shaderc::CompileOptions options;
  
  if (optimize) options.SetOptimizationLevel(shaderc_optimization_level_performance);
  
  shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(
                                code, kind, "shader_src", options);
  
  if (module.GetCompilationStatus() != shaderc_compilation_status_success) {
    std::cerr << module.GetErrorMessage();
    return nullptr;
  }

  std::vector<uint32_t> spv = {module.cbegin(), module.cend()};
  
  vk::ShaderModuleCreateInfo createInfo;
  createInfo.codeSize = spv.size() * sizeof(uint32_t);
  createInfo.pCode = reinterpret_cast<const uint32_t*>(spv.data());

  return dev.createShaderModule(createInfo);
}

void TS_VkCreateTrianglePipeline()
{
  std::string vertShaderCode = R"""(
    #version 450

    layout(location = 0) in vec2 inPosition;
    layout(location = 1) in vec3 inColor;

    layout(location = 0) out vec3 fragColor;

    void main() {
        gl_Position = vec4(inPosition, 0.0, 1.0);
        fragColor = inColor;
    }
  )""";
  std::string fragShaderCode = R"""(
    #version 450

    layout(location = 0) in vec3 fragColor;

    layout(location = 0) out vec4 outColor;

    void main() {
        outColor = vec4(fragColor, 1.0);
    }
  )""";

  vk::ShaderModule vertShaderModule = TS_VkCreateShaderModule(vertShaderCode, shaderc_glsl_vertex_shader, true);
  vk::ShaderModule fragShaderModule = TS_VkCreateShaderModule(fragShaderCode, shaderc_glsl_fragment_shader, true);

  vk::PipelineShaderStageCreateInfo vertShaderStageInfo;
  vertShaderStageInfo.stage = vk::ShaderStageFlagBits::eVertex;
  vertShaderStageInfo.module = vertShaderModule;
  vertShaderStageInfo.pName = "main";
  
  vk::PipelineShaderStageCreateInfo fragShaderStageInfo;
  fragShaderStageInfo.stage = vk::ShaderStageFlagBits::eFragment;
  fragShaderStageInfo.module = fragShaderModule;
  fragShaderStageInfo.pName = "main";

  vk::PipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};

  vk::PipelineVertexInputStateCreateInfo vertexInputInfo;
  auto bindingDescription = Vertex::getBindingDescription();
  auto attributeDescriptions = Vertex::getAttributeDescriptions();
  vertexInputInfo.vertexBindingDescriptionCount = 1;
  vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
  vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
  vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

  vk::PipelineInputAssemblyStateCreateInfo inputAssembly;
  inputAssembly.topology = vk::PrimitiveTopology::eTriangleFan;
  inputAssembly.primitiveRestartEnable = true;

  vk::Viewport viewport;
  viewport.x = 0.0f;
  viewport.y = 0.0f;
  viewport.width = (float) swapchainSize.width;
  viewport.height = (float) swapchainSize.height;
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;

  vk::Rect2D scissor;
  scissor.offset = vk::Offset2D();
  scissor.extent = swapchainSize;

  vk::PipelineViewportStateCreateInfo viewportState;
  viewportState.viewportCount = 1;
  viewportState.pViewports = &viewport;
  viewportState.scissorCount = 1;
  viewportState.pScissors = &scissor;

  vk::PipelineRasterizationStateCreateInfo rasterizer;
  rasterizer.depthClampEnable = false;
  rasterizer.rasterizerDiscardEnable = false;
  rasterizer.polygonMode = vk::PolygonMode::eFill;
  rasterizer.lineWidth = 1.0f;
  rasterizer.cullMode = vk::CullModeFlagBits::eBack;
  rasterizer.frontFace = vk::FrontFace::eClockwise;
  rasterizer.depthBiasEnable = false;

  vk::PipelineMultisampleStateCreateInfo multisampling;
  multisampling.sampleShadingEnable = false;
  multisampling.rasterizationSamples = vk::SampleCountFlagBits::e1;

  vk::PipelineColorBlendAttachmentState colorBlendAttachment;
  colorBlendAttachment.blendEnable = false;
  colorBlendAttachment.colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;

  vk::PipelineColorBlendStateCreateInfo colorBlending;
  colorBlending.logicOpEnable = false;
  colorBlending.logicOp = vk::LogicOp::eCopy;
  colorBlending.attachmentCount = 1;
  colorBlending.pAttachments = &colorBlendAttachment;
  colorBlending.blendConstants[0] = 0.0f;
  colorBlending.blendConstants[1] = 0.0f;
  colorBlending.blendConstants[2] = 0.0f;
  colorBlending.blendConstants[3] = 0.0f;

  vk::PipelineLayoutCreateInfo pipelineLayoutInfo;
  pipelineLayoutInfo.setLayoutCount = 0;
  pipelineLayoutInfo.pushConstantRangeCount = 0;

  trianglePipelineLayout = dev.createPipelineLayout(pipelineLayoutInfo);

  vk::GraphicsPipelineCreateInfo pipelineInfo;
  pipelineInfo.stageCount = 2;
  pipelineInfo.pStages = shaderStages;
  pipelineInfo.pVertexInputState = &vertexInputInfo;
  pipelineInfo.pInputAssemblyState = &inputAssembly;
  pipelineInfo.pViewportState = &viewportState;
  pipelineInfo.pRasterizationState = &rasterizer;
  pipelineInfo.pMultisampleState = &multisampling;
  pipelineInfo.pColorBlendState = &colorBlending;
  pipelineInfo.layout = trianglePipelineLayout;
  pipelineInfo.renderPass = rp;
  pipelineInfo.subpass = 0;

  trianglePipeline = dev.createGraphicsPipeline(VK_NULL_HANDLE, pipelineInfo).value;

  dev.destroyShaderModule(fragShaderModule);
  dev.destroyShaderModule(vertShaderModule);
}

void TS_VkCreateFramebuffers()
{
  for (size_t i = 0; i < swapchainImageViews.size(); ++i)
  {
    std::vector<vk::ImageView> attachments {
      swapchainImageViews[i],
      // depthImageView
    };
    
    vk::FramebufferCreateInfo framebufferInfo {
      vk::FramebufferCreateFlags(),
      rp, attachments, swapchainSize.width, swapchainSize.height, 1
    };
    
    swapchainFramebuffers.push_back(dev.createFramebuffer(framebufferInfo));
  }
}

void TS_VkCreateCommandPool()
{
  cp = dev.createCommandPool({
    vk::CommandPoolCreateFlagBits::eResetCommandBuffer | vk::CommandPoolCreateFlagBits::eTransient,
    graphicsQueueFamilyIndex
  });
}

void TS_VkAllocateCommandBuffers()
{
  cmdbufs = dev.allocateCommandBuffers({cp, vk::CommandBufferLevel::ePrimary, swapchainImageCount});
}

void TS_VkCreateSemaphores()
{
  imageAvailableSemaphore = dev.createSemaphore({});
  renderingFinishedSemaphore = dev.createSemaphore({});
}

void TS_VkCreateFences()
{
  for (uint32_t i = 0; i < swapchainImageCount; ++i)
  {
    fences.push_back(dev.createFence({vk::FenceCreateFlagBits::eSignaled}));
  }
}

void TS_VkInit()
{
  TS_VkCreateInstance();
  TS_VkCreateDebugMessenger();
  TS_VkCreateSurface();
  TS_VkSelectPhysicalDevice();
  TS_VkSelectQueueFamily();
  TS_VkCreateDevice();
  TS_VmaCreateAllocator();
  TS_VmaCreateBuffers();
  TS_VkCreateSwapchain();
  TS_VkCreateImageViews();
  TS_VkSetupDepthStencil();
  TS_VkCreateRenderPass();
  TS_VkCreateTrianglePipeline();
  TS_VkCreateFramebuffers();
  TS_VkCreateCommandPool();
  TS_VkAllocateCommandBuffers();
  TS_VkCreateSemaphores();
  TS_VkCreateFences();
}

void TS_VkDestroyFences()
{
  for (int i = 0; i < swapchainImageCount; ++i)
  {
    dev.destroyFence(fences[i]);
  }
  fences.clear();
}

void TS_VkDestroySemaphores()
{
  dev.destroySemaphore(imageAvailableSemaphore);
  dev.destroySemaphore(renderingFinishedSemaphore);
}

void TS_VkFreeCommandBuffers()
{
  dev.freeCommandBuffers(cp, cmdbufs);
  cmdbufs.clear();
}

void TS_VkDestroyCommandPool()
{
  dev.destroyCommandPool(cp);
}

void TS_VkDestroyFramebuffers()
{
  for (int i = 0; i < swapchainFramebuffers.size(); ++i)
  {
    dev.destroyFramebuffer(swapchainFramebuffers[i]);
  }
  swapchainFramebuffers.clear();
}

void TS_VkDestroyTrianglePipeline()
{
  dev.destroy(trianglePipeline);
  dev.destroy(trianglePipelineLayout);
}

void TS_VkDestroyRenderPass()
{
  dev.destroyRenderPass(rp);
}

void TS_VkTeardownDepthStencil()
{
  dev.destroyImageView(depthImageView);
  al.destroyImage(depthImage.first, depthImage.second);
}

void TS_VkDestroyImageViews()
{
  for (vk::ImageView iv : swapchainImageViews)
  {
    dev.destroyImageView(iv);
  }
  swapchainImageViews.clear();
}

void TS_VkDestroySwapchain()
{
  dev.destroySwapchainKHR(swapchain);
}

void TS_VmaDestroyBuffers()
{
  al.destroyBuffer(vertexStaging.first, vertexStaging.second);
  al.destroyBuffer(indexStaging.first, indexStaging.second);
  al.destroyBuffer(vertexBuffer.first, vertexBuffer.second);
  al.destroyBuffer(indexBuffer.first, indexBuffer.second);
}

void TS_VmaDestroyAllocator()
{
  al.destroy();
}

void TS_VkDestroyDevice()
{
  graphicsQueueFamilyIndex = -1;
  presentQueueFamilyIndex = -1;
  dev.destroy();
}

void TS_VkDestroySurface()
{
  inst.destroySurfaceKHR(srf);
}

void TS_VkDestroyDebugMessenger()
{
  if (!enableValidationLayers) return;
  inst.destroy(dbm);
}

void TS_VkDestroyInstance()
{
  inst.destroy();
}

void TS_VkQuit()
{
  TS_VkDestroyFences();
  TS_VkDestroySemaphores();
  TS_VkFreeCommandBuffers();
  TS_VkDestroyCommandPool();
  TS_VkDestroyFramebuffers();
  TS_VkDestroyTrianglePipeline();
  TS_VkDestroyRenderPass();
  TS_VkTeardownDepthStencil();
  TS_VkDestroyImageViews();
  TS_VkDestroySwapchain();
  TS_VmaDestroyBuffers();
  TS_VmaDestroyAllocator();
  TS_VkDestroyDevice();
  TS_VkDestroySurface();
  TS_VkDestroyDebugMessenger();
  TS_VkDestroyInstance();
}

void TS_Init(const char * ttl, int wdth, int hght)
{ 
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
  {
    std::cerr << "Unable to initialize SDL: " << TS_GetSDLError() << std::endl;
  }

  int mix_init_flags = MIX_INIT_FLAC | MIX_INIT_MP3 | MIX_INIT_OGG;
  if ((Mix_Init(mix_init_flags) & mix_init_flags) != mix_init_flags)
  {
    std::cerr << "Failed to initialise audio mixer properly. All sounds may not play correctly." << std::endl << TS_GetSDLError() << std::endl; 
  }

  if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024) != 0)
  {
    std::cerr << "No audio device available, sounds and music will not play." << std::endl << TS_GetSDLError() << std::endl;
    Mix_CloseAudio();
  }

  window_name = ttl;
  win = SDL_CreateWindow(ttl, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, wdth, hght, SDL_WINDOW_VULKAN|SDL_WINDOW_ALLOW_HIGHDPI|SDL_WINDOW_SHOWN);
  if (win == NULL)
  {
    std::cerr << "Failed to create window: " << TS_GetSDLError() << std::endl;
  }
  else
  {
    SDL_SetWindowMinimumSize(win, wdth, hght);
  }
  
  TS_VkInit();
}

void TS_Quit()
{
  TS_VkQuit();
  SDL_DestroyWindow(win);

  Mix_HaltMusic();
  Mix_HaltChannel(-1);
  Mix_CloseAudio();

  Mix_Quit();
  SDL_Quit();
}

void TS_VkBeginDrawPass()
{
  TS_VkAcquireNextImage();
  TS_VkResetCommandBuffer();
  TS_VkBeginCommandBuffer();

  // clear all buffers
  memset(al.getAllocationInfo(vertexStaging.second).pMappedData, 0, defaultBufferSize);
  memset(al.getAllocationInfo(indexStaging.second).pMappedData, 0, defaultBufferSize);
  cmdbufs[frameIndex].fillBuffer(vertexBuffer.first, 0, VK_WHOLE_SIZE, 0);
  cmdbufs[frameIndex].fillBuffer(indexBuffer.first, 0, VK_WHOLE_SIZE, 0);
  indices.clear();
}

void TS_VkEndDrawPass(float r, float g, float b, float a)
{
  TS_VkDraw(r, g, b, a);
  TS_VkEndCommandBuffer();
  TS_VkQueueSubmit();
  TS_VkQueuePresent();
}

void TS_PlaySound(const char* sound_file, int loops=0, int ticks=-1)
{
  Mix_Chunk *sample = Mix_LoadWAV_RW(SDL_RWFromFile(sound_file, "rb"), 1);
  if (sample == NULL)
  {
    std::cerr << "Could not load sound file: " << std::string(sound_file) << std::endl << TS_GetSDLError() << std::endl;
    return;
  }
  if (Mix_PlayChannelTimed(-1, sample, loops, ticks) == -1)
  {
    std::cerr << "Unable to play sound " << sound_file << std::endl << TS_GetSDLError() << std::endl;
  }
}