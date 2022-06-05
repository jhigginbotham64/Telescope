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
    //
    struct CollisionShape
    {
        public:
            //
            void attach_to(PhysicsObject&, AttachmentConfig = AttachmentConfig());

            //
            void detach_from(PhysicsObject&);

            //
            void set_density(float);

            //
            float get_density() const;

            //
            void set_friction(float);

            //
            float get_friction() const;

            //
            void set_restitution(float);

            //
            float get_restitution() const;

            //
            Rectangle get_bounding_box() const;

            //
            Vector2f get_centroid() const;

        protected:
            b2Fixture _fixture;
    };
}