// 
// Copyright 2022 Joshua Higginbotham
// Created on 20.05.22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <SDL2/SDL_gamecontroller.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_mouse.h>

#include <string>
#include <cinttypes>

namespace ts
{
    /// \brief enum of keyboard keys
    enum KeyboardKey : int64_t
    {
        KEY_UNKNOWN = SDLK_UNKNOWN,

        NUM_0 = SDLK_0,
        NUM_1 = SDLK_1,
        NUM_2 = SDLK_2,
        NUM_3 = SDLK_3,
        NUM_4 = SDLK_4,
        NUM_5 = SDLK_5,
        NUM_6 = SDLK_6,
        NUM_7 = SDLK_7,
        NUM_8 = SDLK_8,
        NUM_9 = SDLK_9,

        A = SDLK_a,
        B = SDLK_b,
        C = SDLK_c,
        D = SDLK_d,
        E = SDLK_e,
        F = SDLK_f,
        G = SDLK_g,
        H = SDLK_h,
        I = SDLK_i,
        J = SDLK_j,
        K = SDLK_k,
        L = SDLK_l,
        M = SDLK_m,
        N = SDLK_n,
        O = SDLK_o,
        P = SDLK_p,
        Q = SDLK_q,
        R = SDLK_r,
        S = SDLK_s,
        T = SDLK_t,
        U = SDLK_u,
        V = SDLK_v,
        W = SDLK_w,
        X = SDLK_x,
        Y = SDLK_y,
        Z = SDLK_z,

        ALT = SDLK_LALT,
        RIGHT_ALT = SDLK_RALT,

        CTRL = SDLK_LCTRL,
        RIGHT_CTRL = SDLK_RCTRL,
        CAPS = SDLK_CAPSLOCK,

        SHIFT = SDLK_LSHIFT,
        RIGHT_SHIFT = SDLK_RSHIFT,

        BRACKET_LEFT = SDLK_LEFTBRACKET,
        BRACKET_RIGHT = SDLK_RIGHTBRACKET,

        SLASH = SDLK_SLASH,
        BACKSLASH = SDLK_BACKSLASH,

        UP = SDLK_UP,
        DOWN = SDLK_DOWN,
        LEFT = SDLK_LEFT,
        RIGHT = SDLK_RIGHT,

        RETURN = SDLK_RETURN,
        ESCAPE = SDLK_ESCAPE,
        BACKSPACE = SDLK_BACKSPACE,
        TAB = SDLK_TAB,
        SPACE = SDLK_SPACE,

        HASH = SDLK_HASH,
        PERCENT = SDLK_PERCENT,
        DOLLAR = SDLK_DOLLAR,
        AND = SDLK_AMPERSAND,
        QUOTE = SDLK_QUOTE,

        LEFT_PARENTHESIS = SDLK_LEFTPAREN,
        RIGHT_PARENTHESIS= SDLK_RIGHTPAREN,

        ASTERISK = SDLK_ASTERISK,
        PLUS = SDLK_PLUS,
        MINUS = SDLK_MINUS,
        UNDERSCORE = SDLK_UNDERSCORE,

        COMMA = SDLK_COMMA,
        PERIOD = SDLK_PERIOD,
        SEMICOLON = SDLK_SEMICOLON,
        COLON = SDLK_COLON,

        LESS_THAN = SDLK_LESS,
        GREATER_THAN = SDLK_GREATER,

        EQUALS = SDLK_EQUALS,
        CIRCUMFLEX = SDLK_CARET,

        EXCLAMATION_MARK = SDLK_EXCLAIM,
        QUESTION_MARK = SDLK_QUESTION,

        F1 = SDLK_F1,
        F2 = SDLK_F2,
        F3 = SDLK_F3,
        F4 = SDLK_F4,
        F5 = SDLK_F5,
        F6 = SDLK_F6,
        F7 = SDLK_F7,
        F8 = SDLK_F8,
        F9 = SDLK_F9,
        F10 = SDLK_F10,
        F11 = SDLK_F11,
        F12 = SDLK_F12,

        AUDIO_MUTE = SDLK_AUDIOMUTE,
        AUDIO_STOP = SDLK_AUDIOSTOP,
        AUDIO_PLAY = SDLK_AUDIOPLAY,
        AUDIO_NEXT = SDLK_AUDIONEXT,
        AUDIO_PREV = SDLK_AUDIOPREV,
        VOLUME_UP = SDLK_VOLUMEUP,
        VOLUME_DOWN = SDLK_VOLUMEDOWN,

        INSERT = SDLK_INSERT,
        PRINTSCREEN = SDLK_PRINTSCREEN,
        DELETE = SDLK_DELETE,
    };

    /// \brief enum of mouse buttons
    enum MouseButton : int64_t
    {
        MOUSE_LEFT = SDL_BUTTON_LEFT,
        MOUSE_RIGHT = SDL_BUTTON_RIGHT,
        MOUSE_MIDDLE = SDL_BUTTON_MIDDLE,

        MOUSE_X1 = SDL_BUTTON_X1,
        MOUSE_X2 = SDL_BUTTON_X2,
    };

    /// \brief enum of controller buttons
    enum ControllerButton : int64_t
    {
        CONTROLLER_BUTTON_UNKNOWN = SDL_CONTROLLER_BUTTON_INVALID,

        CONTROLLER_BUTTON_A = SDL_CONTROLLER_BUTTON_A,
        CONTROLLER_BUTTON_B = SDL_CONTROLLER_BUTTON_B,
        CONTROLLER_BUTTON_X = SDL_CONTROLLER_BUTTON_X,
        CONTROLLER_BUTTON_Y = SDL_CONTROLLER_BUTTON_Y,

        CONTROLLER_LEFTSTICK_PRESS = SDL_CONTROLLER_BUTTON_LEFTSTICK,
        CONTROLLER_RIGHTSTICK_PRESS = SDL_CONTROLLER_BUTTON_RIGHTSTICK,
        CONTROLLER_LEFTSHOULDER = SDL_CONTROLLER_BUTTON_LEFTSHOULDER,
        CONTROLLER_RIGHTSHOULDER = SDL_CONTROLLER_BUTTON_RIGHTSHOULDER,

        CONTROLLER_DPAD_UP = SDL_CONTROLLER_BUTTON_DPAD_UP,
        CONTROLLER_DPAD_DOWN = SDL_CONTROLLER_BUTTON_DPAD_DOWN,
        CONTROLLER_DPAD_LEFT = SDL_CONTROLLER_BUTTON_DPAD_LEFT,
        CONTROLLER_DPAD_RIGHT = SDL_CONTROLLER_BUTTON_DPAD_RIGHT,

        CONTROLLER_SELECT = SDL_CONTROLLER_BUTTON_BACK, //TODO verify this
        CONTROLLER_START = SDL_CONTROLLER_BUTTON_START,

        CONTROLLER_GUIDE = SDL_CONTROLLER_BUTTON_GUIDE,
        CONTROLLER_MAX = SDL_CONTROLLER_BUTTON_MAX,

        CONTROLLER_XBOX_X_SHARE = SDL_CONTROLLER_BUTTON_MISC1,
        CONTROLLER_PS5_MIC = SDL_CONTROLLER_BUTTON_MISC1,
        CONTROLLER_SWITCH_CAPTURE = SDL_CONTROLLER_BUTTON_MISC1,

        CONTROLLER_PADDLE_01 = SDL_CONTROLLER_BUTTON_PADDLE1,
        CONTROLLER_PADDLE_02 = SDL_CONTROLLER_BUTTON_PADDLE2,
        CONTROLLER_PADDLE_03 = SDL_CONTROLLER_BUTTON_PADDLE3,
        CONTROLLER_PADDLE_04 = SDL_CONTROLLER_BUTTON_PADDLE4,

        CONTROLLER_PS5_TOUCHPAD = SDL_CONTROLLER_BUTTON_TOUCHPAD
    };

    namespace detail
    {
        KeyboardKey string_to_keyboard_key(const std::string&);
        MouseButton string_to_mouse_button(const std::string&);
        ControllerButton string_to_controller_button(const std::string&);
    }
}