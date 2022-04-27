/*
  this file contains the C interface used to generate
  wrappers in other programming languages. not all
  functions are exposed due to use of C++ arguments
  and return types.
*/

#ifdef __cplusplus
extern "C" {
#endif

struct TS_BtPosition
{
  float x;
  float y;
  float z;
  TS_BtPosition(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
};

struct TS_CollisionInfo
{
  int id1;
  int id2;
  bool colliding;
  TS_CollisionInfo(int _id1=-1, int _id2=-1, bool _colliding=false) : id1(_id1), id2(_id2), colliding(_colliding) {}
};

void TS_BtAddRigidBox(int id, float hx, float hy, float hz, float m, float px, float py, float pz, bool isKinematic);

void TS_BtAddStaticBox(int id, float hx, float hy, float hz, float px, float py, float pz);

void TS_BtAddTriggerBox(int id, float hx, float hy, float hz, float px, float py, float pz);

void TS_BtRemoveGameObject(int id);

void TS_BtSetLinearVelocity(int id, float vx, float vy, float vz);

void TS_BtStepSimulation();

TS_CollisionInfo TS_GetNextCollision();

TS_BtPosition TS_BtGetPositionById(int id);

const char * TS_SDLGetError();

void TS_VkCmdDrawRect(float r, float g, float b, float a, int x, int y, int w, int h);

void TS_VkCmdDrawSprite(const char * img, float r, float g, float b, float a, int rx, int ry, int rw, int rh, int cw, int ch, int ci, int cj, int px, int py, float sx, float sy);

void TS_VkCmdClearColorImage(float r, float g, float b, float a);

void TS_VkAcquireNextImage();

void TS_VkResetCommandBuffer();

void TS_VkBeginCommandBuffer();

void TS_VkBeginDrawPass();

void TS_VkEndDrawPass(float r, float g, float b, float a);

void TS_VkEndCommandBuffer();

void TS_VkQueueSubmit();

void TS_VkQueuePresent();

void TS_VkCreateInstance();

void TS_VkCreateSurface();

void TS_VkSelectPhysicalDevice();

void TS_VkSelectQueueFamily();

void TS_VkCreateDevice();

#define CLAMP(x, lo, hi)    ((x) < (lo) ? (lo) : (x) > (hi) ? (hi) : (x))
void TS_VkCreateSwapchain();

void TS_VkCreateImageViews();

void TS_VkSetupDepthStencil();

void TS_VkCreateRenderPass();

void TS_VkCreateTrianglePipeline();

void TS_VkCreateFramebuffers();

void TS_VkCreateCommandPool();

void TS_VkAllocateCommandBuffers();

void TS_VkCreateSemaphores();

void TS_VkCreateFences();

void TS_VkInit();

void TS_VkDestroyFences();

void TS_VkDestroySemaphores();

void TS_VkFreeCommandBuffers();

void TS_VkDestroyCommandPool();

void TS_VkDestroyFramebuffers();

void TS_VkDestroyTrianglePipeline();

void TS_VkDestroyRenderPass();

void TS_VkTeardownDepthStencil();

void TS_VkDestroyImageViews();

void TS_VkDestroySwapchain();

void TS_VkDestroyDevice();

void TS_VkDestroySurface();

void TS_VkDestroyInstance();

void TS_VkQuit();

void TS_Init(const char * ttl, int wdth, int hght);

void TS_Quit();

void TS_PlaySound(const char* sound_file, int loops, int ticks);

#ifdef __cplusplus
}
#endif