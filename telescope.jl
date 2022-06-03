#
# Copyright 2022 Clemens Cords
# Created on 6/3/22 by clem (mail@clemens-cords.com)
#

module ts

    function get_keyboard_key(str::String) ::Int32
        return ccall((:ts_keyboard_key, "./libtelescope.so"), Int64, (Cstring,), str)
    end
    # no export

    function get_mouse_button(str::String) ::Int32
        return ccall((:ts_mouse_button, "./libtelescope.so"), Int64, (Cstring,), str)
    end
    # no export

    function get_controller_button(str::String) ::Int32
        return ccall((:ts_controller_button, "./libtelescope.so"), Int64, (Cstring,), str)
    end
    # no export

    function export_enum(name::Symbol) ::Nothing

        this_module = @__MODULE__
        this_module.eval(:(export $name))

        for x in instances(this_module.eval(name))
            this_module.eval(:(export $x))
        end

        return nothing
    end
    # no export

    """
    """
    @enum KeyboardKey begin
    
        KEY_UNKNOWN = get_mouse_button("KEY_UNKNOWN")

        NUM_0 = get_mouse_button("NUM_0")
        NUM_1 = get_mouse_button("NUM_1")
        NUM_2 = get_mouse_button("NUM_2")
        NUM_3 = get_mouse_button("NUM_3")
        NUM_4 = get_mouse_button("NUM_4")
        NUM_5 = get_mouse_button("NUM_5")
        NUM_6 = get_mouse_button("NUM_6")
        NUM_7 = get_mouse_button("NUM_7")
        NUM_8 = get_mouse_button("NUM_8")
        NUM_9 = get_mouse_button("NUM_9")

        A = get_mouse_button("A")
        B = get_mouse_button("B")
        C = get_mouse_button("C")
        D = get_mouse_button("D")
        E = get_mouse_button("E")
        F = get_mouse_button("F")
        G = get_mouse_button("G")
        H = get_mouse_button("H")
        I = get_mouse_button("I")
        J = get_mouse_button("J")
        K = get_mouse_button("K")
        L = get_mouse_button("L")
        M = get_mouse_button("M")
        N = get_mouse_button("N")
        O = get_mouse_button("O")
        P = get_mouse_button("P")
        Q = get_mouse_button("Q")
        R = get_mouse_button("R")
        S = get_mouse_button("S")
        T = get_mouse_button("T")
        U = get_mouse_button("U")
        V = get_mouse_button("V")
        W = get_mouse_button("W")
        X = get_mouse_button("X")
        Y = get_mouse_button("Y")
        Z = get_mouse_button("Z")

        ALT = get_mouse_button("ALT")
        RIGHT_ALT = get_mouse_button("RIGHT_ALT")

        CTRL = get_mouse_button("CTRL")
        RIGHT_CTRL = get_mouse_button("RIGHT_CTRL")
        CAPS = get_mouse_button("CAPS")

        SHIFT = get_mouse_button("SHIFT")
        RIGHT_SHIFT = get_mouse_button("RIGHT_SHIFT")

        BRACKET_LEFT = get_mouse_button("BRACKET_LEFT")
        BRACKET_RIGHT = get_mouse_button("BRACKET_RIGHT")

        SLASH = get_mouse_button("SLASH")
        BACKSLASH = get_mouse_button("BACKSLASH")

        UP = get_mouse_button("UP")
        DOWN = get_mouse_button("DOWN")
        LEFT = get_mouse_button("LEFT")
        RIGHT = get_mouse_button("RIGHT")

        RETURN = get_mouse_button("RETURN")
        ESCAPE = get_mouse_button("ESCAPE")
        BACKSPACE = get_mouse_button("BACKSPACE")
        TAB = get_mouse_button("TAB")
        SPACE = get_mouse_button("SPACE")

        HASH = get_mouse_button("HASH")
        PERCENT = get_mouse_button("PERCENT")
        DOLLAR = get_mouse_button("DOLLAR")
        AND = get_mouse_button("AND")
        QUOTE = get_mouse_button("QUOTE")

        LEFT_PARENTHESIS = get_mouse_button("LEFT_PARENTHESIS")
        RIGHT_PARENTHESIS= get_mouse_button("RIGHT_PARENTHESIS")

        ASTERISK = get_mouse_button("ASTERISK")
        PLUS = get_mouse_button("PLUS")
        MINUS = get_mouse_button("MINUS")
        UNDERSCORE = get_mouse_button("UNDERSCORE")

        COMMA = get_mouse_button("COMMA")
        PERIOD = get_mouse_button("PERIOD")
        SEMICOLON = get_mouse_button("SEMICOLON")
        COLON = get_mouse_button("COLON")

        LESS_THAN = get_mouse_button("LESS_THAN")
        GREATER_THAN = get_mouse_button("GREATER_THAN")

        EQUALS = get_mouse_button("EQUALS")
        CIRCUMFLEX = get_mouse_button("CIRCUMFLEX")

        EXCLAMATION_MARK = get_mouse_button("EXCLAMATION_MARK")
        QUESTION_MARK = get_mouse_button("QUESTION_MARK")

        F1 = get_mouse_button("F1")
        F2 = get_mouse_button("F2")
        F3 = get_mouse_button("F3")
        F4 = get_mouse_button("F4")
        F5 = get_mouse_button("F5")
        F6 = get_mouse_button("F6")
        F7 = get_mouse_button("F7")
        F8 = get_mouse_button("F8")
        F9 = get_mouse_button("F9")
        F10 = get_mouse_button("F10")
        F11 = get_mouse_button("F11")
        F12 = get_mouse_button("F12")

        AUDIO_MUTE = get_mouse_button("AUDIO_MUTE")
        AUDIO_STOP = get_mouse_button("AUDIO_STOP")
        AUDIO_PLAY = get_mouse_button("AUDIO_PLAY")
        AUDIO_NEXT = get_mouse_button("AUDIO_NEXT")
        AUDIO_PREV = get_mouse_button("AUDIO_PREV")
        VOLUME_UP = get_mouse_button("VOLUME_UP")
        VOLUME_DOWN = get_mouse_button("VOLUME_DOWN")

        INSERT = get_mouse_button("INSERT")
        PRINTSCREEN = get_mouse_button("PRINTSCREEN")
        DELETE = get_mouse_button("DELETE")
    end
    export_enum(:KeyboardKey)




end
