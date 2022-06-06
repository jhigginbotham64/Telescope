//
// Created by clem on 6/6/22.
//

#include <include/physics/physics_object.hpp>
#include <include/physics/physics_world.hpp>
#include <include/physics/collision_shape.hpp>

namespace ts
{
    PhysicsObject::PhysicsObject(PhysicsWorld* world, PhysicsObjectType type)
        : _id(object_id++), _world(world)
    {
        _body = world->get_native()->CreateBody(&default_bodydef);
        _body->SetType((b2BodyType) type);
    }

    PhysicsObject::~PhysicsObject()
    {
        _attached.clear();
        _world->get_native()->DestroyBody(_body);
        _body = nullptr;
    }

    void PhysicsObject::attach(CollisionShape* shape, AttachmentConfig config)
    {
        shape->attach_to(*this, config);
        _attached.insert(shape);
    }

    void PhysicsObject::detach(CollisionShape* shape)
    {
        shape->detach_from(*this);
        _attached.erase(shape);
    }

    std::vector<CollisionShape*> PhysicsObject::get_all_attached() const
    {
        std::vector<CollisionShape*> shapes;
        shapes.reserve(_attached.size());

        for (auto* s : _attached)
            shapes.push_back(s);

        return shapes;
    }

    bool PhysicsObject::is_attached(CollisionShape* shape)
    {
        return _attached.find(shape) != _attached.end();
    }

    void PhysicsObject::set_type(PhysicsObjectType type)
    {
        _body->SetType((b2BodyType) type);
    }

    PhysicsObjectType PhysicsObject::get_type() const
    {
        return (PhysicsObjectType) _body->GetType();
    }

    void PhysicsObject::enable()
    {
        _body->SetEnabled(true);
    }

    void PhysicsObject::disable()
    {
        _body->SetEnabled(false);
    }

    bool PhysicsObject::is_enabled() const
    {
        return _body->IsEnabled();
    }

    Vector2f PhysicsObject::get_origin() const
    {
        auto pos = _body->GetPosition();
        return Vector2f(pos.x, pos.y);
    }

    Angle PhysicsObject::get_angle() const
    {
        return ts::radians(_body->GetAngle());
    }

    Vector2f PhysicsObject::get_center_of_mass() const
    {
        return get_center_of_mass_local();
    }

    Vector2f PhysicsObject::get_center_of_mass_local() const
    {
        auto pos = _body->GetLocalCenter();
        return Vector2f(pos.x, pos.y);
    }

    Vector2f PhysicsObject::get_center_of_mass_global() const
    {
        auto pos = _body->GetWorldCenter();
        return Vector2f(pos.x, pos.y);
    }

    void PhysicsObject::set_linear_velocity(Vector2f vec)
    {
        _body->SetLinearVelocity(b2Vec2(vec.x, vec.y));
    }

    Vector2f PhysicsObject::get_linear_velocity() const
    {
        auto out = _body->GetLinearVelocity();
        return Vector2f(out.x, out.y);
    }

    void PhysicsObject::set_angular_veloctiy(float value)
    {
        _body->SetAngularVelocity(value);
    }

    float PhysicsObject::get_angular_velocity() const
    {
        return _body->GetAngularVelocity();
    }

    void PhysicsObject::apply_force_to(Vector2f force, Vector2f point)
    {
        _body->ApplyForce(b2Vec2(force.x, force.y), b2Vec2(point.x, point.y), true);
    }

    void PhysicsObject::apply_force_to_center(Vector2f force)
    {
        _body->ApplyForceToCenter(b2Vec2(force.x, force.y), true);
    }

    void PhysicsObject::apply_torque(float torque)
    {
        _body->ApplyTorque(torque, true);
    }

    void PhysicsObject::apply_linear_impulse_to(Vector2f impulse, Vector2f point)
    {
        _body->ApplyLinearImpulse(b2Vec2(impulse.x, impulse.y), b2Vec2(point.x, point.y), true);
    }

    void PhysicsObject::apply_linear_impulse_to_center(Vector2f impulse)
    {
        _body->ApplyLinearImpulseToCenter(b2Vec2(impulse.x, impulse.y), true);
    }

    float PhysicsObject::get_mass() const
    {
        return _body->GetMass();
    }

    float PhysicsObject::get_inertia() const
    {
        return _body->GetInertia();
    }

    void PhysicsObject::set_is_bullet(bool b)
    {
        _body->SetBullet(b);
    }

    bool PhysicsObject::is_bullet()
    {
        return _body->IsBullet();
    }

    void PhysicsObject::set_rotation_fixed(bool b) const
    {
        _body->SetFixedRotation(b);
    }

    bool PhysicsObject::is_rotation_fixed() const
    {
        return _body->IsFixedRotation();
    }

    size_t PhysicsObject::get_id() const
    {
        return _id;
    }

    b2Body *PhysicsObject::get_native()
    {
        return _body;
    }
}