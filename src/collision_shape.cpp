// 
// Copyright 2022 Clemens Cords
// Created on 6/5/22 by clem (mail@clemens-cords.com)
//

#include <include/physics/collision_shape.hpp>
#include <include/geometric_shapes.hpp>

namespace ts
{
    void CollisionShape::attach_to(PhysicsObject& object, AttachmentConfig config)
    {
        object.attach(this, config);
    }

    void CollisionShape::detach_from(PhysicsObject& object)
    {
        object.detach(this);
    }

    void CollisionShape::set_density(float density)
    {
        _fixture.SetDensity(density);
    }

    float CollisionShape::get_density() const
    {
        return _fixture.GetDensity();
    }

    void CollisionShape::set_friction(float friction)
    {
        _fixture.SetFriction(friction);
    }

    float CollisionShape::get_friction() const
    {
        return _fixture.GetFriction();
    }

    void CollisionShape::set_restitution(float restitution)
    {
        _fixture.SetRestitution(restitution);
    }

    Rectangle CollisionShape::get_bounding_box() const
    {
        auto aabb = _fixture.GetAABB(0);
        auto size = aabb.upperBound - aabb.lowerBound;
        return Rectangle{{aabb.lowerBound.x, aabb.lowerBound.y}, {size.x, size.y}};
    }

    Vector2f CollisionShape::get_centroid() const
    {
        auto center = _fixture.GetAABB(0).GetCenter();
        Vector2f{center.x, center.y};
    }
}