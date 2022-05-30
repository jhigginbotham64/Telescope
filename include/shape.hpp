// 
// Copyright 2022 Clemens Cords
// Created on 27.05.22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <vector>

#include <include/renderable.hpp>
#include <include/texture.hpp>
#include <include/vertex.hpp>
#include <include/color.hpp>
#include <include/geometric_shapes.hpp>

namespace ts
{
    //
    class Shape : public Renderable
    {
        public:
            //
            void render(const RenderTarget*) const override;

            // -1 for all
            void set_color(RGBA, int vertex_index = -1);

            //
            RGBA get_color(size_t vertex_index) const;

            //
            size_t get_n_vertices() const;

            //
            Texture* get_texture() const;

            //
            void set_texture(Texture*);

            //
            void set_position(Vector2f position);

            //
            Vector2f get_position();

            //
            void set_origin(Vector2f absolute_position);

            //
            Vector2f get_origin() const;

            //
            void set_rotation(float in_degree);

        protected:
            //
            Shape();

            //
            std::vector<SDL_Vertex> _vertices;

            // forces daugters to declare origin
            virtual Vector2f get_initial_origin() = 0;

            // update the shape when fields change
            virtual void update() = 0;

        private:
            Vector2f _origin;
            Texture* _texture;
    };
}