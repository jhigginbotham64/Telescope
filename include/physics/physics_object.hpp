// 
// Copyright 2022 Clemens Cords
// Created on 6/5/22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <vector>
#include <set>

#include <box2d/b2_body.h>

#include <include/vector.hpp>
#include <include/angle.hpp>
#include <include/physics/~collision_filter.hpp>

namespace ts
{
    //
    enum PhysicsObjectType : size_t
    {
        // never moves
        STATIC = b2BodyType::b2_staticBody,

        // only responds to velocity
        KINEMATIC = b2BodyType::b2_kinematicBody,

        // fully simulated
        DYNAMIC = b2BodyType::b2_dynamicBody
    };

    //
    struct AttachmentConfig
    {
        //
        float friction = 0;

        //
        float restitution = 0;

        //
        float restitution_threshold = 0;

        //
        float density = 1;

        //
        bool is_sensor = false;
    };

    class CollisionShape;
    class PhysicsWorld;

    //
    class PhysicsObject
    {
        public:
            PhysicsObject() = delete;

            //
            PhysicsObject(PhysicsWorld*, PhysicsObjectType);

            //
            void attach(CollisionShape*, AttachmentConfig = AttachmentConfig());

            //
            void detach(CollisionShape*);

            //
            std::vector<CollisionShape*> get_all_attached() const;

            //
            bool is_attached(CollisionShape*);

            //
            void set_type(PhysicsObjectType);

            //
            PhysicsObjectType get_type() const;

            //
            void enable();

            //
            void disable();

            //
            bool is_enabled() const;

            //
            Vector2f get_origin() const;

            //
            Angle get_angle() const;

            //
            Vector2f get_center_of_mass() const;

            //
            Vector2f get_center_of_mass_global() const;

            //
            Vector2f get_center_of_mass_local() const;

            //
            void set_linear_velocity(Vector2f vec);

            //
            Vector2f get_linear_velocity() const;

            //
            void set_angular_veloctiy(float);

            //
            float get_angular_velocity() const;

            //
            void apply_force_to(Vector2f force, Vector2f point);

            //
            void apply_force_to_center(Vector2f force);

            //
            void apply_torque(float);

            //
            void apply_linear_impulse_to(Vector2f impulse, Vector2f point);

            //
            void apply_linear_impulse_to_center(Vector2f impulse);

            //
            float get_mass() const;

            //
            float get_inertia() const;

            //
            void set_is_bullet(bool);

            //
            bool is_bullet();

            //
            bool is_rotation_fixed() const;

            //
            void set_rotation_fixed(bool) const;

            //
            size_t get_id() const;

        private:
            static inline size_t object_id = 1;

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
            b2Body _body;


    };
}