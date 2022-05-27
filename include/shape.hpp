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
            Vector2f get_position(Vector2f);

        protected:
            //
            Shape(std::vector<Vertex>);

            //
            std::vector<SDL_Vertex> _vertices;

        private:
            Texture* _texture;
    };
}