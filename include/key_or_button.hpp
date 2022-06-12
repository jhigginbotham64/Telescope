// 
// Copyright 2022 Joshua Higginbotham
// Created on 20.05.22 by clem (mail@clemens-cords.com | https://github.com/Clemapfel)
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
        /// `0` or numpad key 0
        NUM_0 = SDLK_0,

        /// `1` or numpad key 1
        NUM_1 = SDLK_1,

        /// `2` or numpad key 2
        NUM_2 = SDLK_2,

        /// `3` or numpad key 3
        NUM_3 = SDLK_3,

        /// `4` or numpad key 4
        NUM_4 = SDLK_4,

        /// `5` or numpad key 5
        NUM_5 = SDLK_5,

        /// `6` or numpad key 6
        NUM_6 = SDLK_6,

        /// `7` or numpad key 7
        NUM_7 = SDLK_7,

        /// `8` or numpad key 8
        NUM_8 = SDLK_8,

        /// `9` or numpad key 9
        NUM_9 = SDLK_9,

        /// `a` or `A` key
        A = SDLK_a,

        /// `b` or `B` key
        B = SDLK_b,

        /// `c` or `C` key
        C = SDLK_c,

        /// `d` or `D` key
        D = SDLK_d,

        /// `e` or `E` key
        E = SDLK_e,

        /// `f` or `F` key
        F = SDLK_f,

        /// `g` or `G` key
        G = SDLK_g,

        /// `h` or `H` key
        H = SDLK_h,

        /// `i` or `I` key
        I = SDLK_i,

        /// `j` or `J` key
        J = SDLK_j,

        /// `k` or `K` key
        K = SDLK_k,

        /// `l` or `L` key
        L = SDLK_l,

        /// `m` or `M` key
        M = SDLK_m,

        /// `n` or `N` key
        N = SDLK_n,

        /// `o` or `O` key
        O = SDLK_o,

        /// `p` or `P` key
        P = SDLK_p,

        /// `q` or `Q` key
        Q = SDLK_q,

        /// `r` or `R` key
        R = SDLK_r,

        /// `s` or `S` key
        S = SDLK_s,

        /// `t` or `T` key
        T = SDLK_t,

        /// `u` or `U` key
        U = SDLK_u,

        /// `v` or `V` key
        V = SDLK_v,

        /// `w` or `W` key
        W = SDLK_w,

        /// `x` or `X` key
        X = SDLK_x,

        /// `y` or `Y` key
        Y = SDLK_y,

        /// `z` or `Z` key
        Z = SDLK_z,

        /// left-most `alt` key
        ALT = SDLK_LALT,

        /// right-most `alt` key
        RIGHT_ALT = SDLK_RALT,

        /// left-most `ctrl` key
        CTRL = SDLK_LCTRL,

        /// right-most `ctrl` key
        RIGHT_CTRL = SDLK_RCTRL,

        /// caps lock key
        CAPS = SDLK_CAPSLOCK,

        /// left-most shift key
        SHIFT = SDLK_LSHIFT,

        /// right-most shift key
        RIGHT_SHIFT = SDLK_RSHIFT,

        /// `[` key
        LEFT_BRACKET = SDLK_LEFTBRACKET,

        /// `]` key
        RIGHT_BRACKET = SDLK_RIGHTBRACKET,

        /// `/` key
        SLASH = SDLK_SLASH,

        /// `\` key
        BACKSLASH = SDLK_BACKSLASH,

        /// up-arrow key
        UP = SDLK_UP,

        /// down-arrow key
        DOWN = SDLK_DOWN,

        /// left-arrow key
        LEFT = SDLK_LEFT,

        /// right-arrow key
        RIGHT = SDLK_RIGHT,

        /// enter or return key
        RETURN = SDLK_RETURN,

        /// `esc` key
        ESCAPE = SDLK_ESCAPE,

        /// backspace key
        BACKSPACE = SDLK_BACKSPACE,

        /// tab key
        TAB = SDLK_TAB,

        /// space bar
        SPACE = SDLK_SPACE,

        /// `#` key
        HASH = SDLK_HASH,

        /// '%' key
        PERCENT = SDLK_PERCENT,

        /// '$' key
        DOLLAR = SDLK_DOLLAR,

        /// '&' key
        AND = SDLK_AMPERSAND,

        /// '"' key
        QUOTE = SDLK_QUOTE,

        /// `(` key
        LEFT_PARENTHESIS = SDLK_LEFTPAREN,

        /// `)` Key
        RIGHT_PARENTHESIS= SDLK_RIGHTPAREN,

        /// `*` key
        ASTERISK = SDLK_ASTERISK,

        /// `+` key
        PLUS = SDLK_PLUS,

        /// `-` key
        MINUS = SDLK_MINUS,

        /// `_` key
        UNDERSCORE = SDLK_UNDERSCORE,

        /// `,` key
        COMMA = SDLK_COMMA,

        /// `.` key
        PERIOD = SDLK_PERIOD,

        /// `;` key
        SEMICOLON = SDLK_SEMICOLON,

        /// `:` key
        COLON = SDLK_COLON,

        /// `<` key
        LESS_THAN = SDLK_LESS,

        /// `>` key
        GREATER_THAN = SDLK_GREATER,

        /// `=` key
        EQUALS = SDLK_EQUALS,

        /// `^` key
        CIRCUMFLEX = SDLK_CARET,

        /// `!` key
        EXCLAMATION_MARK = SDLK_EXCLAIM,

        /// `?` key
        QUESTION_MARK = SDLK_QUESTION,

        /// `F1` key
        F1 = SDLK_F1,

        /// `F2` key
        F2 = SDLK_F2,

        /// `F3` key
        F3 = SDLK_F3,

        /// `F4` key
        F4 = SDLK_F4,

        /// `F5` key
        F5 = SDLK_F5,

        /// `F6` key
        F6 = SDLK_F6,

        /// `F7` key
        F7 = SDLK_F7,

        /// `F8` key
        F8 = SDLK_F8,

        /// `F9` key
        F9 = SDLK_F9,

        /// `F10` key
        F10 = SDLK_F10,

        /// `F11` key
        F11 = SDLK_F11,

        /// `F12` key
        F12 = SDLK_F12,

        /// mute audio key, usually `🔇`
        AUDIO_MUTE = SDLK_AUDIOMUTE,

        /// stop playback key, usually `⏸`
        AUDIO_STOP = SDLK_AUDIOSTOP,

        /// start playback key, usually `⏵`
        AUDIO_PLAY = SDLK_AUDIOPLAY,

        /// next song key, usually `⏭`
        AUDIO_NEXT = SDLK_AUDIONEXT,

        // previous song key, usually `⏮`
        AUDIO_PREV = SDLK_AUDIOPREV,

        /// volunme up key, usually `🔊`
        VOLUME_UP = SDLK_VOLUMEUP,

        /// volume down key, usually `🔉`
        VOLUME_DOWN = SDLK_VOLUMEDOWN,

        /// `insert` key
        INSERT = SDLK_INSERT,

        /// `print scr` key
        PRINTSCREEN = SDLK_PRINTSCREEN,

        /// `delete` key
        DELETE = SDLK_DELETE,

        /// key that is not any of the above
        KEY_UNKNOWN = SDLK_UNKNOWN
    };

    /// \brief enum of mouse buttons
    enum MouseButton : int64_t
    {
        /// left mouse button
        MOUSE_LEFT = SDL_BUTTON_LEFT,

        /// right mouse button
        MOUSE_RIGHT = SDL_BUTTON_RIGHT,

        /// click scrollwheel in button
        MOUSE_MIDDLE = SDL_BUTTON_MIDDLE,

        /// additional mouse button 1
        MOUSE_X1 = SDL_BUTTON_X1,

        /// additional mouse button 2
        MOUSE_X2 = SDL_BUTTON_X2
    };

    //   xbox          playstation       nintendo
    //
    //     y            triangle           x
    // x       b | square      circle | y      a
    //     a               x                b


    /// \brief enum of controller buttons
    enum ControllerButton : int64_t
    {
        /// xbox: `A`, playstation: `X`, nintendo: `B`
        CONTROLLER_BUTTON_A = SDL_CONTROLLER_BUTTON_A,

        /// xbox: `B`, playstation: `◯`, nintendo: `A`
        CONTROLLER_BUTTON_B = SDL_CONTROLLER_BUTTON_B,

        /// xbox: `X`, playstation: `□`, nintendo: `Y`
        CONTROLLER_BUTTON_X = SDL_CONTROLLER_BUTTON_X,

        /// xbox: `Y`, playstation: `△`, nintendo: `X`
        CONTROLLER_BUTTON_Y = SDL_CONTROLLER_BUTTON_Y,

        /// pressing in left analogue joystick
        CONTROLLER_LEFTSTICK_PRESS = SDL_CONTROLLER_BUTTON_LEFTSTICK,

        /// pressing in right analogue joystick
        CONTROLLER_RIGHTSTICK_PRESS = SDL_CONTROLLER_BUTTON_RIGHTSTICK,

        /// left non-analogue shoulder button, usually `lb`
        CONTROLLER_LEFTSHOULDER = SDL_CONTROLLER_BUTTON_LEFTSHOULDER,

        /// right non-analogue shoulder button, usually `rb`
        CONTROLLER_RIGHTSHOULDER = SDL_CONTROLLER_BUTTON_RIGHTSHOULDER,

        /// analogue dpad up
        CONTROLLER_DPAD_UP = SDL_CONTROLLER_BUTTON_DPAD_UP,

        /// analogue dpad down
        CONTROLLER_DPAD_DOWN = SDL_CONTROLLER_BUTTON_DPAD_DOWN,

        /// analogue dpad left
        CONTROLLER_DPAD_LEFT = SDL_CONTROLLER_BUTTON_DPAD_LEFT,

        /// analogue dpad right
        CONTROLLER_DPAD_RIGHT = SDL_CONTROLLER_BUTTON_DPAD_RIGHT,

        /// xbox: back, playstation: select, nintendo: select or minus
        CONTROLLER_SELECT = SDL_CONTROLLER_BUTTON_BACK,

        /// xbox: start, playstation: start, nintendo: start or plus
        CONTROLLER_START = SDL_CONTROLLER_BUTTON_START,

        /// xbox: share, playstation: mic, nintendo: capture
        CONTROLLER_XBOX_X_SHARE = SDL_CONTROLLER_BUTTON_MISC1,

        /// identical to above
        CONTROLLER_PS5_MIC = SDL_CONTROLLER_BUTTON_MISC1,

        /// identical to above
        CONTROLLER_SWITCH_CAPTURE = SDL_CONTROLLER_BUTTON_MISC1,

        /// paddles for pro controller first left
        CONTROLLER_PADDLE_01 = SDL_CONTROLLER_BUTTON_PADDLE1,

        /// paddles for pro controller second left
        CONTROLLER_PADDLE_02 = SDL_CONTROLLER_BUTTON_PADDLE2,

        /// paddles for pro controller first right
        CONTROLLER_PADDLE_03 = SDL_CONTROLLER_BUTTON_PADDLE3,

        /// paddles for pro controller second right
        CONTROLLER_PADDLE_04 = SDL_CONTROLLER_BUTTON_PADDLE4,

        /// playstation: press in touchpad
        CONTROLLER_PS5_TOUCHPAD = SDL_CONTROLLER_BUTTON_TOUCHPAD,

        /// controller button that is none of the above
        CONTROLLER_BUTTON_UNKNOWN = SDL_CONTROLLER_BUTTON_INVALID
    };

    namespace detail
    {
        KeyboardKey string_to_keyboard_key(const std::string&);
        MouseButton string_to_mouse_button(const std::string&);
        ControllerButton string_to_controller_button(const std::string&);
    }
}