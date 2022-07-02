// 
// Copyright 2022 Joshua Higginbotham
// Created on 6/5/22 by clem (mail@clemens-cords.com | https://github.com/Clemapfel)
//

#include <box2d/b2_contact.h>
#include <box2d/b2_distance.h>

#include <include/physics_world.hpp>
#include <include/window.hpp>
#include <include/collision_shape.hpp>

namespace ts
{
    PhysicsWorld::PhysicsWorld()
        : _world(_default_gravity), _contact_listener(this)
    {
        get_native()->SetContactListener(&_contact_listener);
    }

    PhysicsWorld::~PhysicsWorld()
    {}

    void PhysicsWorld::step(Time timestep, int32_t velocity_iterations, int32_t position_iterations)
    {
        _world.Step(timestep.as_seconds(), velocity_iterations, position_iterations);
    }

    b2World *PhysicsWorld::get_native()
    {
        return &_world;
    }

    // native: physics world
    // world: render space
    Vector2f PhysicsWorld::native_to_world(Vector2f in) const
    {
        return Vector2f(in.x * pixel_ratio, in.y * pixel_ratio);
    }

    Vector2f PhysicsWorld::world_to_native(Vector2f in) const
    {
        return Vector2f(in.x / pixel_ratio, in.y / pixel_ratio);
    }

    float PhysicsWorld::get_skin_radius() const
    {
        // in Box2D, polygon (as opposed to circles) have a "skin" which is a buffer zone around the
        // physics object. If configured wrongly, objects will not look like they are colliding, because
        // their hitbox is increased by the size of the skin. If set too low, the objects will
        // seem to overlap.

        return 1.5f / pixel_ratio;
    }

    void PhysicsWorld::clear_forces()
    {
        _world.ClearForces();
    }

    void PhysicsWorld::set_gravity(Vector2f vec)
    {
        vec = world_to_native(vec);
        _world.SetGravity(b2Vec2(vec.x, vec.y));
    }

    Vector2f PhysicsWorld::get_gravity() const
    {
        auto gravity = _world.GetGravity();
        auto out = Vector2f(gravity.x, gravity.y);
        return native_to_world(out);
    }

    DistanceInformation PhysicsWorld::distance_between(CollisionShape *a, CollisionShape *b)
    {
        auto in = b2DistanceInput();
        in.proxyA.Set(a->get_native_fixture()->GetShape(), 0);
        in.proxyB.Set(b->get_native_fixture()->GetShape(), 0);
        in.transformA = a->get_native_body()->GetTransform();
        in.transformB = b->get_native_body()->GetTransform();

        auto out = b2DistanceOutput();
        auto cache = b2SimplexCache();

        b2Distance(&out, &cache, &in);
        return ts::DistanceInformation{
                out.distance * pixel_ratio,
                {native_to_world(Vector2f(out.pointA.x, out.pointA.y)), native_to_world(Vector2f(out.pointB.x, out.pointB.y))}
        };
    }

    bool PhysicsWorld::is_point_in_shape(CollisionShape *a, Vector2f point)
    {
        point = world_to_native(point);
        return a->get_native_shape()->TestPoint(a->get_native_body()->GetTransform(), b2Vec2(point.x, point.y));
    }

    RayCastInformation PhysicsWorld::ray_cast(CollisionShape *a, Vector2f ray_start, Vector2f ray_end, float multiplier)
    {
        static auto transform = a->get_native_body()->GetTransform();
        auto* shape = a->get_native_shape();

        ray_start = world_to_native(ray_start);
        ray_end = world_to_native(ray_end);

        auto in = b2RayCastInput();
        in.p1.x = ray_start.x;
        in.p1.y = ray_start.y;
        in.p2.x = ray_end.x;
        in.p2.y = ray_end.y;
        in.maxFraction = multiplier; // multiplier of length

        auto out = b2RayCastOutput();

        bool hit = shape->RayCast(&out, in, transform, 0);
        b2Vec2 hit_point = in.p1 + out.fraction * (in.p2 - in.p1);
        // source: https://box2d.org/documentation/md__d_1__git_hub_box2d_docs_collision.html#autotoc_md43

        return RayCastInformation {
                hit,
                Vector2f(out.normal.x, out.normal.y),
                Vector2f(hit_point.x, hit_point.y)
        };
    }

    bool PhysicsWorld::next_event(CollisionEvent* event)
    {
        auto lock = std::lock_guard(_queue_lock);

        if (_event_queue.empty())
        {
            event->type = CollisionEvent::CONTACT_END;
            event->shape_a = nullptr;
            event->shape_b = nullptr;
            return false;
        }

        auto out = _event_queue.front();
        _event_queue.pop_front();

        event->type = out.type;
        event->shape_a = out.shape_a;
        event->shape_b = out.shape_b;
        return true;
    }

    void PhysicsWorld::clear_events()
    {
        auto lock = std::lock_guard(_queue_lock);
        _event_queue.clear();
    }

    PhysicsWorld::ContactListener::ContactListener(PhysicsWorld * world)
        : _world(world)
    {}

    // shapes start to overlap
    void PhysicsWorld::ContactListener::BeginContact(b2Contact *contact)
    {
        auto lock = std::lock_guard(_world->_queue_lock);
        _world->_event_queue.push_back({
             CollisionEvent::CONTACT_START,
             (CollisionShape *) contact->GetFixtureA()->GetUserData().pointer,
             (CollisionShape *) contact->GetFixtureB()->GetUserData().pointer
        });
    }

    void PhysicsWorld::ContactListener::EndContact(b2Contact *contact)
    {
        auto lock = std::lock_guard(_world->_queue_lock);
        _world->_event_queue.push_back({
             CollisionEvent::CONTACT_END,
             (CollisionShape *) contact->GetFixtureA()->GetUserData().pointer,
             (CollisionShape *) contact->GetFixtureB()->GetUserData().pointer
        });
    }

    void PhysicsWorld::ContactListener::PreSolve(b2Contact*, const b2Manifold*)
    {
        // noop
    }

    void PhysicsWorld::ContactListener::PostSolve(b2Contact*, const b2ContactImpulse*)
    {
        // noop
    }
}

