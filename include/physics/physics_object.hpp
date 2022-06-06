// 
// Copyright 2022 Clemens Cords
// Created on 6/5/22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <vector>
#include <set>
#include <atomic>

#include <box2d/b2_body.h>

#include <include/vector.hpp>
#include <include/angle.hpp>

namespace ts
{
    /// \brief physic object types, governs how it respond to different forces
    enum PhysicsObjectType : size_t
    {
        /// \brief static: never moves
        STATIC = b2BodyType::b2_staticBody,

        /// \brief kinematic: responds to set_velocity, but to nothing else
        KINEMATIC = b2BodyType::b2_kinematicBody,

        /// \brief dynamic: responds to set_velocity and any other force impulse, including secondary ones
        DYNAMIC = b2BodyType::b2_dynamicBody
    };

    /// \brief configuration of a shape attached to a body
    struct AttachmentConfig
    {
        /// \brief friction, 0 means no friction
        float friction = 0;

        /// \brief restitution, governs "bounciness" (like that of a rubber ball)
        float restitution = 0;

        /// \brief minimum amount of force needed before restitution "springs" the force back
        float restitution_threshold = 0;

        /// \brief density, governs mass by volume. A body should have a density of > 0
        float density = 1;

        /// \brief is the shape a sensor
        bool is_sensor = false;
    };

    class CollisionShape;
    class PhysicsWorld;

    /// \brief An object, has no shape itself but any number of CollisionShapes can be attached to it
    class PhysicsObject
    {
        public:
            PhysicsObject() = delete;

            /// \brief detroy the object, the user is reponsible for safely deallocating any attached shapes
            ~PhysicsObject();

            /// \brief create an object in a world
            /// \param world: world the object is created in
            /// \param type: simulation type of object
            PhysicsObject(PhysicsWorld*, PhysicsObjectType);

            /// \brief attach a shape to the object
            /// \param shape: shape to attach
            /// \param config: configuration applied to the shape after attachment
            void attach(CollisionShape*, AttachmentConfig = AttachmentConfig());

            /// \brief detach a shape, this deallocates it
            /// \param shape: pointer to shape being detached
            void detach(CollisionShape*);

            /// \brief get list of shapes currently attached
            /// \returns vector of pointers to shapes
            std::vector<CollisionShape*> get_all_attached() const;

            /// \brief is a specific shape currently attached
            /// \param shape: pointer to shape to be tested
            /// \returns true if `this.attach(shape)` was called before, false otherwise
            bool is_attached(CollisionShape*);

            /// \brief set the type of this object
            /// \param type: simulation type
            void set_type(PhysicsObjectType);

            /// \brief get the type of this object
            /// \returns type
            PhysicsObjectType get_type() const;

            /// \brief enable this object, it will take part in collision detection and, if it is kinematic or dynamic, it will no respond to forces
            void enable();

            /// \brief disable this object, it will no longer take part in collision or simulation
            void disable();

            /// \brief is the this object enabled
            /// \returns true if enabled, false otherwise
            bool is_enabled() const;

            /// \brief get the origin of rotation for this object
            /// \returns local coodrinates
            Vector2f get_origin() const;

            /// \brief get the angle compared to the normal
            /// \returns angle
            Angle get_angle() const;

            /// \brief get the center of mass of this object
            /// \returns local coordinates of the center of mass
            Vector2f get_center_of_mass() const;

            /// \brief get the world center of mass of this object
            /// \returns world coordinates of the center of mass
            Vector2f get_center_of_mass_global() const;

            /// \brief get the center of mass of this object
            /// \returns local coordinates of the center of mass
            Vector2f get_center_of_mass_local() const;

            /// \brief set the linear velocity along both directions
            /// \param vec: velocity, where x: velocity along x-axis, y: velocity along y-axis
            void set_linear_velocity(Vector2f vec);

            /// \brief get the linear velocity
            /// \returns velocity, where x: velocity along x-axis, y: velocity along y-axis
            Vector2f get_linear_velocity() const;

            /// \brief set the angular velocity, the clockwise direction (relative to he objects normal)
            /// \param velocity: positive for clockwise, negative for counter-clockwise
            void set_angular_veloctiy(float);

            /// \brief get the angular velocity, the clockwise direction (relative to he objects normal)
            /// \returns velocity positive for clockwise, negative for counter-clockwise
            float get_angular_velocity() const;

            /// \brief apply force to a point of the body
            /// \param force: force vector in both directions
            /// \param point: coordinates of the point
            void apply_force_to(Vector2f force, Vector2f point);

            /// \brief apply force to the center of mass of the body
            /// \param force: force vector in both directions
            void apply_force_to_center(Vector2f force);

            /// \brief apply torque to the object
            /// \param value: torque value
            void apply_torque(float);

            /// \brief apply a linear impulse to a point part of an object, unlike with `appply_force`, the impulse will end immediately and any further movement is governed by the simulation
            /// \param impulse: impulse vector in both directions
            /// \param pointer: point the impulse will be applied to
            void apply_linear_impulse_to(Vector2f impulse, Vector2f point);

            /// \brief apply a linear impulse to the center of mass of an object, unlike with `appply_force`, the impulse will end immediately and any further movement is governed by the simulation
            /// \param impulse: impulse vector in both directions
            void apply_linear_impulse_to_center(Vector2f impulse);

            /// \brief get the total mass of the object
            float get_mass() const;

            /// \brief get the total inertia of the object
            float get_inertia() const;

            /// \brief enable bullet-mode, c.f. https://box2d.org/documentation/structb2_body_def.html#a7c0047c9a98a1d20614eeddcdbce7586
            /// \param value: true if bullet-mode should be enabled, false otherwise
            void set_is_bullet(bool);

            /// \brief is bullet-mode enabled
            /// \returns true if enabled, false otherwise
            bool is_bullet();

            /// \brief is the objects rotation fixed
            /// \returns true if object cannot rotate, false otherwise
            bool is_rotation_fixed() const;

            /// \brief set whether the object can rotate
            /// \param value: true if object should not rotate, false otherwise
            void set_rotation_fixed(bool) const;

            /// \brief get the internal id of the object
            /// \returns id
            size_t get_id() const;

            /// \brief get the native box2d body of the object
            /// \returns pointer to body
            b2Body* get_native();

        private:
            static inline std::atomic<size_t> object_id = 1;
            const size_t _id;

            static inline const b2BodyDef default_bodydef = []() -> b2BodyDef
            {
                auto out = b2BodyDef();
                out.type = (b2BodyType) STATIC;
                out.position = b2Vec2(0, 0);
                out.angle = 0;
                out.linearVelocity = b2Vec2(0, 0);
                out.angularVelocity = 0;
                out.linearDamping = 0;
                out.angularDamping = 0;
                out.allowSleep = true;
                out.awake = false;
                out.fixedRotation = false;
                out.bullet = false;
                out.enabled = false;
                out.gravityScale = 1;
                return out;
            }();

            std::set<CollisionShape*> _attached;
            PhysicsWorld* _world;
            b2Body* _body;
    };
}