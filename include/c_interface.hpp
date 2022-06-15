//
// Copyright 2022 Joshua Higginbotham
// Created on 24.05.22 by clem (mail@clemens-cords.com | https://github.com/Clemapfel)
//

#pragma once

#ifdef _cplusplus
extern "C"
{
#endif

// NOTE
//
// This file is **not intended for end-users**. No function in this file
// should be called during normal usage of telescope. Misuse of these
// functions will trigger silent data corruption and memory leaks.
// Only proceed if you're a developer of telescope and know
// what you are doing
//

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

// ### TIME ####################################################

double ts_ns_to_minutes(size_t n);

double ts_ns_to_seconds(size_t n);

double ts_ns_to_milliseconds(size_t n);

double ts_ns_to_microseconds(size_t n);

size_t ts_ns_to_nanoseconds(size_t n);

size_t ts_minutes_to_ns(double n);

size_t ts_seconds_to_ns(double n);

size_t ts_milliseconds_to_ns(double n);

size_t ts_microseconds_to_ns(double n);

size_t ts_nanoseconds_to_ns(size_t n);

size_t ts_clock_create();

void ts_clock_destroy(size_t id);

double ts_clock_elapsed(size_t id);

double ts_clock_restart(size_t id);

// ### COLORS ##################################################

void ts_rgb_to_hsv(float r, float g, float b, float* out_h, float* out_s, float* out_v);

void ts_hsv_to_rgb(float h, float s, float v, float* out_r, float* out_g, float* out_b);

// ### ANGLE ##################################################

float ts_degrees_to_radians(float degrees);

float ts_radians_to_degrees(float radians);

// ### WINDOW ##################################################

size_t ts_window_create(const char* title, size_t width, size_t height, uint32_t options);

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

void ts_window_render(size_t id, void* renderable_ptr, void* transform_ptr);

void ts_window_clear(size_t id);

void ts_window_flush(size_t id);

void ts_window_set_icon(size_t id, const char* path);

// ### CAMERA ##################################################

void ts_window_camera_center_on(size_t window_id, float x, float y);

void ts_window_camera_move(size_t window_id, float x, float y);

void ts_window_camera_zoom_in(size_t window_id, float factor);

void ts_window_camera_zoom_out(size_t window_id, float factor);

void ts_window_camera_set_zoom(size_t window_id, float factor);

void ts_window_camera_rotate(size_t window_id, float degrees);

void* ts_window_camera_get_transform(size_t window_id);

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

double ts_start_frame(size_t window_id);

void ts_end_frame(size_t window_id);

// ### TEXTURES ##################################################

int32_t ts_texture_filtering_mode_nearest_neighbour();

int32_t ts_texture_filtering_mode_linear();

int32_t ts_texture_filtering_mode_anisotropic();

int32_t ts_texture_blend_mode_none();

int32_t ts_texture_blend_mode_alpha();

int32_t ts_texture_blend_mode_add();

int32_t ts_texture_blend_mode_multiply();

void ts_texture_set_blend_mode(void* texture, int32_t blend_mode);

int32_t ts_texture_get_blend_mode(void* texture);

void ts_texture_set_filtering_mode(void* texture, int32_t filtering_mode);

int32_t ts_texture_get_filtering_mode(void* texture);

void* ts_texture_create_static_texture(size_t window_id,
                                        size_t width, size_t height,
                                        float r, float g, float b, float a);

void* ts_texture_load_static_texture(size_t window_id, const char* path);

void ts_texture_get_size(void* texture, size_t* out_x, size_t* out_y);

void ts_texture_set_color(void* texture, float r, float g, float b, float a);

void ts_texture_get_color(void* texture, float* out_r, float* out_g, float* out_b, float* out_a);

void ts_texture_destroy_texture(void* texture);

void* ts_texture_create_render_texture(size_t window_id, size_t width, size_t height);

// ### TRANSFORMS ##################################################

void* ts_transform_create();

void ts_transform_destroy(void* transform_ptr);

void ts_transform_set(void* transform_ptr, size_t x, size_t y, float value);

float ts_transform_get(void* transform_ptr, size_t x, size_t y);

void ts_transform_apply_to(void* transform_ptr, float x, float y, float* out_x, float* out_y);

void ts_transform_reset(void* transform_ptr);

void ts_transform_combine(void* left, void* right);

void ts_transform_translate(void* transform_ptr, float x, float y);

void ts_transform_rotate(void* transform_ptr, float degrees, float origin_x, float origin_y);

void ts_transform_scale(void* transform_ptr, float x_scale, float y_scale);

void ts_transform_shear(void* transform_ptr, float x_scale, float y_scale);

void ts_transform_reflect(void* transform_ptr, bool about_x_axis, bool about_y_axis, float origin_x, float origin_y);

// ### SHAPES ##################################################

void ts_shape_render_to_window(void* shape_ptr, size_t window_id, void* transform_ptr);

void ts_shape_render_to_texture(void* shape_ptr, void* render_texture_ptr, void* transform_ptr);

void ts_shape_get_centroid(void* shape_ptr, int* out_x, int* out_y);

void ts_shape_set_centroid(void* shape_ptr, float x, float y);

void ts_shape_move(void* shape_ptr, float x, float y);

size_t ts_shape_get_n_vertices(void* shape_ptr);

void ts_shape_set_color(void* shape_ptr, float r, float g, float b, float a);

void ts_shape_set_vertex_color(void* shape_ptr, size_t vertex_index, float r, float g, float b, float a);

void ts_shape_get_vertex_color(void* shape_ptr, size_t vertex_index, float* out_r, float* out_g, float* out_b, float* out_a);

void ts_shape_set_vertex_texture_coordinates(void* shape_ptr, size_t vertex_index, float u, float v);

void ts_shape_get_vertex_texture_coordinates(void* shape_ptr, size_t vertex_index, float* out_u, float* out_v);

void ts_shape_set_vertex_position(void* shape_ptr, size_t vertex_index, float x, float y);

void ts_shape_get_vertex_position(void* shape_ptr, size_t vertex_index, float* out_x, float* out_y);

void ts_shape_set_texture(void* shape_ptr, void* texture);

void* ts_shape_get_texture(void* shape_ptr);

void ts_shape_set_texture_rectangle(void* shape_ptr, float top_left_x, float top_left_y, float width, float height);

void ts_shape_get_texture_rectangle(void* shape_ptr, float* out_top_left_x, float* out_top_left_y, float* out_width, float* out_height);

void ts_shape_get_bounding_box(void* shape_ptr, float* out_top_left_x, float* out_top_left_y, float* out_width, float* out_height);

void* ts_shape_create_triangle(float a_x, float a_y, float b_x, float b_y, float c_x, float c_y);

void ts_shape_triangle_get_vertices(void* triangle_ptr,
                                    float* out_a_x, float* out_a_y,
                                    float* out_b_x, float* out_b_y,
                                    float* out_c_x, float* out_c_y);

void ts_shape_destroy_triangle(void* triangle_ptr);

void* ts_shape_create_rectangle(float top_left_x, float top_left_y, float width, float height);

void ts_shape_destroy_rectangle(void* rectangle_ptr);

void ts_shape_rectangle_set_top_left(void* rectangle_ptr, float top_left_x, float top_left_y);

void ts_shape_rectangle_get_top_left(void* rectangle_ptr, float* out_top_left_x, float* out_top_left_y);

void ts_shape_rectangle_set_size(void* rectangle_ptr, float width, float height);

void ts_shape_rectangle_get_size(void* rectangle_ptr, float* out_width, float* out_height);

void* ts_shape_create_circle(float center_x, float center_y, float radius, size_t n_vertices);

void ts_shape_destroy_circle(void* circle_ptr);

float ts_shape_circle_get_radius(void* circle_ptr);

void ts_shape_circle_set_radius(void* circle_ptr, float);

void* ts_shape_create_polygon(float* vertices_x, float* vertices_y, size_t n_vertices);

void ts_shape_destroy_polygon(void* polygon_ptr);

// ### COLLISION ###################################################

size_t ts_physics_world_create();

void ts_physics_world_destroy(size_t world_id);

void ts_physics_world_step(size_t world_id, float time_ms, int32_t velocity_iterations, int32_t position_iterations);

void ts_physics_world_clear_forces(size_t world);

void ts_physics_world_set_gravity(size_t world_id, float x, float y);

void ts_physics_world_get_gravity(size_t world_id, float* out_x, float* out_y);

void ts_physics_world_distance_between(size_t world_id, void* shape_a, void* shape_b, float* out_distance, float* out_point_a_x, float* out_point_a_y, float* out_point_b_x, float* out_point_b_y);

bool ts_physics_world_is_point_in_shape(size_t world_id, void* shape, float point_x, float point_y);

bool ts_physics_world_ray_cast(
        size_t world_id, void* shape,
        float ray_start_x, float ray_start_y, float ray_end_x, float ray_end_y,
        float* out_normal_x, float* out_normal_y,
        float* out_hit_x, float* out_hit_y);

bool ts_physics_world_next_event(size_t world_id, int* out_event_type, size_t* out_shape_a_id, size_t* out_shape_b_id);

size_t ts_collision_type_static();
size_t ts_collision_type_dynamic();
size_t ts_collision_type_kinematic();

void* ts_collision_circle_create(size_t world_id, size_t type, float center_x, float center_y, float radius);

void* ts_collision_rectangle_create(size_t world_id, size_t type, float top_left_x, float top_left_y, float width, float height);

void* ts_collision_triangle_create(size_t world_id, size_t type, float a_x, float a_y, float b_x, float b_y, float c_x, float c_y);

void* ts_collision_polygon_create(size_t world_id, size_t type, float* xs, float* ys, size_t n_vertices);

void* ts_collision_wire_frame_create(size_t world_id, size_t type, float* xs, float* ys, size_t n_vertices);

void* ts_collision_line_create(size_t world_id, size_t type, float a_x, float a_y, float b_x, float b_y, bool two_sided);

void ts_collision_shape_destroy(void* shape);

void ts_collision_shape_set_density(void* shape, float);

float ts_collision_shape_get_density(void* shape);

void ts_collision_shape_set_friction(void* shape, float);

float ts_collision_shape_get_friction(void* shape);

void ts_collision_shape_set_restitution(void* shape, float);

float ts_collision_shape_get_restitution(void* shape);

void ts_collision_shape_get_centroid(void* shape, float* out_x, float* out_y);

void ts_collision_shape_get_bounding_box(void* shape, float* out_x, float* out_y, float* out_width, float* out_height);

float ts_collision_shape_get_rotation(void* shape);

void ts_collision_shape_set_type(void* shape, size_t);

size_t ts_collision_shape_get_type(void* shape);

void ts_collision_shape_enable(void*);

void ts_collision_shape_disable(void*);

bool ts_collision_shape_is_enabled(void*);

void ts_collision_shape_get_origin(void*, float* out_x, float* out_y);

void ts_collision_shape_get_center_of_mass_global(void*, float* out_x, float* out_y);

void ts_collision_shape_get_center_of_mass_local(void*, float* out_x, float* out_y);

void ts_collision_shape_set_linear_velocity(void*, float x, float y);

void ts_collision_shape_set_linear_velocity(void*, float* out_x, float* out_y);

void ts_collision_shape_set_angular_velocity(void*, float);

float ts_collision_shape_get_angular_velocity(void*);

void ts_collision_shape_apply_force_to(void*, float force_x, float force_y, float point_x, float point_y);

void ts_collision_shape_apply_force_to_center(void*, float force_x, float force_y);

void ts_collision_shape_apply_torque(void*, float torque);

void ts_collision_shape_apply_linear_impulse_to(void*, float impulse_x, float impulse_y, float point_x, float point_y);

void ts_collision_shape_apply_linear_impulse_to_center(void*, float force_x, float force_y);

float ts_collision_shape_get_mass(void*);

float ts_collision_shape_get_inertia(void*);

void ts_collision_shape_set_is_bullet(void*, bool);

bool ts_collision_shape_get_is_bullet(void*);

bool ts_collision_shape_get_is_rotation_fixed(void*);

void ts_collision_shape_set_is_rotation_fixed(void*, bool);

size_t ts_collision_shape_get_id(void*);

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

void ts_mouse_cursor_position(float*, float*);

void ts_mouse_scrollwheel(float*, float*);

bool ts_controller_is_down(int64_t button, size_t controller_id);

bool ts_controller_has_state_changed(int64_t button, size_t controller_id);

bool ts_controller_was_pressed(int64_t button, size_t controller_id);

bool ts_controller_was_released(int64_t button, size_t controller_id);

float ts_controller_axis_left(size_t controller_id, float*, float*);

float ts_controller_axis_right(size_t controller_id, float*, float*);

float ts_controller_trigger_left(size_t controller_id);

float ts_controller_trigger_right(size_t controller_id);

// ### MUSIC ###################################################

size_t ts_music_sample_rate();

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

void ts_music_skip_to(double ms);

bool ts_music_is_playing();

bool ts_music_is_paused();

bool ts_music_is_stopped();

void ts_music_set_volume(float zero_to_one);

float ts_music_get_volume();

// ### SOUNDS ##################################################

size_t ts_sound_get_max_n_channels();

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

void ts_sound_set_panning(size_t channel, float zero_to_360_degree);

float ts_sound_get_panning(size_t channel);

#ifdef _cplusplus
}
#endif