// 
// Copyright 2022 Clemens Cords
// Created on 27.05.22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <vector>
#include <array>

#include <include/renderable.hpp>
#include <include/static_texture.hpp>
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
            void move(float x_offset, float y_offset);

            // of all vertices
            void set_color(RGBA);

            //
            RGBA get_color(size_t vertex_index) const;

            //
            size_t get_n_vertices() const;

            //
            Texture* get_texture() const;

            //
            void set_texture(Texture*);

            //
            Rectangle get_bounding_box() const;

            //
            void set_vertex_position(size_t index, Vector2f pos);

            //
            void set_vertex_color(size_t index, RGBA);

            // in [0, 1]
            void set_vertex_texture_coordinates(size_t index, Vector2f relative);

            //
            Vector2f get_vertex_position(size_t index) const;

            //
            RGBA get_vertex_color(size_t index) const;

            //
            Vector2f get_vertex_texture_coordinates(size_t index);

        protected:
            //
            Shape() = default;

            //
            void render(RenderTarget*, Transform) const final override;

            // tris
            std::vector<SDL_Vertex> _vertices;

            // call when _vertices was modified in any way
            void signal_vertices_updated();

        private:
            Texture* _texture = nullptr;

            // offset from centroid
            Vector2f _origin = {0, 0};

            Vector2f compute_centroid() const;

            // contiguous data needed for fast rendering

            std::vector<float> _xy; // spacial position, absolute
            std::vector<SDL_Color> _colors; // color
            std::vector<float> _uv; // texture position, relative

            void update_xy();
            void update_colors();
            void update_uv();

    };
}