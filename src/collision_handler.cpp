//
// Created by clem on 6/6/22.
//

#include <include/collision_handler.hpp>

#include <box2d/b2_distance.h>
#include <box2d/b2_contact.h>

#include <iostream>

namespace ts
{
    CollisionHandler::CollisionHandler(PhysicsWorld* world)
        : _world(world), _contact_listener(this)
    {
        _world->get_native()->SetContactListener(&_contact_listener);
    }

    DistanceInformation CollisionHandler::distance_between(CollisionShape *a, CollisionShape *b)
    {
        auto in = b2DistanceInput();
        in.proxyA.Set(a->get_native_shape(), 0);
        in.proxyB.Set(b->get_native_shape(), 0);
        in.transformA = b2Transform();
        in.transformB = b2Transform();

        auto out = b2DistanceOutput();
        auto cache = b2SimplexCache();

        b2Distance(&out, &cache, &in);

        return ts::DistanceInformation{
            out.distance,
            {Vector2f(out.pointA.x, out.pointA.y),{Vector2f(out.pointB.x, out.pointB.y)}}
        };
    }

    bool CollisionHandler::is_point_in_shape(CollisionShape *a, Vector2f point)
    {
        static auto transform = b2Transform();
        return a->get_native_shape()->TestPoint(transform, b2Vec2(point.x, point.y));
    }

    RayCastInformation CollisionHandler::ray_cast(CollisionShape *a, Vector2f ray_start, Vector2f ray_end)
    {
        static auto transform = b2Transform();
        auto* shape = a->get_native_shape();

        auto in = b2RayCastInput();
        in.p1.x = ray_start.x;
        in.p1.y = ray_start.y;
        in.p2.x = ray_end.x;
        in.p2.y = ray_end.y;
        in.maxFraction = 1; // multiplier of length

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

    namespace detail
    {

        ContactListener::ContactListener(CollisionHandler *handler)
            : _handler(handler)
        {}

        // shapes start to overlap
        void ContactListener::BeginContact(b2Contact *contact)
        {
            auto *a = contact->GetFixtureA();
            auto *b = contact->GetFixtureB();

            auto *a_data = (CollisionShape::CollisionData *) a->GetUserData().pointer;
            auto *b_data = (CollisionShape::CollisionData *) b->GetUserData().pointer;

            std::cout << "body : " << a_data->get_body_id() << " | fixture a: " << a_data->get_fixture_id() << " | fixture b: " << b_data->get_fixture_id() << std::endl;
        }

        void ContactListener::EndContact(b2Contact *contact)
        {
        }

        void ContactListener::PreSolve(b2Contact *contact, const b2Manifold *)
        {
        }

        void ContactListener::PostSolve(b2Contact *contact, const b2ContactImpulse *)
        {
        }
    }
}
