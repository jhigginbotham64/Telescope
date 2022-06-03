//
// Copyright 2022 Clemens Cords
// Created on 24.05.22 by clem (mail@clemens-cords.com)
//

#pragma once

#ifdef _cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

// ### TIME ####################################################

double ts_minutes(double n);

double ts_seconds(double n);

double ts_milliseconds(double n);

double ts_microseconds(double n);

double ts_nanoseconds(double n);

size_t ts_clock_create();

void ts_clock_destroy(size_t id);

double ts_clock_elapsed(size_t id);

double ts_clock_restart(size_t id);

// ### WINDOW ##################################################

size_t ts_window_create(
        size_t width,
        size_t height,
        const char* title,
        bool fullscreen,
        bool borderless,
        bool resizable);

void ts_window_destroy(size_t id);

void ts_window_close(size_t id);

bool ts_window_is_open(size_t id);

void ts_window_get_size(size_t window_id, size_t* out_x, size_t* out_y);

void ts_window_get_position(size_t window_id, size_t* out_x, size_t* out_y);

void ts_window_set_hidden(size_t id, bool hidden);

bool ts_window_is_hidden(size_t id);

void ts_window_minimize(size_t id);

bool ts_window_is_minimized(size_t id);

void ts_window_maximize(size_t id);

bool ts_window_is_maximized(size_t id);

bool ts_window_has_focus(size_t id);

bool ts_window_has_mouse_focus(size_t id);

void ts_window_clear(size_t id);

void ts_window_flush(size_t id);

// ### CAMERA ##################################################

void ts_window_camera_center_on(size_t window_id, float x, float y);

void ts_window_camera_move(size_t window_id, float x, float y);

void ts_window_camera_zoom_in(size_t window_id, float factor);

void ts_window_camera_zoom_out(size_t window_id, float factor);

void ts_window_camera_set_zoom(size_t window_id, float factor);

void ts_window_camera_rotate(size_t window_id, float degrees);

void ts_window_camera_set_rotation(size_t window_id, float degrees);

void ts_window_camera_get_center(size_t window_id, float* out_x, float* out_y);

void ts_window_camera_get_view_area(size_t window_id,
                                    float* out_top_left_x, float* out_top_left_y,
                                    float* out_top_right_x, float* out_top_right_y,
                                    float* out_bottom_left_x, float* out_bottom_left_y,
                                    float* out_bottom_right_x, float* out_bottom_right_y);

// ### COMMON ##################################################

bool ts_initialize();

void ts_set_framerate_limit(size_t frames_per_second);

void ts_start_frame(size_t window_id);

void ts_end_frame(size_t window_id);

// ### TEXTURES ##################################################

int32_t ts_texture_filtering_mode_nearest_neighbour();

int32_t ts_texture_filtering_mode_linear();

int32_t ts_texture_filtering_mode_anisotropic();

int32_t ts_texture_blend_mode_none();

int32_t ts_texture_blend_mode_alpha();

int32_t ts_texture_blend_mode_add();

int32_t ts_texture_blend_mode_multiply();

size_t ts_texture_create_static_texture(size_t window_id,
                                        size_t width, size_t height,
                                        float r, float g, float b, float a);

size_t ts_texture_load_static_texture(size_t window_id, const char* path);

void ts_texture_get_size(size_t texture_id, size_t* out_x, size_t* out_y);

void ts_texture_destroy_texture(size_t texture_id);

size_t ts_texture_create_render_texture(size_t window_id, size_t width, size_t height);

// ### SHAPES ##################################################

void ts_shape_render(void* shape_ptr, size_t window_id,
                     float t_00, float t_01, float t_02,
                     float t_10, float t_11, float t_12,
                     float t_20, float t_21, float t_22);

void ts_shape_get_centroid(void* shape_ptr, int* out_x, int* out_y);

void ts_shape_set_centroid(void* shape_ptr, float x, float y);

size_t ts_shape_get_n_vertices(void* shape_ptr);

void ts_shape_get_vertex(void* shape_ptr, size_t vertex_index,
                         float* out_pos_x, float* out_pos_y,
                         float* out_tex_coord_x, float * out_tex_coord_y,
                         float* out_r, float* out_g, float* out_b, float* out_a);

void ts_shape_set_color(void* shape_ptr, float r, float g, float b, float a);

void ts_shape_set_vertex_color(void* shape_ptr, size_t vertex_index, float r, float g, float b, float a);

void ts_shape_get_vertex_color(void* shape_ptr, size_t vertex_index, float* out_r, float* out_g, float* out_b, float* out_a);

void ts_shape_set_vertex_texture_coordinates(void* shape_ptr, size_t vertex_index, float u, float v);

void ts_shape_get_vertex_texture_coordinates(void* shape_ptr, size_t vertex_index, float* out_u, float* out_v);

void ts_shape_set_vertex_position(void* shape_ptr, size_t vertex_index, float x, float y);

void ts_shape_get_vertex_position(void* shape_ptr, size_t vertex_index, float* out_x, float* out_y);

void ts_shape_set_texture(void* shape_ptr, size_t texture_id);

void* ts_shape_new_triangle(float a_x, float a_y, float b_x, float b_y, float c_x, float c_y);

void ts_shape_triangle_get_vertices(void* triangle_ptr,
                                    float* out_a_x, float* out_a_y,
                                    float* out_b_x, float* out_b_y,
                                    float* out_c_x, float* out_c_y);

void ts_shape_destroy_triangle(void* triangle_ptr);

void* ts_shape_new_rectangle(float top_left_x, float top_left_y, float width, float height);

void ts_shape_destroy_rectangle(void* rectangle_ptr);

void* ts_shape_new_circle(float center_x, float center_y, float radius, size_t n_vertices);

float ts_shape_circle_get_radius(void* circle_ptr);

void ts_shape_destroy_circle(void* circle_ptr);

void* ts_shape_new_polygon(float* vertices_x, float* vertices_y, size_t n_vertices);

void ts_shape_destroy_polygon(void* polygon_ptr);

// ### INPUT ###################################################

void ts_input_update();

int64_t ts_keyboard_key(const char* id);

int64_t ts_mouse_button(const char* id);

int64_t ts_controller_button(const char* id);

bool ts_keyboard_is_down(int64_t key);

bool ts_keyboard_has_state_changed(int64_t key);

bool ts_keyboard_was_pressed(int64_t key);

bool ts_keyboard_was_released(int64_t key);

bool ts_mouse_is_down(int64_t button);

bool ts_mouse_has_state_changed(int64_t button);

bool ts_mouse_was_pressed(int64_t button);

bool ts_mouse_was_released(int64_t button);

float ts_mouse_cursor_position_x();

float ts_mouse_cursor_position_y();

float ts_mouse_scrollwheel_x();

float ts_mouse_scrollwheel_y();

bool ts_controller_is_down(int64_t button, size_t controller_id);

bool ts_controller_has_state_changed(int64_t button, size_t controller_id);

bool ts_controller_was_pressed(int64_t button, size_t controller_id);

bool ts_controller_was_released(int64_t button, size_t controller_id);

float ts_controller_axis_left_x(size_t controller_id);

float ts_controller_axis_left_y(size_t controller_id);

float ts_controller_axis_right_x(size_t controller_id);

float ts_controller_axis_right_y(size_t controller_id);

float ts_controller_trigger_left(size_t controller_id);

float ts_controller_trigger_right(size_t controller_id);

// ### MUSIC ###################################################

size_t ts_music_load(const char* path);

void ts_music_unload(size_t id);

void ts_music_play(size_t id, bool should_loop, double fade_in_ms);

void ts_music_play_next(size_t id, bool should_loop, double fade_in_ms);

void ts_music_stop(double fade_out_ms);

void ts_music_next(double fade_out_ms);

void ts_music_clear_next();

size_t ts_music_get_next();

void ts_music_force_stop();

void ts_music_pause();

void ts_music_unpause();

void ts_music_skip_to(int ms);

bool ts_music_is_playing();

bool ts_music_is_paused();

bool ts_music_is_stopped();

void ts_music_set_volume(float zero_to_one);

float ts_music_get_volume();

// ### SOUNDS ##################################################

size_t ts_sound_load(const char* path, float volume);

void ts_sound_unload(size_t id);

size_t ts_sound_n_channels();

void ts_sound_play(size_t id, size_t channel, size_t n_loops, double fade_in_ms);

void ts_sound_stop(size_t channel, double fade_out_ms);

void ts_sound_pause(size_t channel);

void ts_sound_unpause(size_t channel);

void ts_sound_force_stop(size_t channel);

bool ts_sound_is_playing(size_t channel);

bool ts_sound_is_paused(size_t channel);

bool ts_sound_is_stopped(size_t channel);

void ts_sound_set_volume(size_t channel, float zero_to_one);

float ts_sound_get_volume(size_t channel);

void ts_sound_set_panning(size_t channel, size_t zero_to_360_degree);

size_t ts_sound_get_panning(size_t channel);

#ifdef _cplusplus
}
#endif