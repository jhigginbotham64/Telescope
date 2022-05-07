//
// Copyright 2022, Joshua Higginbotham
//

#pragma once

#include <array>
#include <include/link_as_c.hpp>

#define CLAMP(x, lo, hi)  ((x) < (lo) ? (lo) : (x) > (hi) ? (hi) : (x))

extern "C" {

/// \brief initialize the state
void TS_Init(const char * ttl, int wdth, int hght);

/// \brief quit the state
void TS_Quit();

/// \brief play a sound
/// \param path: path to the sound file
/// \param loops: [optional] number of loops, -1 for infinite
/// \param ticks: [optional] maximum time to play a sample, in milliseconds, -1 for infinite
void TS_PlaySound(const char* sound_file, int loops, int ticks);

/// \brief get sdl error
/// \returns error message
const char * TS_SDLGetError();

}

/// \brief get normalized device coordinates along the x and y axes
/// \param x: x coordinate
/// \returns normalized y coordinate
float TS_NDCX(float x);

/// \brief get normalized device coordinates along the x and y axes
/// \param y: y coordinate
/// \returns normalized x coordinate
float TS_NDCY(float y);

/// \brief normalize rectangle
/// \param x: x-coordinate of the top left corner
/// \param y: y-coordinate of the top left corner
/// \param w: size in x-dimension
/// \param h: size in y-dimension
/// \returns rectangle as 4-array of floats
std::array<float, 4> TS_NDCRect(float x, float y, float w, float h);

/// \brief normalize texture coordinates along the u axis
/// \param x: x-coordinate
/// \param w: width
/// \returns normalized y-coordinate
float TS_NTCU(int x, int w);

/// \brief normalize texture coordinates along the v axis
/// \param y: y-coordinate
/// \param h: height
/// \returns normalized x-coordinate
float TS_NTCV(int y, int h);

/// \brief normalize texture coordinate rectangle
/// \param x: x-coordinate of top left corner
/// \param y: y-coordinate of top left corner
/// \param w: width
/// \param h: height
/// \param w2: new width
/// \param h2: new height
/// \returns rect as 4-array
std::array<float, 4> TS_NTCRect(int x, int y, int w, int h, int w2, int h2);

/// \brief TODO: idk what this does
void TS_Add4Indices();

