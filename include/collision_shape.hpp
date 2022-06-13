// 
// Copyright 2022 Joshua Higginbotham
// Created on 6/5/22 by clem (mail@clemens-cords.com | https://github.com/Clemapfel)
//

#pragma once

#include <atomic>

#include <include/vector.hpp>
#include <include/geometric_shapes.hpp>
#include <include/shape.hpp>
#include <include/circle_shape.hpp>
#include <include/rectangle_shape.hpp>
#include <include/triangle_shape.hpp>
#include <include/polygon_shape.hpp>

#include <box2d/b2_circle_shape.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_edge_shape.h>
#include <box2d/b2_chain_shape.h>
#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>

namespace ts
{
    class PhysicsWorld;

    /// \brief physic object types, governs how it respond to different forces
    enum CollisionType : size_t
    {
        /// \brief static: never moves
        STATIC = b2BodyType::b2_staticBody,

        /// \brief kinematic: responds to set_velocity, but to nothing else
        KINEMATIC = b2BodyType::b2_kinematicBody,

        /// \brief dynamic: responds to set_velocity and any other force impulse, including secondary ones
        DYNAMIC = b2BodyType::b2_dynamicBody
    };

    /// \brief abstract collision shape
    class CollisionShape
    {
        public:
            /// \brief destruct, this also deallocates the box2d fixture. The user is responsible for keeping the shape in memory while it is attached to a PhysicsObject
            virtual ~CollisionShape();

            /// \brief set the density of this shape. This governs mass
            /// \param density
            void set_density(float);

            /// \brief access the density of this shape
            /// \returns density
            float get_density() const;

            /// \brief set the friction of the shape
            /// \param friction
            void set_friction(float);

            /// \brief get the friction of the shape
            /// \returns friction
            float get_friction() const;

            /// \brief set the restitution of the shape
            /// \param restitution
            void set_restitution(float);

            /// \brief get the restitution of the shape
            /// \returns restitution
            float get_restitution() const;

            /// \brief compute the axis-aligned bounding box of the shape
            /// \returns rectangle that is the bounding box
            Rectangle get_bounding_box() const;

            /// \brief get the shapes center of mass
            /// \returns world coordinates of the center of mass
            Vector2f get_centroid() const;

            /// \brief teleport to position, regardless of whether there is an object in the way
            /// \param position: position in 2d space
            void set_centroid(Vector2f);

            /// \brief get the rotation of the shape
            /// \returns rotation, respective the the normal the shape spawned with
            Angle get_rotation() const;

            /// \brief get the native box2d shape
            /// \returns pointer to shape
            virtual b2Shape* get_native_shape() = 0;

            /// \brief get the native box2d fixture
            /// \returns pointer to fixture
            b2Fixture* get_native_fixture();

            /// \brief get the native box2d body of the object
            /// \returns pointer to body
            b2Body* get_native_body();

            /// \brief set the type of this object
            /// \param type: simulation type
            void set_type(CollisionType);

            /// \brief get the type of this object
            /// \returns type
            CollisionType get_type() const;

            /// \brief set whether this object should be disabled
            /// \param bool
            void set_hidden(bool);

            /// \brief is the this object enabled
            /// \returns true if enabled, false otherwise
            bool is_hidden() const;

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
            void set_angular_velocity(float);

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

        protected:
            /// \brief create an object in a world
            /// \param world: world the object is created in
            /// \param initial_center: initial position of the shape
            /// \param type: simulation type of object
            CollisionShape(PhysicsWorld*, CollisionType, Vector2f initial_center);

            PhysicsWorld* _world;
            b2Body* _body;
            b2Fixture* _fixture;

            static inline std::atomic<bool> _current_id;
            size_t _id;

            static inline const b2BodyDef default_body_def = []() -> b2BodyDef
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
                out.awake = true;
                out.fixedRotation = false;
                out.bullet = false;
                out.enabled = true;
                out.gravityScale = 1;
                return out;
            }();

            static inline const b2FixtureDef default_fixture_def = []() -> b2FixtureDef
            {
                auto out = b2FixtureDef();
                out.shape = nullptr;
                out.friction = 0;
                out.restitution = 0;
                out.restitutionThreshold = 0;
                out.density = 10;
                out.isSensor = false;
                return out;
            }();

            void assert_hidden() const;
    };
}