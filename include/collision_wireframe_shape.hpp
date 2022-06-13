// 
// Copyright 2022 Clemens Cords
// Created on 6/13/22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <include/physics_world.hpp>
#include <include/collision_wireframe.hpp>

namespace ts
{
    //
    class CollisionWireframeShape : public CollisionWireframe//, public Renderable
    {
        public:
            //
            CollisionWireframeShape(PhysicsWorld*, CollisionType, const std::vector<Vector2f>&);

            //
            void update();

        protected:
            //
            //virtual void render(RenderTarget* target, Transform transform) const override;

        private:
            std::vector<RectangleShape> _lines;
    };
}