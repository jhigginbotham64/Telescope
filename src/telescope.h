//
// Copyright 2022, Joshua Higginbotham
//

//
//  this file contains the C interface used to generate
//  wrappers in other programming languages. not all
//  functions are exposed due to use of C++ arguments
//  and return types.
//

#pragma once

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

struct TS_VelocityInfo
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

void TS_BtAddTriggerBox(int id, float hx, float hy, float hz, float px, float py, float pz);

void TS_BtRemovePhysicsObject(int id);

void TS_BtSetLinearVelocity(int id, float vx, float vy, float vz);

struct TS_VelocityInfo TS_BtGetLinearVelocity(int id);

void TS_BtSetGravity(float gx, float gy, float gz);

void TS_BtSetCollisionMargin(int id, float margin);

void TS_BtStepSimulation();

struct TS_CollisionEvent TS_BtGetNextCollision();

struct TS_PositionInfo TS_BtGetPosition(int id);

const char * TS_SDLGetError();

void TS_VkCmdDrawRect(float r, float g, float b, float a, float x, float y, float w, float h);

void TS_VkCmdDrawSprite(const char * img, float r, float g, float b, float a, int rx, int ry, int rw, int rh, int cw, int ch, int ci, int cj, float px, float py, float sx, float sy);

void TS_VkCmdClearColorImage(float r, float g, float b, float a);

void TS_VkBeginDrawPass();

void TS_VkEndDrawPass(float r, float g, float b, float a);

void TS_Init(const char * ttl, int wdth, int hght);

void TS_Quit();

void TS_PlaySound(const char* sound_file, int loops, int ticks);

#ifdef __cplusplus
}
#endif