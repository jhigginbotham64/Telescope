// 
// Copyright 2022 Joshua Higginbotham
// Created on 24.05.22 by clem (mail@clemens-cords.com | https://github.com/Clemapfel)
//

#include <exception>
#include <sstream>

#include <include/key_or_button.hpp>

namespace ts::detail
{
    KeyboardKey string_to_keyboard_key(const std::string& str)
    {
        if (str == "NUM_0") return NUM_0;
        if (str == "NUM_1") return NUM_1;
        if (str == "NUM_2") return NUM_2;
        if (str == "NUM_3") return NUM_3;
        if (str == "NUM_4") return NUM_4;
        if (str == "NUM_5") return NUM_5;
        if (str == "NUM_6") return NUM_6;
        if (str == "NUM_7") return NUM_7;
        if (str == "NUM_8") return NUM_8;
        if (str == "NUM_9") return NUM_9;

        if (str == "A" or str == "a") return A;
        if (str == "B" or str == "b") return B;
        if (str == "C" or str == "c") return C;
        if (str == "D" or str == "d") return D;
        if (str == "E" or str == "e") return E;
        if (str == "F" or str == "f") return F;
        if (str == "G" or str == "g") return G;
        if (str == "H" or str == "h") return H;
        if (str == "I" or str == "i") return I;
        if (str == "J" or str == "j") return J;
        if (str == "K" or str == "k") return K;
        if (str == "L" or str == "l") return L;
        if (str == "M" or str == "m") return M;
        if (str == "N" or str == "n") return N;
        if (str == "O" or str == "o") return O;
        if (str == "P" or str == "p") return P;
        if (str == "Q" or str == "q") return Q;
        if (str == "R" or str == "r") return R;
        if (str == "S" or str == "s") return S;
        if (str == "T" or str == "t") return T;
        if (str == "U" or str == "u") return U;
        if (str == "V" or str == "v") return V;
        if (str == "W" or str == "w") return W;
        if (str == "X" or str == "x") return X;
        if (str == "Y" or str == "y") return Y;
        if (str == "Z" or str == "z") return Z;

        if (str == "ALT") return ALT;
        if (str == "RIGHT_ALT") return RIGHT_ALT;

        if (str == "CTRL") return CTRL;
        if (str == "RIGHT_CTRL") return RIGHT_CTRL;
        if (str == "CAPS") return CAPS;

        if (str == "SHIFT") return SHIFT;
        if (str == "RIGHT_SHIFT") return RIGHT_SHIFT;

        if (str == "BRACKET_LEFT") return BRACKET_LEFT;
        if (str == "BRACKET_RIGHT") return BRACKET_RIGHT;

        if (str == "SLASH") return SLASH;
        if (str == "BACKSLASH") return BACKSLASH;

        if (str == "UP") return UP;
        if (str == "DOWN") return DOWN;
        if (str == "LEFT") return LEFT;
        if (str == "RIGHT") return RIGHT;

        if (str == "RETURN") return RETURN;
        if (str == "ESCAPE") return ESCAPE;
        if (str == "BACKSPACE") return BACKSPACE;
        if (str == "TAB") return TAB;
        if (str == "SPACE") return SPACE;

        if (str == "HASH") return HASH;
        if (str == "PERCENT") return PERCENT;
        if (str == "DOLLAR") return DOLLAR;
        if (str == "AND") return AND;
        if (str == "QUOTE") return QUOTE;

        if (str == "LEFT_PARENTHESIS") return LEFT_PARENTHESIS;
        if (str == "RIGHT_PARENTHESIS") return RIGHT_PARENTHESIS;

        if (str == "ASTERISK") return ASTERISK;
        if (str == "PLUS") return PLUS;
        if (str == "MINUS") return MINUS;
        if (str == "UNDERSCORE") return UNDERSCORE;

        if (str == "COMMA") return COMMA;
        if (str == "PERIOD") return PERIOD;
        if (str == "SEMICOLON") return SEMICOLON;
        if (str == "COLON") return COLON;

        if (str == "LESS_THAN") return LESS_THAN;
        if (str == "GREATER_THAN") return GREATER_THAN;

        if (str == "EQUALS") return EQUALS;
        if (str == "CIRCUMFLEX") return CIRCUMFLEX;

        if (str == "EXCLAMATION_MARK") return EXCLAMATION_MARK;
        if (str == "QUESTION_MARK") return QUESTION_MARK;

        if (str == "INSERT") return INSERT;
        if (str == "PRINTSCREEN") return PRINTSCREEN;
        if (str == "DELETE") return DELETE;

        if (str == "F1") return F1;
        if (str == "F2") return F2;
        if (str == "F3") return F3;
        if (str == "F4") return F4;
        if (str == "F5") return F5;
        if (str == "F6") return F6;
        if (str == "F7") return F7;
        if (str == "F8") return F8;
        if (str == "F9") return F9;
        if (str == "F10") return F10;
        if (str == "F11") return F11;
        if (str == "F12") return F12;

        if (str == "AUDIO_MUTE") return AUDIO_MUTE;
        if (str == "AUDIO_STOP") return AUDIO_STOP;
        if (str == "AUDIO_PLAY") return AUDIO_PLAY;
        if (str == "AUDIO_NEXT") return AUDIO_NEXT;
        if (str == "AUDIO_PREV") return AUDIO_PREV;
        if (str == "VOLUME_UP") return VOLUME_UP;
        if (str == "VOLUME_DOWN") return VOLUME_DOWN;

        if (str == "KEY_UNKNOWN") return KEY_UNKNOWN;

        std::stringstream stream;
        stream << "In string_to_keyboard_key: Key with id \"" << str << "\" unrecognized" << std::endl;
        throw std::invalid_argument(stream.str());
        return KEY_UNKNOWN;
    }

    MouseButton string_to_mouse_button(const std::string& str)
    {
        if (str == "MOUSE_LEFT") return MOUSE_LEFT;
        if (str == "MOUSE_RIGHT") return MOUSE_RIGHT;
        if (str == "MOUSE_MIDDLE") return MOUSE_MIDDLE;
        if (str == "MOUSE_X1") return MOUSE_X1;
        if (str == "MOUSE_X2") return MOUSE_X2;

        std::stringstream stream;
        stream << "In string_to_mouse_button: Button with id \"" << str << "\" unrecognized" << std::endl;
        throw std::invalid_argument(stream.str());
        return MOUSE_LEFT;
    }

    ControllerButton string_to_controller_button(const std::string& str)
    {
        if (str == "CONTROLLER_BUTTON_UNKNOWN") return CONTROLLER_BUTTON_UNKNOWN;

        if (str == "CONTROLLER_BUTTON_A") return CONTROLLER_BUTTON_A;
        if (str == "CONTROLLER_BUTTON_B") return CONTROLLER_BUTTON_B;
        if (str == "CONTROLLER_BUTTON_X") return CONTROLLER_BUTTON_X;
        if (str == "CONTROLLER_BUTTON_Y") return CONTROLLER_BUTTON_Y;

        if (str == "CONTROLLER_LEFTSTICK_PRESS") return CONTROLLER_LEFTSTICK_PRESS;
        if (str == "CONTROLLER_RIGHTSTICK_PRESS") return CONTROLLER_RIGHTSTICK_PRESS;
        if (str == "CONTROLLER_LEFTSHOULDER") return CONTROLLER_LEFTSHOULDER;
        if (str == "CONTROLLER_RIGHTSHOULDER") return CONTROLLER_RIGHTSHOULDER;

        if (str == "CONTROLLER_DPAD_UP") return CONTROLLER_DPAD_UP;
        if (str == "CONTROLLER_DPAD_DOWN") return CONTROLLER_DPAD_DOWN;
        if (str == "CONTROLLER_DPAD_LEFT") return CONTROLLER_DPAD_LEFT;
        if (str == "CONTROLLER_DPAD_RIGHT") return CONTROLLER_DPAD_RIGHT;

        if (str == "CONTROLLER_SELECT") return CONTROLLER_SELECT;
        if (str == "CONTROLLER_START") return CONTROLLER_START;

        if (str == "CONTROLLER_GUIDE") return CONTROLLER_GUIDE;
        if (str == "CONTROLLER_MAX") return CONTROLLER_MAX;

        if (str == "CONTROLLER_XBOX_X_SHARE") return CONTROLLER_XBOX_X_SHARE;
        if (str == "CONTROLLER_PS5_MIC") return CONTROLLER_PS5_MIC;
        if (str == "CONTROLLER_SWITCH_CAPTURE") return CONTROLLER_SWITCH_CAPTURE;

        if (str == "CONTROLLER_PADDLE_01") return CONTROLLER_PADDLE_01;
        if (str == "CONTROLLER_PADDLE_02") return CONTROLLER_PADDLE_02;
        if (str == "CONTROLLER_PADDLE_03") return CONTROLLER_PADDLE_03;
        if (str == "CONTROLLER_PADDLE_04") return CONTROLLER_PADDLE_04;

        if (str == "CONTROLLER_PS5_TOUCHPAD") return CONTROLLER_PS5_TOUCHPAD;

        std::stringstream stream;
        stream << "In string_to_controller_buttong: Button with id \"" << str << "\" unrecognized" << std::endl;
        throw std::invalid_argument(stream.str());
        return CONTROLLER_BUTTON_A;
    }
}

