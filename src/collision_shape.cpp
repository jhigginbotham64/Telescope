// 
// Copyright 2022 Clemens Cords
// Created on 6/5/22 by clem (mail@clemens-cords.com)
//

#include <include/physics/collision_shape.hpp>
#include <include/geometric_shapes.hpp>

namespace ts
{
    CollisionShape::~CollisionShape()
    {
        if (_body != nullptr)
            _body->DestroyFixture(_fixture);
    }

    void CollisionShape::attach_to(PhysicsObject& object, AttachmentConfig config)
    {
        _fixture = _body->CreateFixture(get_native_shape(), config.density);

        _fixture->SetFriction(config.friction);
        _fixture->SetRestitution(config.restitution);
        _fixture->SetRestitutionThreshold(config.restitution_threshold);
        _fixture->SetDensity(config.density);
        _fixture->SetSensor(config.is_sensor);
    }

    void CollisionShape::detach_from(PhysicsObject& object)
    {
        _body->DestroyFixture(_fixture);
        _fixture = nullptr;
        _body = nullptr;
    }

    void CollisionShape::set_density(float density)
    {
        _fixture->SetDensity(density);
    }

    float CollisionShape::get_density() const
    {
        return _fixture->GetDensity();
    }

    void CollisionShape::set_friction(float friction)
    {
        _fixture->SetFriction(friction);
    }

    float CollisionShape::get_friction() const
    {
        return _fixture->GetFriction();
    }

    void CollisionShape::set_restitution(float restitution)
    {
        _fixture->SetRestitution(restitution);
    }

    Rectangle CollisionShape::get_bounding_box() const
    {
        auto aabb = _fixture->GetAABB(0);
        auto size = aabb.upperBound - aabb.lowerBound;
        return Rectangle{{aabb.lowerBound.x, aabb.lowerBound.y}, {size.x, size.y}};
    }

    Vector2f CollisionShape::get_centroid() const
    {
        auto center = _fixture->GetAABB(0).GetCenter();
        Vector2f{center.x, center.y};
    }

    b2Fixture* CollisionShape::get_native_fixture()
    {
        return _fixture;
    }
}