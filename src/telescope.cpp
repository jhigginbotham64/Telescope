/*
  TODO: "can't find SDL_mixer.h"
  TODO: "can't convert between VulkanHPP and normal Vulkan types"
  TODO: better support for rotation of rectangles, sprites, and collision objects
*/

#define VULKAN_HPP_TYPESAFE_CONVERSION 1
#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1
#define VULKAN_HPP_STORAGE_SHARED 1
#define VULKAN_HPP_STORAGE_SHARED_EXPORT 1
#include <vulkan/vulkan.hpp>
VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

#define VMA_IMPLEMENTATION
#define VMA_STATIC_VULKAN_FUNCTIONS 0
#define VMA_DYNAMIC_VULKAN_FUNCTIONS 1
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

#define CLAMP(x, lo, hi)    ((x) < (lo) ? (lo) : (x) > (hi) ? (hi) : (x))

const char *window_name = NULL;
SDL_Window *win = NULL;
int window_width = 0;
int window_height = 0;
#define SDL_MAX_QUEUED_EVENTS 65535
std::string events;
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

struct TS_Texture {
  std::pair<vk::Image, vma::Allocation> img;
  vk::ImageView view;

  uint32_t width;
  uint32_t height;

  std::string fname;
};

vk::Sampler smp;
vk::DescriptorPool dscPool;
vk::DescriptorSet dscSet;
vk::DescriptorSetLayout dscSetLayout;

#define NUM_SUPPORTED_TEXTURES 80
std::queue<int> availableInds;
std::map<std::string, int> txtInds;
std::array<TS_Texture, NUM_SUPPORTED_TEXTURES> txts;
std::array<vk::DescriptorImageInfo, NUM_SUPPORTED_TEXTURES> dscImgInfos;

struct TS_Vertex {
  glm::vec2 pos;
  glm::vec2 uv;
  glm::vec4 col;
  int tex;

  TS_Vertex(float x, float y, float r, float g, float b, float a, float u = 0, float v = 0, int t = -1)
  {
    this->pos = glm::vec2(x, y);
    this->uv = glm::vec2(u, v);
    this->col = glm::vec4(r, g, b, a);
    this->tex = t;
  }

  static vk::VertexInputBindingDescription getBindingDescription()
  {
    vk::VertexInputBindingDescription bindingDescription;
    bindingDescription.binding = 0;
    bindingDescription.stride = sizeof(TS_Vertex);
    bindingDescription.inputRate = vk::VertexInputRate::eVertex;

    return bindingDescription;
  }

  static std::array<vk::VertexInputAttributeDescription, 4> getAttributeDescriptions()
  {
    std::array<vk::VertexInputAttributeDescription, 4> attributeDescriptions;

    attributeDescriptions[0].binding = 0;
    attributeDescriptions[0].location = 0;
    attributeDescriptions[0].format = vk::Format::eR32G32Sfloat;
    attributeDescriptions[0].offset = offsetof(TS_Vertex, pos);

    attributeDescriptions[1].binding = 0;
    attributeDescriptions[1].location = 1;
    attributeDescriptions[1].format = vk::Format::eR32G32Sfloat;
    attributeDescriptions[1].offset = offsetof(TS_Vertex, uv);

    attributeDescriptions[2].binding = 0;
    attributeDescriptions[2].location = 2;
    attributeDescriptions[2].format = vk::Format::eR32G32B32A32Sfloat;
    attributeDescriptions[2].offset = offsetof(TS_Vertex, col);

    attributeDescriptions[3].binding = 0;
    attributeDescriptions[3].location = 3;
    attributeDescriptions[3].format = vk::Format::eR32Sint;
    attributeDescriptions[3].offset = offsetof(TS_Vertex, tex);

    return attributeDescriptions;
  }
};

std::vector<TS_Vertex> vertices;
std::vector<uint32_t> indices;
uint32_t current_index = 0;
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

btDbvtBroadphase btbpi;
btDefaultCollisionConfiguration btcc;
btCollisionDispatcher btcd(&btcc);
btSequentialImpulseConstraintSolver btcs;
btDiscreteDynamicsWorld btdw(&btcd, &btbpi, &btcs, &btcc);

struct TS_PhysicsObject {
  btCollisionObject * cobj;
  btCollisionShape * cshape;
  btRigidBody * rbody;
  btDefaultMotionState * dmstate;

  TS_PhysicsObject(btCollisionShape * s, float mass = 0.0f, bool isKinematic = false, bool isTrigger = false, const btVector3 &initPos = btVector3(0,0,0), const btQuaternion &initRot = btQuaternion(0,0,1,1))
  {
    this->cshape = s;
    this->cobj = nullptr;
    this->rbody = nullptr;
    this->dmstate = nullptr;

    btTransform t;
    t.setIdentity();
    t.setOrigin(initPos);
    t.setRotation(initRot);

    btVector3 locInertia(0,0,0);

    if (mass != 0.0f)
      this->cshape->calculateLocalInertia(mass, locInertia);

    this->dmstate = new btDefaultMotionState(t);
    
    btRigidBody::btRigidBodyConstructionInfo cinfo(mass, this->dmstate, this->cshape, locInertia);

    this->rbody = new btRigidBody(cinfo);

    this->cobj = this->rbody;

    if (isTrigger)
      this->cobj->setCollisionFlags(this->cobj->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);

    if (isKinematic || isTrigger)
    {
      // this->cobj->setCollisionFlags(this->cobj->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
      this->cobj->setActivationState(DISABLE_DEACTIVATION);
    }

    btdw.addRigidBody(this->rbody);
  }

  ~TS_PhysicsObject()
  {
    if (this->rbody)
    { 
      btdw.removeRigidBody(this->rbody);
      delete this->rbody;
    }
    if (this->dmstate) delete this->dmstate;
    if (this->cobj && this->cobj != this->rbody) 
    {
      btdw.removeCollisionObject(this->cobj);
      delete this->cobj;
    }
    if (this->cshape) delete this->cshape;
  }

  btTransform getTransform()
  {
    btTransform t;
    this->dmstate->getWorldTransform(t);
    return t;
  }
};

std::map<int, TS_PhysicsObject*> physicsObjectsById;
std::map<const void*, int> idsByPtr;

// convenient typedefs for collision events
typedef std::pair<const void*, const void*> CollisionPair;
typedef std::set<CollisionPair> CollisionPairs;
CollisionPairs oldPairs;
std::queue<TS_CollisionEvent> collisions;

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

const char * TS_SDLGetError()
{
  return SDL_GetError();
}

// normalized device coordinates along the x and y axes
float TS_NDCX(float x)
{
 return (2.0f / window_width) * x - 1.0f;
}

float TS_NDCY(float y)
{
  return (2.0f / window_height) * y - 1.0f;
}

std::array<float, 4> TS_NDCRect(float x, float y, float w, float h)
{
  return std::array<float, 4>({TS_NDCX(x), TS_NDCX(x + w), TS_NDCY(y), TS_NDCY(y + h)});
}

// normalized texture coordinates along the u and v axes
float TS_NTCU(int x, int w)
{
 return (1.0f / w) * x;
}

float TS_NTCV(int y, int h)
{
  return (1.0f / h) * y;
}

std::array<float, 4> TS_NTCRect(int x, int y, int w, int h, int w2, int h2)
{
  return std::array<float, 4>({TS_NTCU(x, w2), TS_NTCU(x + w, w2), TS_NTCV(y, h2), TS_NTCV(y + h, h2)});
}

void TS_Add4Indices()
{
  indices.push_back(current_index);
  indices.push_back(current_index + 1);
  indices.push_back(current_index + 2);
  indices.push_back(current_index + 3);
  indices.push_back(0xffffffff); // primitive restart
  current_index += 4;
}

vk::CommandBuffer TS_VkBeginScratchBuffer()
{
  vk::CommandBufferAllocateInfo allocInfo;
  allocInfo.level = vk::CommandBufferLevel::ePrimary;
  allocInfo.commandPool = cp;
  allocInfo.commandBufferCount = 1;

  vk::CommandBuffer tmp = dev.allocateCommandBuffers(allocInfo)[0];

  vk::CommandBufferBeginInfo beginInfo;
  beginInfo.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;

  tmp.begin(beginInfo);

  return tmp;
}

void TS_VkSubmitScratchBuffer(vk::CommandBuffer &tmp)
{
  tmp.end();

  vk::SubmitInfo submitInfo;
  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &tmp;

  gq.submit(1, &submitInfo, vk::Fence());
  gq.waitIdle();

  dev.freeCommandBuffers(cp, 1, &tmp);
}

void TS_VkTransitionImageLayout(vk::Image img, vk::ImageLayout oldLayout, vk::ImageLayout newLayout)
{
  vk::CommandBuffer tmp = TS_VkBeginScratchBuffer();

  vk::ImageMemoryBarrier barrier;
  barrier.oldLayout = oldLayout;
  barrier.newLayout = newLayout;
  barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  barrier.image = img;
  barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
  barrier.subresourceRange.baseMipLevel = 0;
  barrier.subresourceRange.levelCount = 1;
  barrier.subresourceRange.baseArrayLayer = 0;
  barrier.subresourceRange.layerCount = 1;

  vk::PipelineStageFlags srcStage;
  vk::PipelineStageFlags dstStage;

  if (oldLayout == vk::ImageLayout::eUndefined && newLayout == vk::ImageLayout::eTransferDstOptimal)
  {
    barrier.dstAccessMask = vk::AccessFlagBits::eTransferWrite;
    srcStage = vk::PipelineStageFlagBits::eTopOfPipe;
    dstStage = vk::PipelineStageFlagBits::eTransfer;
  }
  else if (oldLayout == vk::ImageLayout::eTransferDstOptimal && newLayout == vk::ImageLayout::eShaderReadOnlyOptimal)
  {
    barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
    barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;
    srcStage = vk::PipelineStageFlagBits::eTransfer;
    dstStage = vk::PipelineStageFlagBits::eFragmentShader;
  }
  else
  {
    // unsupported layout transition
    throw std::runtime_error("Attempting an unsupported image layout transition");
  }

  tmp.pipelineBarrier(srcStage, dstStage, vk::DependencyFlags(), 0, nullptr, 0, nullptr, 1, &barrier);

  TS_VkSubmitScratchBuffer(tmp);
}

void TS_VkCopyBufferToImage(vk::Buffer buf, vk::Image img, uint32_t wdth, uint32_t hght)
{
  vk::CommandBuffer tmp = TS_VkBeginScratchBuffer();

  vk::BufferImageCopy region;
  region.bufferOffset = 0;
  region.bufferRowLength = 0;
  region.bufferImageHeight = 0;

  region.imageSubresource.aspectMask = vk::ImageAspectFlagBits::eColor;
  region.imageSubresource.mipLevel = 0;
  region.imageSubresource.baseArrayLayer = 0;
  region.imageSubresource.layerCount = 1;

  region.imageOffset = vk::Offset3D(0,0,0);
  region.imageExtent = vk::Extent3D(wdth,hght,1);

  tmp.copyBufferToImage(buf, img, vk::ImageLayout::eTransferDstOptimal, 1, &region);

  TS_VkSubmitScratchBuffer(tmp);
}

void TS_VkWriteDescriptorSet()
{
  vk::WriteDescriptorSet setWrites[2];

  vk::DescriptorImageInfo samplerInfo;
  samplerInfo.sampler = smp;

  setWrites[0].dstBinding = 0;
  setWrites[0].dstArrayElement = 0;
	setWrites[0].descriptorType = vk::DescriptorType::eSampler;
	setWrites[0].descriptorCount = 1;
	setWrites[0].dstSet = dscSet;
	setWrites[0].pBufferInfo = 0;
	setWrites[0].pImageInfo = &samplerInfo;

	setWrites[1].dstBinding = 1;
	setWrites[1].dstArrayElement = 0;
	setWrites[1].descriptorType = vk::DescriptorType::eSampledImage;
	setWrites[1].descriptorCount = NUM_SUPPORTED_TEXTURES;
	setWrites[1].pBufferInfo = 0;
	setWrites[1].dstSet = dscSet;
	setWrites[1].pImageInfo = dscImgInfos.data();

  dev.updateDescriptorSets(2, setWrites, 0, nullptr);
}

int TS_VkLoadTexture(const char * img)
{
  // initialize on first access
  if (txtInds.empty() && availableInds.empty())
  {
    for (int i = 0; i < NUM_SUPPORTED_TEXTURES; ++i)
    {
      availableInds.push(i);
    }
  }
  
  // max textures allocated
  if (availableInds.empty())
  {
    return -1;
  }
  
  // key not present means texture not loaded yet
  if (!txtInds.count(std::string(img)))
  {
    int txtInd = availableInds.front();

    SDL_Surface *srf = IMG_Load(img);
    int wdth = srf->w;
    int hght = srf->h;

    std::vector<uint8_t> pixels;
    SDL_PixelFormat * fmt = srf->format;

    SDL_LockSurface(srf);
    for (int i = 0; i < wdth * hght; ++i)
    {
      // https://wiki.libsdl.org/SDL_PixelFormat
      if (fmt->BitsPerPixel != 8)
      {
        uint32_t temp, pixel;
        uint8_t r, g, b, a;
        pixel = *((uint32_t*)(((char*)srf->pixels) + i * fmt->BytesPerPixel));
        
        temp = pixel & fmt->Rmask;
        temp = temp >> fmt->Rshift;
        temp = temp << fmt->Rloss;
        r = (uint8_t)temp;

        temp = pixel & fmt->Gmask;
        temp = temp >> fmt->Gshift;
        temp = temp << fmt->Gloss;
        g = (uint8_t)temp;

        temp = pixel & fmt->Bmask;
        temp = temp >> fmt->Bshift;
        temp = temp << fmt->Bloss;
        b = (uint8_t)temp;

        temp = pixel & fmt->Amask;
        temp = temp >> fmt->Ashift;
        temp = temp << fmt->Aloss;
        a = (uint8_t)temp;

        pixels.push_back(r);
        pixels.push_back(g);
        pixels.push_back(b);
        pixels.push_back(255); // TODO fix this
      }
      else
      {
        SDL_Color col = fmt->palette->colors[*(uint8_t *) (((char*)srf->pixels) + i)];
        pixels.push_back(col.r);
        pixels.push_back(col.g);
        pixels.push_back(col.b);
        pixels.push_back(col.a);
      }
    }
    SDL_UnlockSurface(srf);
    SDL_FreeSurface(srf);

    std::pair<vk::Buffer, vma::Allocation> pixelStaging = TS_VmaCreateBuffer(pixels.size() * sizeof(uint8_t), vk::BufferUsageFlagBits::eTransferSrc, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, vma::AllocationCreateFlagBits::eMapped);    
    memcpy(al.getAllocationInfo(pixelStaging.second).pMappedData, (void*)pixels.data(), pixels.size() * sizeof(uint8_t));
    std::pair<vk::Image, vma::Allocation> pixelImg = TS_VmaCreateImage(wdth, hght, vk::Format::eR8G8B8A8Unorm, vk::ImageTiling::eOptimal, vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled, vk::MemoryPropertyFlagBits::eDeviceLocal);
    TS_VkTransitionImageLayout(pixelImg.first, vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal);
    TS_VkCopyBufferToImage(pixelStaging.first, pixelImg.first, wdth, hght);
    TS_VkTransitionImageLayout(pixelImg.first, vk::ImageLayout::eTransferDstOptimal, vk::ImageLayout::eShaderReadOnlyOptimal);
    vk::ImageView v = TS_VkCreateImageView(pixelImg.first, vk::Format::eR8G8B8A8Unorm, vk::ImageAspectFlagBits::eColor);
    al.destroyBuffer(pixelStaging.first, pixelStaging.second);

    txts[txtInd] = TS_Texture();
    txts[txtInd].img = pixelImg;
    txts[txtInd].fname = std::string(img);
    txts[txtInd].view = v;
    txts[txtInd].width = wdth;
    txts[txtInd].height = hght;
    
    dscImgInfos[txtInd] = vk::DescriptorImageInfo();
    dscImgInfos[txtInd].sampler = nullptr;
    dscImgInfos[txtInd].imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
    dscImgInfos[txtInd].imageView = v;

    txtInds[std::string(img)] = txtInd;
    availableInds.pop();

    TS_VkWriteDescriptorSet();
  }

  return txtInds[std::string(img)];
}

void TS_VkUnloadTexture(const char * img)
{
  // retrieve index from map
  int ind = txtInds[std::string(img)];

  // remove from map
  txtInds.erase(std::string(img));

  // reset index in txts
  txts[ind] = TS_Texture();

  // reset index in dscImgInfos
  dscImgInfos[ind] = vk::DescriptorImageInfo();

  // return index to queue
  availableInds.push(ind);

  // update descriptor set
  TS_VkWriteDescriptorSet();
}

void TS_VkCmdDrawRect(float r, float g, float b, float a, float x, float y, float w, float h)
{
  // convert from screen space to normalized device coordinates
  std::array<float, 4> ndc = TS_NDCRect(x, y, w, h);

  // update vertices
  vertices.push_back(TS_Vertex(ndc[1], ndc[3], r, g, b, a));
  vertices.push_back(TS_Vertex(ndc[0], ndc[3], r, g, b, a));
  vertices.push_back(TS_Vertex(ndc[0], ndc[2], r, g, b, a));
  vertices.push_back(TS_Vertex(ndc[1], ndc[2], r, g, b, a));

  // update indices
  TS_Add4Indices();
}

void TS_VkCmdDrawSprite(const char * img, float r, float g, float b, float a, int rx, int ry, int rw, int rh, int cw, int ch, int ci, int cj, float px, float py, float sx, float sy)
{
  int txtInd = TS_VkLoadTexture(img);

  TS_Texture txt = txts[txtInd];

  uint32_t w = txt.width;
  uint32_t h = txt.height;

  uint32_t srcw, srch, dstw, dsth, srctlx, srctly;

  // using a specific region
  if (rw != 0 && rh != 0)
  {
    srctlx = rx;
    srctly = ry;
    srcw = rw;
    srch = rh;
    dstw = rw;
    dsth = rh;
  }
  // divide texture into cells and use specific cell
  else if (cw != 0 && ch != 0)
  {
    srctlx = cj * cw;
    srctly = ci * ch;
    srcw = cw;
    srch = ch;
    dstw = cw;
    dsth = ch;
  }
  // use whole texture
  else
  {
    srctlx = 0;
    srctly = 0;
    srcw = w;
    srch = h;
    dstw = w;
    dsth = h;
  }

  // apply scaling
  dstw = floor(dstw * sx);
  dsth = floor(dsth * sy);

  // normalized device coordinates
  std::array<float, 4> ndc = TS_NDCRect(px, py, dstw, dsth);

  // normalized texture coordinates
  std::array<float, 4> ntc = TS_NTCRect(srctlx, srctly, srcw, srch, w, h);

  // update vertices
  vertices.push_back(TS_Vertex(ndc[1], ndc[3], r, g, b, a, ntc[1], ntc[3], txtInd));
  vertices.push_back(TS_Vertex(ndc[0], ndc[3], r, g, b, a, ntc[0], ntc[3], txtInd));
  vertices.push_back(TS_Vertex(ndc[0], ndc[2], r, g, b, a, ntc[0], ntc[2], txtInd));
  vertices.push_back(TS_Vertex(ndc[1], ndc[2], r, g, b, a, ntc[1], ntc[2], txtInd));

  // update indices
  TS_Add4Indices();
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
  // copy data
  memcpy(al.getAllocationInfo(vertexStaging.second).pMappedData, (void*)vertices.data(), vertices.size() * sizeof(TS_Vertex));
  memcpy(al.getAllocationInfo(indexStaging.second).pMappedData, (void*)indices.data(), indices.size() * sizeof(uint32_t));

  // copy buffers
  vk::BufferCopy bfcpy;
  bfcpy.srcOffset = 0;
  bfcpy.dstOffset = 0;
  bfcpy.size = defaultBufferSize;
  cmdbufs[frameIndex].copyBuffer(vertexStaging.first, vertexBuffer.first, 1, &bfcpy);
  cmdbufs[frameIndex].copyBuffer(indexStaging.first, indexBuffer.first, 1, &bfcpy);

  // bind descriptor sets (sampler and textures)
  cmdbufs[frameIndex].bindDescriptorSets(vk::PipelineBindPoint::eGraphics, trianglePipelineLayout, 0, 1, &dscSet, 0, 0);

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
    VK_MAKE_VERSION(0, 1, 2), 
    "Telescope", 
    VK_MAKE_VERSION(0, 1, 2),
    VK_API_VERSION_1_2
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
  const std::vector<const char*> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME, VK_EXT_ROBUSTNESS_2_EXTENSION_NAME, VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME};
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
  deviceFeatures.shaderSampledImageArrayDynamicIndexing = VK_TRUE;
  deviceFeatures.shaderStorageImageArrayDynamicIndexing = VK_TRUE;
  deviceFeatures.shaderStorageBufferArrayDynamicIndexing = VK_TRUE;
  deviceFeatures.shaderUniformBufferArrayDynamicIndexing = VK_TRUE;
  vk::DeviceCreateInfo deviceCreateInfo {
    vk::DeviceCreateFlags(),
    static_cast<uint32_t>(queueCreateInfos.size()), queueCreateInfos.data(),
    0, nullptr,
    static_cast<uint32_t>(deviceExtensions.size()), deviceExtensions.data(),
    &deviceFeatures
  };

  vk::PhysicalDeviceRobustness2FeaturesEXT robustnessFeatures;
  robustnessFeatures.nullDescriptor = VK_TRUE;
  deviceCreateInfo.pNext = &robustnessFeatures;

  vk::PhysicalDeviceVulkan12Features vulkan12Features;
  vulkan12Features.descriptorIndexing = VK_TRUE;
  vulkan12Features.descriptorBindingPartiallyBound = VK_TRUE;
  robustnessFeatures.pNext = &vulkan12Features;

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

void TS_VkCreateDescriptorSet()
{
  vk::SamplerCreateInfo samplerInfo;
  samplerInfo.magFilter = vk::Filter::eNearest;
  samplerInfo.minFilter = vk::Filter::eNearest;

  samplerInfo.addressModeU = vk::SamplerAddressMode::eMirroredRepeat;
  samplerInfo.addressModeV = vk::SamplerAddressMode::eMirroredRepeat;
  samplerInfo.addressModeW = vk::SamplerAddressMode::eMirroredRepeat;

  samplerInfo.anisotropyEnable = VK_FALSE;
  samplerInfo.maxAnisotropy = 0.0f;
  samplerInfo.borderColor = vk::BorderColor::eIntOpaqueBlack;
  samplerInfo.unnormalizedCoordinates = VK_FALSE;

  samplerInfo.compareEnable = VK_FALSE;
  samplerInfo.compareOp = vk::CompareOp::eAlways;

  samplerInfo.mipmapMode = vk::SamplerMipmapMode::eNearest;
  samplerInfo.mipLodBias = 0.0f;
  samplerInfo.minLod = 0.0f;
  samplerInfo.maxLod = 0.0f;

  smp = dev.createSampler(samplerInfo);

  vk::DescriptorSetLayoutBinding smpBinding;
  smpBinding.descriptorType = vk::DescriptorType::eSampler;
  smpBinding.stageFlags = vk::ShaderStageFlagBits::eFragment;
  smpBinding.binding = 0;
  smpBinding.descriptorCount = 1;

  vk::DescriptorSetLayoutBinding txtsBinding;
  txtsBinding.descriptorType = vk::DescriptorType::eSampledImage;
  txtsBinding.stageFlags = vk::ShaderStageFlagBits::eFragment;
  txtsBinding.binding = 1;
  txtsBinding.descriptorCount = NUM_SUPPORTED_TEXTURES;

  vk::DescriptorSetLayoutBinding layoutBindings[] = {smpBinding, txtsBinding};

  vk::DescriptorSetLayoutCreateInfo layoutInfo;
  layoutInfo.bindingCount = 2;
  layoutInfo.pBindings = layoutBindings;

  vk::DescriptorBindingFlags layoutBindingFlags[] = {vk::DescriptorBindingFlags(), vk::DescriptorBindingFlagBits::ePartiallyBound};
  vk::DescriptorSetLayoutBindingFlagsCreateInfo layoutFlagsInfo;
  layoutFlagsInfo.bindingCount = 2;
  layoutFlagsInfo.pBindingFlags = layoutBindingFlags;
  layoutInfo.pNext = &layoutFlagsInfo;

  dscSetLayout = dev.createDescriptorSetLayout(layoutInfo);

  vk::DescriptorPoolSize smpPoolSize;
  smpPoolSize.type = vk::DescriptorType::eSampler;
  smpPoolSize.descriptorCount = 1;
  vk::DescriptorPoolSize txtsPoolSize;
  txtsPoolSize.type = vk::DescriptorType::eSampledImage;
  txtsPoolSize.descriptorCount = NUM_SUPPORTED_TEXTURES;
  std::array<vk::DescriptorPoolSize, 2> poolSizes = {smpPoolSize, txtsPoolSize};

  vk::DescriptorPoolCreateInfo poolCreateInfo;
  poolCreateInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
  poolCreateInfo.pPoolSizes = poolSizes.data();
  poolCreateInfo.maxSets = 1 + NUM_SUPPORTED_TEXTURES;
  poolCreateInfo.flags = vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet;
  dscPool = dev.createDescriptorPool(poolCreateInfo);

  vk::DescriptorSetAllocateInfo allocInfo;
  allocInfo.descriptorPool = dscPool;
  allocInfo.descriptorSetCount = 1;
  allocInfo.pSetLayouts = &dscSetLayout;
  dscSet = dev.allocateDescriptorSets(allocInfo)[0];
}

void TS_VkCreateTrianglePipeline()
{
  std::string vertShaderCode = R"""(
    #version 450

    layout(location = 0) in vec2 inPos;
    layout(location = 1) in vec2 inUv;
    layout(location = 2) in vec4 inCol;
    layout(location = 3) in int inTex;

    layout(location = 0) out vec4 fragCol;
    layout(location = 1) out int fragTex;
    layout(location = 2) out vec2 fragUv;

    void main() {
        gl_Position = vec4(inPos, 0.0, 1.0);
        fragCol = inCol;
        fragTex = inTex;
        fragUv = inUv;
    }
  )""";

  std::string fragShaderCode = R"""(
    #version 450

    layout(set = 0, binding = 0) uniform sampler smp;
    layout(set = 0, binding = 1) uniform texture2D txts[80];

    layout(location = 0) in vec4 fragCol;
    layout(location = 1) flat in int fragTex;
    layout(location = 2) in vec2 fragUv;

    layout(location = 0) out vec4 outCol;

    void main() {
        if (fragTex == -1)
            outCol = fragCol;
        else
            outCol = fragCol * texture(sampler2D(txts[fragTex], smp), fragUv);
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
  auto bindingDescription = TS_Vertex::getBindingDescription();
  auto attributeDescriptions = TS_Vertex::getAttributeDescriptions();
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
  colorBlendAttachment.blendEnable = true;
  colorBlendAttachment.colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;
  colorBlendAttachment.srcColorBlendFactor = vk::BlendFactor::eSrcAlpha;
  colorBlendAttachment.dstColorBlendFactor = vk::BlendFactor::eOneMinusSrcAlpha;
  colorBlendAttachment.colorBlendOp = vk::BlendOp::eAdd;
  colorBlendAttachment.srcAlphaBlendFactor = vk::BlendFactor::eSrcAlpha;
  colorBlendAttachment.dstAlphaBlendFactor = vk::BlendFactor::eOneMinusSrcAlpha;
  colorBlendAttachment.alphaBlendOp = vk::BlendOp::eAdd;

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
  pipelineLayoutInfo.setLayoutCount = 1;
  pipelineLayoutInfo.pSetLayouts = &dscSetLayout;
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

  trianglePipeline = dev.createGraphicsPipeline(vk::PipelineCache(), pipelineInfo).value;

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
  TS_VkCreateDescriptorSet();
  TS_VkWriteDescriptorSet();
  TS_VkCreateTrianglePipeline();
  TS_VkCreateFramebuffers();
  TS_VkCreateCommandPool();
  TS_VkAllocateCommandBuffers();
  TS_VkCreateSemaphores();
  TS_VkCreateFences();
}

void TS_BtAddRigidBox(int id, float hx, float hy, float hz, float m, float px, float py, float pz, bool isKinematic = false)
{
  TS_PhysicsObject * g = new TS_PhysicsObject(new btBoxShape(btVector3(hy, hx, hz)), m, isKinematic, false, btVector3(px, py, pz));
  physicsObjectsById[id] = g;
  idsByPtr[static_cast<void*>(g->rbody)] = id;
}

void TS_BtAddStaticBox(int id, float hx, float hy, float hz, float px, float py, float pz)
{
  TS_PhysicsObject * g = new TS_PhysicsObject(new btBoxShape(btVector3(hy, hx, hz)), 0.0f, false, false, btVector3(px, py, pz));
  physicsObjectsById[id] = g;
  idsByPtr[static_cast<void*>(g->rbody)] = id;
}

void TS_BtAddTriggerBox(int id, float hx, float hy, float hz, float px, float py, float pz)
{
  TS_PhysicsObject * g = new TS_PhysicsObject(new btBoxShape(btVector3(hy, hx, hz)), 1.0f, false, true, btVector3(px, py, pz));
  physicsObjectsById[id] = g;
  idsByPtr[static_cast<void*>(g->cobj)] = id;
}

void TS_BtRemovePhysicsObject(int id)
{
  TS_PhysicsObject * g = physicsObjectsById[id];
  physicsObjectsById.erase(id);
  if (g->rbody)
    idsByPtr.erase(static_cast<void *>(g->rbody));
  else
    idsByPtr.erase(static_cast<void *>(g->cobj));
  delete g;
}

void TS_BtSetLinearVelocity(int id, float vx, float vy, float vz)
{
  TS_PhysicsObject * g = physicsObjectsById[id];
  if (g->rbody)
  {
    g->rbody->setLinearVelocity(btVector3(vx, vy, vz));
    btVector3 vel = g->rbody->getLinearVelocity();
  }
}

TS_VelocityInfo TS_BtGetLinearVelocity(int id)
{
  TS_PhysicsObject * g = physicsObjectsById[id];
  TS_VelocityInfo vel;
  if (g->rbody)
  {
    btVector3 _vel = g->rbody->getLinearVelocity();
    vel.x = _vel.x();
    vel.y = _vel.y();
    vel.z = _vel.z();
    return vel;
  }
  else
  {
    vel.x = 0;
    vel.y = 0;
    vel.z = 0;
    return vel;
  }
}

void TS_BtStepSimulation()
{
  btdw.stepSimulation(0.01667f); // same as Starlight's clock

  CollisionPairs newPairs;

  for (int i = 0; i < btcd.getNumManifolds(); ++i)
  {
    // get the manifold
    btPersistentManifold* man = btcd.getManifoldByIndexInternal(i);

    // ignore manifolds that have 
    // no contact points.
    if (man->getNumContacts() > 0) {
      // get the two rigid bodies involved in the collision
      const void* b0 = static_cast<const void*>(man->getBody0());
      const void* b1 = static_cast<const void*>(man->getBody1());
    
      // always create the pair in a predictable order
      // (use the pointer value..)
      bool const swapped = b0 > b1;
      const void* bA = swapped ? b1 : b0;
      const void* bB = swapped ? b0 : b1;
      
      // create the pair
      CollisionPair newPair = std::make_pair(bA, bB);
      
      // insert the pair into the current list
      newPairs.insert(newPair);

      // if this pair doesn't exist in the list
      // from the previous update, it is a new
      // pair and we must send a collision event
      if (oldPairs.find(newPair) == oldPairs.end()) {
        TS_CollisionEvent t = TS_CollisionEvent();
        t.id1 = idsByPtr[bA];
        t.id2 = idsByPtr[bB];
        t.colliding = true;
        collisions.push(t);
      }
    }
  }

  // create another list for pairs that
  // were removed this update
  CollisionPairs removedPairs;

  // this handy function gets the difference beween
  // two sets. It takes the difference between
  // collision pairs from the last update, and this 
  // update and pushes them into the removed pairs list
  std::set_difference( oldPairs.begin(), oldPairs.end(),
  newPairs.begin(), newPairs.end(),
  std::inserter(removedPairs, removedPairs.begin()));
  
  // iterate through all of the removed pairs
  // sending separation events for them
  for (CollisionPairs::const_iterator it = removedPairs.begin(); it != removedPairs.end(); ++it) {
    TS_CollisionEvent t = TS_CollisionEvent();
    t.id1 = idsByPtr[it->first];
    t.id2 = idsByPtr[it->second];
    t.colliding = false;
    collisions.push(t);
  }
  
  // in the next iteration we'll want to
  // compare against the pairs we found
  // in this iteration
  oldPairs = newPairs;
}

TS_CollisionEvent TS_BtGetNextCollision()
{
  if (collisions.empty())
  {
    TS_CollisionEvent t = TS_CollisionEvent();
    t.id1 = -1;
    t.id2 = -1;
    t.colliding = false;
    return t;
  }
  else
  {
    TS_CollisionEvent ret = collisions.front();
    collisions.pop();
    return ret;
  }
}

TS_PositionInfo TS_BtGetPosition(int id)
{
  btVector3 pos = physicsObjectsById[id]->getTransform().getOrigin();
  TS_PositionInfo p = TS_PositionInfo();
  p.x = float(pos.x());
  p.y = float(pos.y());
  p.z = float(pos.z());
  return p;
}

void TS_BtSetGravity(float gx, float gy, float gz)
{
  btdw.setGravity(btVector3(gx, gy, gz));
}

void TS_BtSetCollisionMargin(int id, float margin)
{
  physicsObjectsById[id]->cshape->setMargin(margin);
}

void TS_BtInit()
{
  // can do anything we want here
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

void TS_VkDestroyDescriptorSet()
{
  dev.freeDescriptorSets(dscPool, 1, &dscSet);
  dev.destroy(dscSetLayout);
  dev.destroy(smp);
  dev.destroy(dscPool);
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

void TS_VkDestroyTextures()
{
  for (std::map<std::string, int>::iterator it = txtInds.begin(); it != txtInds.end(); ++it)
  {
    al.destroyImage(txts[it->second].img.first, txts[it->second].img.second);
    dev.destroyImageView(txts[it->second].view);
    availableInds.push(it->second);
  }

  txtInds.clear();
  txts.fill(TS_Texture());
  dscImgInfos.fill(vk::DescriptorImageInfo());
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
  TS_VkDestroyDescriptorSet();
  TS_VkDestroyRenderPass();
  TS_VkTeardownDepthStencil();
  TS_VkDestroyImageViews();
  TS_VkDestroySwapchain();
  TS_VmaDestroyBuffers();
  TS_VkDestroyTextures();
  TS_VmaDestroyAllocator();
  TS_VkDestroyDevice();
  TS_VkDestroySurface();
  TS_VkDestroyDebugMessenger();
  TS_VkDestroyInstance();
}

void TS_BtQuit()
{
  for (auto it = physicsObjectsById.begin(); it != physicsObjectsById.end(); ++it)
  {
    delete (*it).second;
  }
  physicsObjectsById.clear();
  idsByPtr.clear(); // no need to delete in a loop here, it only stores copies that have just been invalidated
}

void TS_Init(const char * ttl, int wdth, int hght)
{ 
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
  {
    std::cerr << "Unable to initialize SDL: " << TS_SDLGetError() << std::endl;
  }

  int img_init_flags = IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF;
  if ((IMG_Init(img_init_flags) & img_init_flags) != img_init_flags)
  {
    std::cerr << "Failed to initialize SDL_image, textures will not load" << std::endl;
  }

  int mix_init_flags = MIX_INIT_FLAC | MIX_INIT_MP3 | MIX_INIT_OGG;
  if ((Mix_Init(mix_init_flags) & mix_init_flags) != mix_init_flags)
  {
    std::cerr << "Failed to initialise audio mixer properly. All sounds may not play correctly." << std::endl << TS_SDLGetError() << std::endl; 
  }

  if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024) != 0)
  {
    std::cerr << "No audio device available, sounds and music will not play." << std::endl << TS_SDLGetError() << std::endl;
    Mix_CloseAudio();
  }

  window_name = ttl;
  window_width = wdth;
  window_height = hght;
  win = SDL_CreateWindow(ttl, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, wdth, hght, SDL_WINDOW_VULKAN|SDL_WINDOW_ALLOW_HIGHDPI|SDL_WINDOW_SHOWN);
  if (win == NULL)
  {
    std::cerr << "Failed to create window: " << TS_SDLGetError() << std::endl;
  }
  else
  {
    SDL_SetWindowMinimumSize(win, wdth, hght);
  }
  
  TS_VkInit();

  TS_BtInit();
}

void TS_Quit()
{
  TS_VkQuit();

  SDL_DestroyWindow(win);

  Mix_HaltMusic();
  Mix_HaltChannel(-1);
  Mix_CloseAudio();

  IMG_Quit();
  Mix_Quit();
  SDL_Quit();

  TS_BtQuit();
}

void TS_VkBeginDrawPass()
{
  TS_VkAcquireNextImage();
  TS_VkResetCommandBuffer();
  TS_VkBeginCommandBuffer();

  // clear data
  vertices.clear();
  indices.clear();
  current_index = 0;

  // clear buffers
  memset(al.getAllocationInfo(vertexStaging.second).pMappedData, 0, defaultBufferSize);
  memset(al.getAllocationInfo(indexStaging.second).pMappedData, 0, defaultBufferSize);
  cmdbufs[frameIndex].fillBuffer(vertexBuffer.first, 0, VK_WHOLE_SIZE, 0);
  cmdbufs[frameIndex].fillBuffer(indexBuffer.first, 0, VK_WHOLE_SIZE, 0);
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
    std::cerr << "Could not load sound file: " << std::string(sound_file) << std::endl << TS_SDLGetError() << std::endl;
    return;
  }
  if (Mix_PlayChannelTimed(-1, sample, loops, ticks) == -1)
  {
    std::cerr << "Unable to play sound " << sound_file << std::endl << TS_SDLGetError() << std::endl;
  }
}