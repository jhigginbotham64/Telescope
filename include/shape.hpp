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
            virtual void set_centroid(Vector2f position) = 0;

            //
            virtual Vector2f get_centroid() const = 0;

            //
            void render(const RenderTarget*) const final override;

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
            void set_origin(Vector2f absolute_position);

            //
            Vector2f get_origin() const;

            //
            void set_rotation(float in_degree);

            //
            Rectangle get_bounding_box() const;

        protected:
            //
            Shape() = default;

            // tris
            std::vector<SDL_Vertex> _vertices;

        private:
            Texture* _texture;

            // offset from centroid
            Vector2f _origin = {0, 0};

            Vector2f compute_centroid() const;
    };
}