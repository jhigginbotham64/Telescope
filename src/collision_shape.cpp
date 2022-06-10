// 
// Copyright 2022 Joshua Higginbotham
// Created on 6/5/22 by clem (mail@clemens-cords.com | https://github.com/Clemapfel)
//

#include <include/physics/collision_shape.hpp>
#include <include/geometric_shapes.hpp>
#include <include/physics/physics_world.hpp>

namespace ts
{
    CollisionShape::CollisionShape(PhysicsWorld* world, CollisionType type, Vector2f initial_center)
        : _world(world), _id(_current_id)
    {
        _current_id = (_current_id + 1);

        auto bodydef = default_body_def;
        bodydef.position.Set(initial_center.x, initial_center.y);
        bodydef.type = (b2BodyType) type;

        _body = world->get_native()->CreateBody(&bodydef);
    }

    CollisionShape::~CollisionShape()
    {
        if (_fixture != nullptr)
            _body->DestroyFixture(_fixture);

        if (_world != nullptr)
            _world->get_native()->DestroyBody(_body);
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
        return Vector2f{center.x, center.y};
    }

    Angle CollisionShape::get_rotation() const
    {
        return ts::radians(-1 * _body->GetAngle());
    }

    b2Fixture* CollisionShape::get_native_fixture()
    {
        return _fixture;
    }

    void CollisionShape::set_type(CollisionType type)
    {
        _body->SetType((b2BodyType) type);
    }

    CollisionType CollisionShape::get_type() const
    {
        return (CollisionType) _body->GetType();
    }

    void CollisionShape::enable()
    {
        _body->SetEnabled(true);
    }

    void CollisionShape::disable()
    {
        _body->SetEnabled(false);
    }

    bool CollisionShape::is_enabled() const
    {
        return _body->IsEnabled();
    }

    Vector2f CollisionShape::get_origin() const
    {
        auto pos = _body->GetPosition();
        return Vector2f(pos.x, pos.y);
    }

    Angle CollisionShape::get_angle() const
    {
        return ts::radians(_body->GetAngle());
    }

    Vector2f CollisionShape::get_center_of_mass() const
    {
        return get_center_of_mass_local();
    }

    Vector2f CollisionShape::get_center_of_mass_local() const
    {
        auto pos = _body->GetLocalCenter();
        return Vector2f(pos.x, pos.y);
    }

    Vector2f CollisionShape::get_center_of_mass_global() const
    {
        auto pos = _body->GetWorldCenter();
        return Vector2f(pos.x, pos.y);
    }

    void CollisionShape::set_linear_velocity(Vector2f vec)
    {
        _body->SetLinearVelocity(b2Vec2(vec.x, vec.y));
    }

    Vector2f CollisionShape::get_linear_velocity() const
    {
        auto out = _body->GetLinearVelocity();
        return Vector2f(out.x, out.y);
    }

    void CollisionShape::set_angular_veloctiy(float value)
    {
        _body->SetAngularVelocity(value);
    }

    float CollisionShape::get_angular_velocity() const
    {
        return _body->GetAngularVelocity();
    }

    void CollisionShape::apply_force_to(Vector2f force, Vector2f point)
    {
        _body->ApplyForce(b2Vec2(force.x, force.y), b2Vec2(point.x, point.y), true);
    }

    void CollisionShape::apply_force_to_center(Vector2f force)
    {
        _body->ApplyForceToCenter(b2Vec2(force.x, force.y), true);
    }

    void CollisionShape::apply_torque(float torque)
    {
        _body->ApplyTorque(torque, true);
    }

    void CollisionShape::apply_linear_impulse_to(Vector2f impulse, Vector2f point)
    {
        _body->ApplyLinearImpulse(b2Vec2(impulse.x, impulse.y), b2Vec2(point.x, point.y), true);
    }

    void CollisionShape::apply_linear_impulse_to_center(Vector2f impulse)
    {
        _body->ApplyLinearImpulseToCenter(b2Vec2(impulse.x, impulse.y), true);
    }

    float CollisionShape::get_mass() const
    {
        return _body->GetMass();
    }

    float CollisionShape::get_inertia() const
    {
        return _body->GetInertia();
    }

    void CollisionShape::set_is_bullet(bool b)
    {
        _body->SetBullet(b);
    }

    bool CollisionShape::is_bullet()
    {
        return _body->IsBullet();
    }

    void CollisionShape::set_rotation_fixed(bool b) const
    {
        _body->SetFixedRotation(b);
    }

    bool CollisionShape::is_rotation_fixed() const
    {
        return _body->IsFixedRotation();
    }

    b2Body *CollisionShape::get_native_body()
    {
        return _body;
    }

    size_t CollisionShape::get_id() const
    {
        return _id;
    }

    float CollisionShape::get_restitution() const
    {
        return _fixture->GetRestitution();
    }
}