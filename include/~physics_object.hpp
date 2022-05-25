//
// Copyright 2022, Joshua Higginbotham
//

#pragma once

#include <btBulletCollisionCommon.h>

extern "C"
{

/// \brief positional information, describes point in 3D space
struct TS_PositionInfo
{
    /// \brief x-coordinate
    float x;

    /// \brief y-coordinate
    float y;

    /// \brief z-coodrinate
    float z;
};

/// \brief velocity information, describes velocity vector in 3D space
struct TS_VelocityInfo
{
    /// \brief velocity along x-axis
    float x;

    /// \brief velocity along y-axis
    float y;

    /// \brief velocity along z-axis
    float z;
};

/// \brief proxy representing a bullet physics-enabled object
struct TS_PhysicsObject
{

    /// \brief internal bullet collision object
    btCollisionObject* cobj;

    /// \brief collision shape
    btCollisionShape* cshape;

    /// \brief rigid body
    btRigidBody* rbody;

    /// \brief default motion state
    btDefaultMotionState* dmstate;

    /// \brief construct the object
    /// \param s: bullet collision shape
    /// \param mass: [optional] mass
    /// \param isKinematic: [optional] is a kinematic object
    /// \param isTrigger: [optional] is a trigger object
    /// \param initPos: [optional] initial position in 3d space
    /// \param initRot: [optional] initial quaternion
    TS_PhysicsObject(btCollisionShape* s,
                     float mass = 0.0f,
                     bool isKinematic = false,
                     bool isTrigger = false,
                     const btVector3& initPos = btVector3(0, 0, 0),
                     const btQuaternion& initRot = btQuaternion(0, 0, 1, 1));

    /// \brief destructor
    ~TS_PhysicsObject();

    /// \brief access transforms
    /// \returns btTransform object
    btTransform getTransform();
};

}