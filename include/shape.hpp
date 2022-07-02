// 
// Copyright 2022 Joshua Higginbotham
// Created on 27.05.22 by clem (mail@clemens-cords.com | https://github.com/Clemapfel)
//

#pragma once

#include <vector>
#include <array>

#include <include/renderable.hpp>
#include <include/static_texture.hpp>
#include <include/color.hpp>
#include <include/geometric_shapes.hpp>

namespace ts
{
    /// \brief a textured, vertex-based shape. Rendered as a triangle-fan
    class Shape : public Renderable
    {
        public:
            // no docs
            virtual ~Shape() = default;

            /// \brief set the position of the shapes centroid
            /// \param position: centroid position
            void set_centroid(Vector2f position);

            /// \brief get the position of the shapes centroid
            /// \returns position, true coordinates
            Vector2f get_centroid() const;

            /// \brief move the shape such that its centroid will be offset by the specified amount
            /// \param x_offset: offset along the x-axis, true coordinates
            /// \param y_offset: offset along the y-axis, true coordiantes
            virtual void move(float x_offset, float y_offset);

            /// \brief set all vertices color
            /// \param color
            void set_color(RGBA);

            /// \param get the color of a specific vertex
            /// \param vertex_index: index of the vertex, in [0, get_n_vertices()]
            /// \returns color
            RGBA get_color(size_t vertex_index) const;

            /// \brief get number of vertices
            /// \returns number of vertices
            size_t get_n_vertices() const;

            /// \brief get texture of the shape
            /// \returns pointer to texture, or nullptr if not texture is set
            Texture* get_texture() const;

            /// \brief set the texture of the shape, unless otherwise specified, the textures centroid will be aligned with the shapes centroid
            /// \param pointer to texture, or nullptr for no texture
            void set_texture(Texture*);

            /// \brief set which part of the texture is mapped, texture is anchored at the top left of the bounding box
            /// \param rect: rectangle, relative coordinates. Rectangle({0, 0,}, {1, 1}) is the whole texture
            void set_texture_rectangle(Rectangle rect);

            /// \brief get the texture rectangle
            Rectangle get_texture_rectangle();

            /// \brief get the axis-aligned bounding box of the shape
            /// \returns rectangle
            Rectangle get_bounding_box() const;

            /// \brief set the position of a specific vertex
            /// \param index: index of the vertex, in [0, get_n_vertices()]
            /// \param position: new position of the vertex
            void set_vertex_position(size_t index, Vector2f position);

            /// \brief set the color of a specific vertex
            /// \param index: index of the vertex, in [0, get_n_vertices()]
            /// \param color: color
            void set_vertex_color(size_t index, RGBA);

            /// \brief set the texture coordinate of a specific vertex
            /// \param index: index of the vertex, in [0, get_n_vertices()]
            /// \param relative: relative texture coordinates, each element in [0, 1]
            void set_vertex_texture_coordinates(size_t index, Vector2f relative);

            /// \brief get the true coordinates of a specific vertex
            /// \param index: index of the vertex, in [0, get_n_vertices()]
            /// \returns position, true coordinates
            Vector2f get_vertex_position(size_t index) const;

            /// \brief get the color of a specific vertex
            /// \param index: index of the vertex, in [0, get_n_vertices()]
            /// \returns color
            RGBA get_vertex_color(size_t index) const;

            /// \brief get the texture coordinates of a specific vertex
            /// \param index: index of the vertex, in [0, get_n_vertices()]
            /// \returns relative coordinates, each element in [0, 1]
            Vector2f get_vertex_texture_coordinates(size_t index);

            /// \brief set the origin of rotation and scaling
            /// \param relative_to_centroid: relative distance from centroid, where (0, 0) makes the origin the centroid
            void set_origin(Vector2f relative_to_centroid);

            /// \brief access the origin
            /// \returns origin
            Vector2f get_origin() const;

            /// \param rotate around the shapes origin
            /// \param angle
            void rotate(Angle);

            /// \brief scale around the shapes origin
            /// \param factor: 1.0 is no change, 1.5 is 50% bigger, 0.5 is 50% smaller
            void scale(float);

            /// \copydoc Renderable::render
            void render(RenderTarget*, Transform) const final override;

        protected:
            /// \brief default constructor
            Shape() = default;

            /// \brief pairwise different vertices of shapes tris
            std::vector<SDL_Vertex> _vertices;

            /// \brief
            std::vector<int> _vertex_indices;

            /// \brief signal to the shape that a vertex property has changed
            void signal_vertices_updated();

        private:
            Vector2f _origin = {0, 0};

            Texture* _texture = nullptr;
            Rectangle _texture_rect = Rectangle{{0, 0}, {1, 1}};
            void apply_texture_rectangle();

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