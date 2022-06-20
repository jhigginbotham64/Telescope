// 
// Copyright 2022 Joshua Higginbotham
// Created on 6/19/22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <include/collision_shape.hpp>
#include <include/collision_line.hpp>
#include <include/collision_line_sequence.hpp>
#include <include/collision_polygon.hpp>
#include <include/collision_circle.hpp>

namespace ts
{
    /// \brief renderable collision shape
    struct CollisionRenderShape
    {
        /// \brief synchronize the hitbox and render shape
        virtual void update() = 0;

        protected:
            Angle _rotation = degrees(0);
    };

    /// \brief triangle shape with identically sized hitbox
    class CollisionTriangleShape : public CollisionRenderShape, public TriangleShape, public CollisionPolygon
    {
        public:
            // no docs
            virtual ~CollisionTriangleShape() = default;

            /// \brief construct
            /// \param world: physics world
            /// \param type: collision type
            /// \param a: first vertex position
            /// \param b: second vertex position
            /// \param c: thrid vertex position
            CollisionTriangleShape(PhysicsWorld*, CollisionType, Vector2f a, Vector2f b, Vector2f c);

            /// \brief synchronize the position of the shape with that of the hitbox
            void update() override;
    };

    /// \brief debug shape that is a rectangle with a hitbox of the same size
    class CollisionRectangleShape : public CollisionRenderShape, public RectangleShape, public CollisionPolygon
    {
        public:
            // no docs
            virtual ~CollisionRectangleShape() = default;

            /// \brief construct
            /// \param world: physics world
            /// \param type: collision type
            /// \param top_left: top left coordinate of the shape
            /// \param size: size of the shape
            CollisionRectangleShape(PhysicsWorld*, CollisionType, Vector2f top_left, Vector2f size);

            /// \brief synchronize the shapes position with that of its hitbox
            void update() override;
    };

    /// \brief renderable polygon
    class CollisionPolygonShape : public CollisionRenderShape, public PolygonShape//, public CollisionPolygon
    {
        public:
            // no docs
            virtual ~CollisionPolygonShape() = default;

            /// \brief construct
            /// \param world: physics world
            /// \param type: collision type
            /// \param vertex_positions: positions of vertices
            CollisionPolygonShape(PhysicsWorld*, CollisionType, const std::vector<Vector2f>&);

            /// \brief synchronize the position of the shape with that of the hitbox
            void update() override;
    };

    /// \brief debug shape, a renderable circle with a hitbox the same size as the render shape
    class CollisionCircleShape : public CollisionRenderShape, public CircleShape, public CollisionCircle
    {
        public:
            // no docs
            virtual ~CollisionCircleShape() = default;

            /// \brief create
            /// \param world: physics world
            /// \param type: collision type
            /// \param center: position of the centroid of the circle
            /// \param radius: radius of the circle
            CollisionCircleShape(PhysicsWorld*, CollisionType, Vector2f center, float radius);

            /// \brief synchronize the shapes position with the hitbox' position
            void update() override;
    };

    /// \brief renderable 1-pixel wide line
    class CollisionLineShape : public CollisionRenderShape, public CollisionLine, public RectangleShape
    {
        public:
            // no docs
            virtual ~CollisionLineShape() = default;

            /// \brief construct
            /// \param world: physics world
            /// \param type: collision type
            /// \param a: start of the line
            /// \param b: end of the line
            CollisionLineShape(PhysicsWorld*, CollisionType, Vector2f a, Vector2f b);

            /// \brief synchronize the shapes position with that of its hitbox
            void update() override;
    };

    /// \brief renderable line sequence
    class CollisionLineSequenceShape : public CollisionRenderShape, public CollisionLineSequence, public Renderable
    {
        public:
            // no docs
            virtual ~CollisionLineSequenceShape() = default;

            /// \brief construct
            /// \param world: physics world
            /// \param type: collision type
            /// \param vertex_positions: positions of vertices
            /// \param is_two_sided
            CollisionLineSequenceShape(PhysicsWorld*, CollisionType, const std::vector<Vector2f>&, bool is_two_sided = true);

            /// \brief synchronize the position of the shape with that of the hitbox
            void update() override;

            /// \brief expose render shapes
            /// \returns shapes
            std::vector<RectangleShape>& get_shapes();

        protected:
            void render(RenderTarget *target, Transform transform) const;

        private:
            std::vector<RectangleShape> _lines;
    };
}