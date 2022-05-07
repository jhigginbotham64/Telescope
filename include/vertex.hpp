//
// Copyright 2022, Joshua Higginbotham
//

#pragma once

#include <glm/glm.hpp>
#include <vulkan/vulkan.hpp>

/// \brief vertex object
struct TS_Vertex {

  /// \brief position in 2D space
  glm::vec2 pos;

  /// \brief uv coordinate
  glm::vec2 uv;

  /// \brief color, in RGBA
  glm::vec4 col;

  /// \brief texture id
  int tex;

  /// \brief ctor
  /// \param x: x position
  /// \param y: y position
  /// \param r: red component
  /// \param g: green component
  /// \param b: blue component
  /// \param a: transparency component
  /// \param u: u-coordinate
  /// \param v: v-coordinate
  /// \param t: texture id
  TS_Vertex(float x, float y, float r, float g, float b, float a, float u = 0, float v = 0, int t = -1);

  /// \brief get vertices vulkan binding description
  /// \returns description
  static vk::VertexInputBindingDescription getBindingDescription();

  /// \brief get vertices vulkan attribute description
  /// \returns 4-array of descriptions
  static std::array<vk::VertexInputAttributeDescription, 4> getAttributeDescriptions();
};