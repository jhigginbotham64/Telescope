/*
  this file contains the C interface used to generate
  wrappers in other programming languages. not all
  functions are exposed due to use of C++ arguments
  and return types.
*/

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

struct TS_PositionInfo
{
  float x;
  float y;
  float z;
};

struct TS_CollisionEvent
{
  int id1;
  int id2;
  bool colliding;
};

void TS_BtAddRigidBox(int id, float hx, float hy, float hz, float m, float px, float py, float pz, bool isKinematic);

void TS_BtAddStaticBox(int id, float hx, float hy, float hz, float px, float py, float pz);

void TS_BtAddStaticTriggerBox(int id, float hx, float hy, float hz, float px, float py, float pz);

void TS_BtRemovePhysicsObject(int id);

void TS_BtSetLinearVelocity(int id, float vx, float vy, float vz);

void TS_BtStepSimulation();

struct TS_CollisionEvent TS_BtGetNextCollision();

struct TS_PositionInfo TS_BtGetPositionById(int id);

const char * TS_SDLGetError();

void TS_VkCmdDrawRect(float r, float g, float b, float a, int x, int y, int w, int h);

void TS_VkCmdDrawSprite(const char * img, float r, float g, float b, float a, int rx, int ry, int rw, int rh, int cw, int ch, int ci, int cj, int px, int py, float sx, float sy);

void TS_VkCmdClearColorImage(float r, float g, float b, float a);

void TS_VkBeginDrawPass();

void TS_VkEndDrawPass(float r, float g, float b, float a);

void TS_Init(const char * ttl, int wdth, int hght);

void TS_Quit();

void TS_PlaySound(const char* sound_file, int loops, int ticks);

#ifdef __cplusplus
}
#endif