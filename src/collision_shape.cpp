// 
// Copyright 2022 Joshua Higginbotham
// Created on 6/5/22 by clem (mail@clemens-cords.com | https://github.com/Clemapfel)
//

#include <include/collision_shape.hpp>
#include <include/geometric_shapes.hpp>
#include <include/physics_world.hpp>
#include <include/logging.hpp>

#include <iostream>

namespace ts
{
    void CollisionShape::assert_hidden() const
    {
        if (is_hidden())
            Log::warning("Trying to query the state of, or otherwise interacting with ts::CollisionShape while it is hidden.");
    }

    CollisionShape::CollisionShape(PhysicsWorld* world, CollisionType type, Vector2f initial_center)
        : _world(world), _id(_current_id)
    {
        _current_id = (_current_id + 1);
        initial_center = _world->world_to_native(initial_center);

        auto bodydef = default_body_def;
        bodydef.position.Set(initial_center.x, initial_center.y);
        bodydef.type = (b2BodyType) type;

        _body = world->get_native()->CreateBody(&bodydef);
    }

    CollisionShape::~CollisionShape()
    {
        /*
        if (_fixture != nullptr)
            _body->DestroyFixture(_fixture);

        if (_world != nullptr)
            _world->get_native()->DestroyBody(_body);
            */
    }

    void CollisionShape::set_density(float density)
    {
        assert_hidden();

        if (density == 0)
        {
            _fixture->SetDensity(density);
            _body->SetGravityScale(0);
        }
        else
        {
            _fixture->SetDensity(density);
            _body->SetGravityScale(1);
        }
    }

    float CollisionShape::get_density() const
    {
        assert_hidden();

        return _fixture->GetDensity();
    }

    void CollisionShape::set_friction(float friction)
    {
        assert_hidden();

        _fixture->SetFriction(friction);
    }

    float CollisionShape::get_friction() const
    {
        assert_hidden();

        return _fixture->GetFriction();
    }

    void CollisionShape::set_restitution(float restitution)
    {
        assert_hidden();

        _fixture->SetRestitution(restitution);
    }

    Rectangle CollisionShape::get_bounding_box() const
    {
        assert_hidden();

        auto aabb = _fixture->GetAABB(0);
        auto size = aabb.upperBound - aabb.lowerBound;
        return Rectangle{
            _world->native_to_world(Vector2f(aabb.lowerBound.x, aabb.lowerBound.y)),
            _world->native_to_world(Vector2f(size.x, size.y) * _world->pixel_ratio)
        };
    }

    Vector2f CollisionShape::get_centroid() const
    {
        assert_hidden();

        auto center = _fixture->GetAABB(0).GetCenter();
        return _world->native_to_world(Vector2f{center.x, center.y});
    }

    void CollisionShape::set_centroid(Vector2f vector)
    {
        assert_hidden();

    }

    Angle CollisionShape::get_rotation() const
    {
        assert_hidden();

        return ts::radians(-1 * _body->GetAngle());
    }

    b2Fixture* CollisionShape::get_native_fixture()
    {
        assert_hidden();

        return _fixture;
    }

    void CollisionShape::set_type(CollisionType type)
    {
        assert_hidden();

        _body->SetType((b2BodyType) type);
    }

    CollisionType CollisionShape::get_type() const
    {
        assert_hidden();

        return (CollisionType) _body->GetType();
    }

    void CollisionShape::set_hidden(bool b)
    {
        _body->SetEnabled(b);
    }

    bool CollisionShape::is_hidden() const
    {
        return _body->IsEnabled();
    }

    Vector2f CollisionShape::get_origin() const
    {
        assert_hidden();

        auto pos = _body->GetPosition();
        return _world->native_to_world(Vector2f(pos.x, pos.y));
    }

    Angle CollisionShape::get_angle() const
    {
        assert_hidden();

        return ts::radians(_body->GetAngle());
    }

    Vector2f CollisionShape::get_center_of_mass() const
    {
        assert_hidden();

        return get_center_of_mass_local();
    }

    Vector2f CollisionShape::get_center_of_mass_local() const
    {
        assert_hidden();

        auto pos = _body->GetLocalCenter();
        return Vector2f(pos.x, pos.y);
    }

    Vector2f CollisionShape::get_center_of_mass_global() const
    {
        assert_hidden();

        auto pos = _body->GetWorldCenter();
        return _world->native_to_world(Vector2f(pos.x, pos.y));
    }

    void CollisionShape::set_linear_velocity(Vector2f vec)
    {
        assert_hidden();

        vec = _world->world_to_native(vec);
        _body->SetLinearVelocity(b2Vec2(vec.x, vec.y));
    }

    Vector2f CollisionShape::get_linear_velocity() const
    {
        assert_hidden();

        auto out = _body->GetLinearVelocity();
        return _world->native_to_world(Vector2f(out.x, out.y));
    }

    void CollisionShape::set_angular_velocity(float value)
    {
        assert_hidden();

        _body->SetAngularVelocity(value);
    }

    float CollisionShape::get_angular_velocity() const
    {
        assert_hidden();

        return _body->GetAngularVelocity();
    }

    void CollisionShape::apply_force_to(Vector2f force, Vector2f point)
    {
        assert_hidden();

        force = _world->world_to_native(force);
        _body->ApplyForce(b2Vec2(force.x, force.y), b2Vec2(point.x, point.y), true);
    }

    void CollisionShape::apply_force_to_center(Vector2f force)
    {
        assert_hidden();

        force = _world->world_to_native(force);
        _body->ApplyForceToCenter(b2Vec2(force.x, force.y), true);
    }

    void CollisionShape::apply_torque(float torque)
    {
        assert_hidden();

        _body->ApplyTorque(torque, true);
    }

    void CollisionShape::apply_linear_impulse_to(Vector2f impulse, Vector2f point)
    {
        assert_hidden();

        _body->ApplyLinearImpulse(b2Vec2(impulse.x, impulse.y), b2Vec2(point.x, point.y), true);
    }

    void CollisionShape::apply_linear_impulse_to_center(Vector2f impulse)
    {
        assert_hidden();

        _body->ApplyLinearImpulseToCenter(b2Vec2(impulse.x, impulse.y), true);
    }

    float CollisionShape::get_mass() const
    {
        assert_hidden();

        return _body->GetMass();
    }

    float CollisionShape::get_inertia() const
    {
        assert_hidden();

        return _body->GetInertia();
    }

    void CollisionShape::set_is_bullet(bool b)
    {
        assert_hidden();

        _body->SetBullet(b);
    }

    bool CollisionShape::is_bullet()
    {
        assert_hidden();

        return _body->IsBullet();
    }

    void CollisionShape::set_rotation_fixed(bool b) const
    {
        assert_hidden();

        _body->SetFixedRotation(b);
    }

    bool CollisionShape::is_rotation_fixed() const
    {
        assert_hidden();

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
        assert_hidden();

        return _fixture->GetRestitution();
    }
}