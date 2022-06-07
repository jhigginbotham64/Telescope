// 
// Copyright 2022 Clemens Cords
// Created on 6/5/22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <include/vector.hpp>
#include <include/geometric_shapes.hpp>
#include <include/shape.hpp>
#include <include/circle_shape.hpp>
#include <include/rectangle_shape.hpp>
#include <include/triangle_shape.hpp>
#include <include/polygon_shape.hpp>
#include <include/physics/physics_object.hpp>

#include <box2d/b2_circle_shape.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_edge_shape.h>
#include <box2d/b2_chain_shape.h>
#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>

namespace ts
{
    /// \brief abstract collision shape
    struct CollisionShape
    {
        public:
            /// \brief destruct, this also deallocates the box2d fixture. The user is responsible for keeping the shape in memory while it is attached to a PhysicsObject
            ~CollisionShape();

            /// \brief attach to a physics object
            /// \param object: physics object
            /// \param config: config for this shape
            void attach_to(PhysicsObject&, AttachmentConfig = AttachmentConfig());

            /// \brief detach from a physics object
            /// \param object: physics object
            void detach_from(PhysicsObject&);

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

            /// \brief get the native box2d shape
            /// \returns pointer to shape
            virtual b2Shape* get_native_shape() = 0;

            /// \brief get the native box2d fixture
            /// \returns pointer to fixture
            b2Fixture* get_native_fixture();

        protected:
            b2Body* _body;
            b2Fixture* _fixture;
    };
}