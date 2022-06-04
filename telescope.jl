#
# Copyright 2022 Clemens Cords
# Created on 6/3/22 by clem (mail@clemens-cords.com)
#

module ts
    ### COMMON ################################################################

    const _lib = "./libtelescope.so"

    """
    `initialize() -> Bool`
    """
    function initialize() ::Bool
    end
    export initialize

    ### VECTOR ################################################################

    """
    TODO
    """
    struct Vector2{T}
        x::T
        y::T
    end
    export Vector2

    const Vector2f = Vector2{Float32}
    const Vector2i = Vector2{Int64}
    const Vector2ui = Vector2{Csize_t}

    export Vector2f, Vector2i, Vector2ui

    """
    TODO
    """
    struct Vector3{T}
        x::T
        y::T
        z::T
    end
    export Vector3

    const Vector3f = Vector3{Float32}
    const Vector3i = Vector3{Int64}
    const Vector3ui = Vector3{Csize_t}

    export Vector3f, Vector3i, Vector3ui
        
    ### COLOR #################################################################
    
    """
    TODO
    """
    struct RGBA
        red::Float32
        green::Float32
        blue::Float32
        alpha::Float32
    end
    export RGBA
    
    """
    TODO
    """
    struct HSVA
        hue::Float32
        saturation::Float32
        value::Float32
        alpha::Float32
    end
    export HSVA
    
    """
    `as_hsv(::RGBA) -> HSVA`
    """
    function as_hsv(rgba::RGBA) ::HSVA
        
        h = Ref{Float32}(-1)
        s = Ref{Float32}(-1)
        v = Ref{Float32}(-1)

        ccall((:ts_rgb_to_hsv, _lib), Cvoid, 
            (Cfloat, Cfloat, Cfloat, Ref{Cfloat}, Ref{Cfloat}, Ref{Cfloat}),
            rgba.red, rgba.green, rgba.blue, h, s, v)
            
        return HSVA(h[], s[], v[], rgba.alpha)
    end
    export as_hsv
    
    """
    `as_rgb(::HSVA) -> RGBA`
    """
    function as_rgb(hsva::HSVA) ::RGBA
        
        r = Ref{Float32}(-1)
        g = Ref{Float32}(-1)
        b = Ref{Float32}(-1)

        ccall((:ts_rgb_to_hsv, _lib), Cvoid, 
            (Cfloat, Cfloat, Cfloat, Ref{Cfloat}, Ref{Cfloat}, Ref{Cfloat}),
            hsva.hue, hsva.saturation, hsva.value, r, g, b)
            
        return RGBA(r[], g[], b[], hsva.alpha)
    end
    export as_rgb


    ### TIME #################################################################

    """
    TODO
    """
    struct Time
        _count_ns::Int64
    end
    export Time

    """
    `as_minutes(::Time) -> Float64`
    """
    function as_minutes(time::Time) ::Float64
        return ccall((:ts_ns_to_minutes, _lib), Cdouble, (Csize_t,), time._count_ns)
    end
    export as_minutes

    """
    `as_seconds(::Time) -> Float64`
    """
    function as_seconds(time::Time) ::Float64
        return ccall((:ts_ns_to_seconds, _lib), Cdouble, (Csize_t,), time._count_ns)
    end
    export as_seconds

    """
    `as_milliseconds(::Time) -> Float64`
    """
    function as_milliseconds(time::Time) ::Float64
        return ccall((:ts_ns_to_milliseconds, _lib), Cdouble, (Csize_t,), time._count_ns)
    end
    export as_milliseconds

    """
    `as_microseconds(::Time) -> Float64`
    """
    function as_microseconds(time::Time) ::Float64
        return ccall((:ts_ns_to_microseconds, _lib), Cdouble, (Csize_t,), time._count_ns)
    end
    export as_microseconds

    """
    `as_nanoseconds(::Time) -> Float64`
    """
    function as_nanoseconds(time::Time) ::Float64
        return ccall((:ts_ns_to_nanoseconds, _lib), Cdouble, (Csize_t,), time._count_ns)
    end
    export as_nanoseconds

    """
    `minutes(::Float64) -> Time`
    """
    function minutes(n::Float64) ::Time
        ns::Csize_t = ccall((:ts_minutes_to_ns, _lib), Csize_t, (Cdouble,), n)
        return Time(ns)
    end
    export minutes

    """
    `seconds(::Float64) -> Time`
    """
    function seconds(n::Float64) ::Time
        ns::Csize_t = ccall((:ts_seconds_to_ns, _lib), Csize_t, (Cdouble,), n)
        return Time(ns)
    end
    export seconds

    """
    `milliseconds(::Float64) -> Time`
    """
    function milliseconds(n::Float64) ::Time
        ns::Csize_t = ccall((:ts_milliseconds_to_ns, _lib), Csize_t, (Cdouble,), n)
        return Time(ns)
    end
    export milliseconds

    """
    `microseconds(::Float64) -> Time`
    """
    function microseconds(n::Float64) ::Time
        ns::Csize_t = ccall((:ts_microseconds_to_ns, _lib), Csize_t, (Cdouble,), n)
        return Time(ns)
    end
    export microseconds

    """
    `nanoseconds(::UInt64) -> Time`
    """
    function nanoseconds(n::UInt64) ::Time
        ns::Csize_t = ccall((:ts_nanoseconds_to_ns, _lib), Csize_t, (Cdouble,), n)
        return Time(ns)
    end
    export nanoseconds

    """
    TODO
    """
    struct Clock
        _native_id::UInt64

        function Clock()

            id = ccall((:ts_clock_create, _lib), Csize_t, ())
            out = new(id)
            finalizer(out) do x::Clock
                ccall((:ts_clock_destroy, _lib), Cvoid, (Csize_t,), x._native_id)
            end
        end
    end
    export Clock

    """
    `elapsed(::Clock) -> Time`
    """
    function elapsed(clock::Clock) ::Time
        out_ns::Csize_t = ccall((:ts_clock_elapsed, _lib), Cdouble, (Csize_t,), clock._native_id)
        return nanoseconds(out_ns)
    end

    """
    `restart(::Clock) -> Time`
    """
    function restart(clock::Clock) ::Time
        out_ns::Csize_t = ccall((:ts_clock_restart, _lib), Cdouble, (Csize_t,), clock._native_id)
        return nanoseconds(out_ns)
    end

    ### ANGLE ################################################################

    """
    TODO
    """
    struct Angle
        _degrees::Float32
    end

    """
    `degrees(::Float32) -> Angle`
    """
    function degrees(n::Float32) ::Angle
    end

    """
    `radians(::Float32) -> Angle`
    """
    function radians(n::Float32) ::Angle
    end

    """
    `as_degrees(::Angle) -> Float32`
    """
    function as_degrees(angle::Angle) ::Float32
    end

    """
    `as_radians(::Angle) -> Float32`
    """
    function as_radians(angle::Angle) ::Float32
    end

    ### INPUT ################################################################

    function get_keyboard_key_enum_value(str::String) ::Int32
        return ccall((:ts_keyboard_key, _lib), Int64, (Cstring,), str)
    end
    # no export

    function get_mouse_button_enum_value(str::String) ::Int32
        return ccall((:ts_mouse_button, _lib), Int64, (Cstring,), str)
    end
    # no export

    function get_controller_button_enum_value(str::String) ::Int32
        return ccall((:ts_controller_button, _lib), Int64, (Cstring,), str)
    end
    # no export

    function export_enum(enum::Enum) ::Nothing

        this_module = @__MODULE__
        this_module.eval(:(export $(enum.name)))

        for x in instances(this_module.eval(name))
            this_module.eval(:(export $x))
        end

        return nothing
    end
    # no export

    """
    TODO
    """
    @enum KeyboardKey begin
    
        KEY_UNKNOWN = get_keyboard_key_enum_value("KEY_UNKNOWN")

        NUM_0 = get_keyboard_key_enum_value("NUM_0")
        NUM_1 = get_keyboard_key_enum_value("NUM_1")
        NUM_2 = get_keyboard_key_enum_value("NUM_2")
        NUM_3 = get_keyboard_key_enum_value("NUM_3")
        NUM_4 = get_keyboard_key_enum_value("NUM_4")
        NUM_5 = get_keyboard_key_enum_value("NUM_5")
        NUM_6 = get_keyboard_key_enum_value("NUM_6")
        NUM_7 = get_keyboard_key_enum_value("NUM_7")
        NUM_8 = get_keyboard_key_enum_value("NUM_8")
        NUM_9 = get_keyboard_key_enum_value("NUM_9")

        A = get_keyboard_key_enum_value("A")
        B = get_keyboard_key_enum_value("B")
        C = get_keyboard_key_enum_value("C")
        D = get_keyboard_key_enum_value("D")
        E = get_keyboard_key_enum_value("E")
        F = get_keyboard_key_enum_value("F")
        G = get_keyboard_key_enum_value("G")
        H = get_keyboard_key_enum_value("H")
        I = get_keyboard_key_enum_value("I")
        J = get_keyboard_key_enum_value("J")
        K = get_keyboard_key_enum_value("K")
        L = get_keyboard_key_enum_value("L")
        M = get_keyboard_key_enum_value("M")
        N = get_keyboard_key_enum_value("N")
        O = get_keyboard_key_enum_value("O")
        P = get_keyboard_key_enum_value("P")
        Q = get_keyboard_key_enum_value("Q")
        R = get_keyboard_key_enum_value("R")
        S = get_keyboard_key_enum_value("S")
        T = get_keyboard_key_enum_value("T")
        U = get_keyboard_key_enum_value("U")
        V = get_keyboard_key_enum_value("V")
        W = get_keyboard_key_enum_value("W")
        X = get_keyboard_key_enum_value("X")
        Y = get_keyboard_key_enum_value("Y")
        Z = get_keyboard_key_enum_value("Z")

        ALT = get_keyboard_key_enum_value("ALT")
        RIGHT_ALT = get_keyboard_key_enum_value("RIGHT_ALT")

        CTRL = get_keyboard_key_enum_value("CTRL")
        RIGHT_CTRL = get_keyboard_key_enum_value("RIGHT_CTRL")
        CAPS = get_keyboard_key_enum_value("CAPS")

        SHIFT = get_keyboard_key_enum_value("SHIFT")
        RIGHT_SHIFT = get_keyboard_key_enum_value("RIGHT_SHIFT")

        BRACKET_LEFT = get_keyboard_key_enum_value("BRACKET_LEFT")
        BRACKET_RIGHT = get_keyboard_key_enum_value("BRACKET_RIGHT")

        SLASH = get_keyboard_key_enum_value("SLASH")
        BACKSLASH = get_keyboard_key_enum_value("BACKSLASH")

        UP = get_keyboard_key_enum_value("UP")
        DOWN = get_keyboard_key_enum_value("DOWN")
        LEFT = get_keyboard_key_enum_value("LEFT")
        RIGHT = get_keyboard_key_enum_value("RIGHT")

        RETURN = get_keyboard_key_enum_value("RETURN")
        ESCAPE = get_keyboard_key_enum_value("ESCAPE")
        BACKSPACE = get_keyboard_key_enum_value("BACKSPACE")
        TAB = get_keyboard_key_enum_value("TAB")
        SPACE = get_keyboard_key_enum_value("SPACE")

        HASH = get_keyboard_key_enum_value("HASH")
        PERCENT = get_keyboard_key_enum_value("PERCENT")
        DOLLAR = get_keyboard_key_enum_value("DOLLAR")
        AND = get_keyboard_key_enum_value("AND")
        QUOTE = get_keyboard_key_enum_value("QUOTE")

        LEFT_PARENTHESIS = get_keyboard_key_enum_value("LEFT_PARENTHESIS")
        RIGHT_PARENTHESIS= get_keyboard_key_enum_value("RIGHT_PARENTHESIS")

        ASTERISK = get_keyboard_key_enum_value("ASTERISK")
        PLUS = get_keyboard_key_enum_value("PLUS")
        MINUS = get_keyboard_key_enum_value("MINUS")
        UNDERSCORE = get_keyboard_key_enum_value("UNDERSCORE")

        COMMA = get_keyboard_key_enum_value("COMMA")
        PERIOD = get_keyboard_key_enum_value("PERIOD")
        SEMICOLON = get_keyboard_key_enum_value("SEMICOLON")
        COLON = get_keyboard_key_enum_value("COLON")

        LESS_THAN = get_keyboard_key_enum_value("LESS_THAN")
        GREATER_THAN = get_keyboard_key_enum_value("GREATER_THAN")

        EQUALS = get_keyboard_key_enum_value("EQUALS")
        CIRCUMFLEX = get_keyboard_key_enum_value("CIRCUMFLEX")

        EXCLAMATION_MARK = get_keyboard_key_enum_value("EXCLAMATION_MARK")
        QUESTION_MARK = get_keyboard_key_enum_value("QUESTION_MARK")

        F1 = get_keyboard_key_enum_value("F1")
        F2 = get_keyboard_key_enum_value("F2")
        F3 = get_keyboard_key_enum_value("F3")
        F4 = get_keyboard_key_enum_value("F4")
        F5 = get_keyboard_key_enum_value("F5")
        F6 = get_keyboard_key_enum_value("F6")
        F7 = get_keyboard_key_enum_value("F7")
        F8 = get_keyboard_key_enum_value("F8")
        F9 = get_keyboard_key_enum_value("F9")
        F10 = get_keyboard_key_enum_value("F10")
        F11 = get_keyboard_key_enum_value("F11")
        F12 = get_keyboard_key_enum_value("F12")

        AUDIO_MUTE = get_keyboard_key_enum_value("AUDIO_MUTE")
        AUDIO_STOP = get_keyboard_key_enum_value("AUDIO_STOP")
        AUDIO_PLAY = get_keyboard_key_enum_value("AUDIO_PLAY")
        AUDIO_NEXT = get_keyboard_key_enum_value("AUDIO_NEXT")
        AUDIO_PREV = get_keyboard_key_enum_value("AUDIO_PREV")
        VOLUME_UP = get_keyboard_key_enum_value("VOLUME_UP")
        VOLUME_DOWN = get_keyboard_key_enum_value("VOLUME_DOWN")

        INSERT = get_keyboard_key_enum_value("INSERT")
        PRINTSCREEN = get_keyboard_key_enum_value("PRINTSCREEN")
        DELETE = get_keyboard_key_enum_value("DELETE")
    end
    export_enum(KeyboardKey)

    """
    TODO
    """
    @enum MouseButton begin
    
        MOUSE_LEFT = get_mouse_button_enum_value("MOUSE_LEFT")
        MOUSE_RIGHT = get_mouse_button_enum_value("MOUSE_RIGHT")
        MOUSE_MIDDLE = get_mouse_button_enum_value("MOUSE_MIDDLE")

        MOUSE_X1 = get_mouse_button_enum_value("MOUSE_X1")
        MOUSE_X2 = get_mouse_button_enum_value("MOUSE_X2")
    end
    export_enum(MouseButton)

    """
    TODO
    """
    @enum ControllerButton begin

        CONTROLLER_BUTTON_UNKNOWN = get_controller_button_enum_value("CONTROLLER_BUTTON_UNKNOWN")

        CONTROLLER_BUTTON_A = get_controller_button_enum_value("CONTROLLER_BUTTON_A")
        CONTROLLER_BUTTON_B = get_controller_button_enum_value("CONTROLLER_BUTTON_B")
        CONTROLLER_BUTTON_X = get_controller_button_enum_value("CONTROLLER_BUTTON_X")
        CONTROLLER_BUTTON_Y = get_controller_button_enum_value("CONTROLLER_BUTTON_Y")

        CONTROLLER_LEFTSTICK_PRESS = get_controller_button_enum_value("CONTROLLER_LEFTSTICK_PRESS")
        CONTROLLER_RIGHTSTICK_PRESS = get_controller_button_enum_value("CONTROLLER_RIGHTSTICK_PRESS")
        CONTROLLER_LEFTSHOULDER = get_controller_button_enum_value("CONTROLLER_LEFTSHOULDER")
        CONTROLLER_RIGHTSHOULDER = get_controller_button_enum_value("CONTROLLER_RIGHTSHOULDER")

        CONTROLLER_DPAD_UP = get_controller_button_enum_value("CONTROLLER_DPAD_UP")
        CONTROLLER_DPAD_DOWN = get_controller_button_enum_value("CONTROLLER_DPAD_DOWN")
        CONTROLLER_DPAD_LEFT = get_controller_button_enum_value("CONTROLLER_DPAD_LEFT")
        CONTROLLER_DPAD_RIGHT = get_controller_button_enum_value("CONTROLLER_DPAD_RIGHT")

        CONTROLLER_SELECT = get_controller_button_enum_value("CONTROLLER_SELECT")
        CONTROLLER_START = get_controller_button_enum_value("CONTROLLER_START")

        CONTROLLER_GUIDE = get_controller_button_enum_value("CONTROLLER_GUIDE")
        CONTROLLER_MAX = get_controller_button_enum_value("CONTROLLER_MAX")

        CONTROLLER_XBOX_X_SHARE = get_controller_button_enum_value("CONTROLLER_XBOX_X_SHARE")
        CONTROLLER_PS5_MIC = get_controller_button_enum_value("CONTROLLER_PS5_MIC")
        CONTROLLER_SWITCH_CAPTURE = get_controller_button_enum_value("CONTROLLER_SWITCH_CAPTURE")

        CONTROLLER_PADDLE_01 = get_controller_button_enum_value("CONTROLLER_PADDLE_01")
        CONTROLLER_PADDLE_02 = get_controller_button_enum_value("CONTROLLER_PADDLE_02")
        CONTROLLER_PADDLE_03 = get_controller_button_enum_value("CONTROLLER_PADDLE_03")
        CONTROLLER_PADDLE_04 = get_controller_button_enum_value("CONTROLLER_PADDLE_04")

        CONTROLLER_PS5_TOUCHPAD = get_controller_button_enum_value("CONTROLLER_PS5_TOUCHPAD")
    end
    export_enum(ControllerButton)

    """
    InputHandler: query the global state of the keyboard, mouse and any controller
    """
    module InputHandler

        import Main.ts; using Main.ts
        
        ### KEYBOARD #########################################################

        """
        `is_down(::KeyboardKey) -> Bool`
        """
        function is_down(key::KeyboardKey) ::Bool
            return ccall((:ts_keyboard_is_down, _lib), Bool, (Int64,), key)
        end
        export is_down
        
         """
        `has_state_changed(::KeyboardKey) -> Bool`
        """
        function has_state_changed(key::KeyboardKey) ::Bool
            return ccall((:ts_keyboard_has_state_changed, _lib), Bool, (Int64,), key)
        end
        export has_state_changed
        
        """
        `was_pressed(::KeyboardKey) -> Bool`
        """
        function was_pressed(key::KeyboardKey) ::Bool
            return ccall((:ts_keyboard_was_pressed, _lib), Bool, (Int64,), key)
        end
        export was_pressed
        
        """
        `was_released(::KeyboardKey) -> Bool`
        """
        function was_released(key::KeyboardKey) ::Bool
            return ccall((:ts_keyboard_was_released, _lib), Bool, (Int64,), key)
        end
        export was_released
        
        ### MOUSE ############################################################

        """
        `is_down(::MouseButton) -> Bool`
        """
        function is_down(key::MouseButton) ::Bool
            return ccall((:ts_mouse_is_down, _lib), Bool, (Int64,), key)
        end
        export is_down
        
        """
        `has_state_changed(::MouseButton) -> Bool`
        """
        function has_state_changed(key::MouseButton) ::Bool
            return ccall((:ts_mouse_has_state_changed, _lib), Bool, (Int64,), key)
        end
        export has_state_changed
        
        """
        `was_pressed(::MouseButton) -> Bool`
        """
        function was_pressed(key::MouseButton) ::Bool
            return ccall((:ts_mouse_was_pressed, _lib), Bool, (Int64,), key)
        end
        export was_pressed
        
        """
        `was_released(::MouseButton) -> Bool`
        """
        function was_released(key::MouseButton) ::Bool
            return ccall((:ts_mouse_was_released, _lib), Bool, (Int64,), key)
        end
        export was_released
        
        """
        `get_cursor_position() -> ts.Vector2f`
        """
        function get_cursor_position() ::Vector2f

            x = Ref{Cfloat}(0)
            y = Ref{Cfloat}(0)
            ccall((:ts_mouse_cursor_position, _lib), Cvoid, (Ref{Cfloat}, Ref{Cfloat}), x, y)
            return Vector2f(x, y)
        end
        export get_cursor_position

        """
        `get_scrollwheel() -> ts.Vector2f`
        """
        function get_scrollwheel() ::Vector2f

            x = Ref{Cfloat}(0)
            y = Ref{Cfloat}(0)
            ccall((:ts_mouse_scrollwheel, _lib), Cvoid, (Ref{Cfloat}, Ref{Cfloat}), x, y)
            return Vector2f(x, y)
        end
        export get_scrollwheel
        
        ### CONTROLLER #######################################################

        const ControllerID = Csize_t
        export ControllerID

        """
        `is_down(::ControllerButton, [::ControllerID]) -> Bool`
        """
        function is_down(key::ControllerButton, id::ControllerID = 0) ::Bool
            return ccall((:ts_controller_is_down, _lib), Bool, (Int64, Csize_t), key, id)
        end
        export is_down

        """
        `has_state_changed(::ControllerButton, [::ControllerID]) -> Bool`
        """
        function has_state_changed(key::ControllerButton, id::ControllerID = 0) ::Bool
            return ccall((:ts_controller_has_state_changed, _lib), Bool, (Int64, Csize_t), key, id)
        end
        export has_state_changed

        """
        `was_pressed(::ControllerButton, [::ControllerID]) -> Bool`
        """
        function was_pressed(key::ControllerButton, id::ControllerID = 0) ::Bool
            return ccall((:ts_controller_was_pressed, _lib), Bool, (Int64, Csize_t), key, id)
        end
        export was_pressed
        
        """
        `was_released(::ControllerButton, [::ControllerID]) -> Bool`
        """
        function was_released(key::ControllerButton, id::ControllerID = 0) ::Bool
            return ccall((:ts_controller_was_released, _lib), Bool, (Int64, Csize_t), key, id)
        end
        export was_released

        """
        `get_controller_axis_left([::ControllerID]) -> ts.Vector2f`
        """
        function get_controller_axis_left(id::ControllerID = 0) ::Vector2f

            x = Ref{Cfloat}(0)
            y = Ref{Cfloat}(0)
            ccall((:ts_controller_axis_left, _lib), Cvoid, (Ref{Cfloat}, Ref{Cfloat}), x, y)
            return Vector2f(x, y)

        end
        export get_controller_axis_left

        """
        `get_controller_axis_right([::ControllerID]) -> ts.Vector2f`
        """
        function get_controller_axis_right(id::ControllerID = 0) ::Vector2f

            x = Ref{Cfloat}(0)
            y = Ref{Cfloat}(0)
            ccall((:ts_controller_axis_right, _lib), Cvoid, (Ref{Cfloat}, Ref{Cfloat}), x, y)
            return Vector2f(x, y)

        end
        export get_controller_axis_right

        """
        `get_controller_trigger_left([::ControllerID]) -> Float32`
        """
        function get_controller_trigger_left(id::ControllerID = 0) ::Float32
            return ccall((:ts_controller_trigger_left, _lib), Cfloat, (Csize_t,), id)
        end
        export get_controller_trigger_left

        """
        `get_controller_trigger_right([::ControllerID]) -> Float32`
        """
        function get_controller_trigger_right(id::ControllerID = 0) ::Float32
            return ccall((:ts_controller_trigger_left, _lib), Cfloat, (Csize_t,), id)
        end
        export get_controller_trigger_left
    end

    ### SOUND ################################################################

    """
    TODO
    """
    struct Sound

        _native_id::Csize_t

        function Sound(path::String, volume_modifier::Cfloat = 1)

            id = ccall((:ts_sound_load, _lib), Csize_t, (Cstring, Cfloat), path, volume_modifier)
            out = new(id)
            finalizer(out) do x::Sound
                call((:ts_sound_unload, _lib), Cvoid, (Csize_t), x._native_id)
            end
            return out
        end
    end
    export Sound

    const ChannelID = Csize_t;

    """
    TODO
    """
    module SoundHandler

        import Main.ts; using Main.ts

        const n_channels = ccall((:ts_sound_get_max_n_channels, _lib), Csize_t, ())
        export n_channels

        """
        `play(::ChannelID, ::Sound, ::Integer, ::Time) -> Nothing`
        """
        function play(channel::ChannelID, sound::Sound, n_loops::Integer, fade_in_duration::Time) ::Nothing
        end
        export play
        
        """
        `stop(::ChannelID, ::Time) -> Nothing`
        """
        function stop(channel::ChannelID, fade_out_duration::Time) ::Nothing
        end
        export stop
        
        """
        `pause(::ChannelID) -> Nothing`
        """
        function pause(channel::ChannelID) ::Nothing
        end
        export pause
        
        """
        `unpause(::ChannelID) -> Nothing`
        """
        function unpause(channel::ChannelID) ::Nothing
        end
        export unpause

        """
        `force_stop(::ChannelID) -> Nothing`
        """
        function force_stop(channel::ChannelID) ::Nothing
        end
        export force_stop

        """
        `is_playing(::ChannelID) -> Bool`
        """
        function is_playing(channel::ChannelID) ::Bool
        end
        export is_playing

        """
        `is_paused(::ChannelID) -> Bool`
        """
        function is_paused(channel::ChannelID) ::Bool
        end
        export is_paused

        """
        `is_stopped(::ChannelID) -> Bool`
        """
        function is_stopped(channel::ChannelID) ::Bool
        end
        export is_stopped

        """
        `set_volume(::ChannelID, ::Float32) -> Nothing`
        """
        function set_volume(channel::ChannelID, zero_to_one::Float32) ::Nothing
        end
        export set_volume

        """
        `get_volume(::ChannelID) -> Float32`
        """
        function get_volume(channel::ChannelID) ::Float32
        end
        export get_volume

        """
        `set_panning(::ChannelID, ::Angle) -> Nothing`
        """
        function set_panning(channel::ChannelID, angle::Angle) ::Nothing
        end
        export set_panning

        """
        `get_panning(::ChannelID) -> Angle`
        """
        function get_panning(channel::ChannelID) ::Angle
        end
        export get_panning
    end

    ### MUSIC ################################################################

    """
    TODO
    """
    struct Music

        _native_id::Csize_t

        function Music(path::String)

            id = ccall((:ts_music_load, _lib), Csize_t, (Cstring,), path)
            out = new(id)
            finalizer(out) do x::Music
                ccall((ts_music_unload, _lib), Cvoid, (Csize_t,), x._native_id)
            end
        end
    end
    export Music

    """
    TODO
    """
    module MusicHandler

        import Main.ts; using Main.ts

        const sample_rate = ccall((:ts_music_sample_rate, _lib), Csize_t, ())
        export sample_rate

        """
        `set_volume(::Float32) -> Nothing`
        """
        function set_volume(zero_to_one::Float32) ::Nothing
        end
        export set_volume

        """
        `get_volume() -> Float32`
        """
        function get_volume() ::Float32
        end
        export get_volume

        """
        `play(::Music, ::Bool, ::Time) -> Nothing`
        """
        function play(music::Music, should_loop::Bool, fade_in_duration::Time) ::Nothing
        end
        export play

        """
        `play_next(::Music, ::Bool, ::Time) -> Nothing
        """
        function play_next(music::Music, should_loop::Bool, fade_in_duration::Time) ::Nothing
        end
        export play_next

        """
        `stop(::Time) -> Nothing`
        """
        function stop(fade_out_duration::Time) ::Nothing
        end
        export stop

        """
        `next(::Time) -> Nothing`
        """
        function next(fade_out_duration::Time) ::Nothing
        end
        export next

        """
        `clear_next() -> Nothing`
        """
        function clear_next() ::Nothing
        end
        export clear_next

        """
        `force_stop() -> Nothing`
        """
        function force_stop() ::Nothing
        end
        export force_stop

        """
        `pause() -> Nothing`
        """
        function pause() ::Nothing
        end
        export pause

        """
        `unpause() -> Nothing`
        """
        function unpause() ::Nothing
        end
        export unpause

        """
        `skip_to(::Time) -> Nothing`
        """
        function skip_to(timestamp::Time) ::Nothing
        end
        export skip_to

        """
        `is_playing() -> Bool`
        """
        function is_playing() ::Bool
        end
        export is_playing

        """
        `is_paused() -> Bool`
        """
        function is_paused() ::Bool
        end
        export is_paused

        """
        `is_stopped() -> Bool`
        """
        function is_stopped() ::Bool
        end
        export is_stopped
    end

    ### WINDOW ################################################################

    const WindowID = UInt64

    @enum WindowOptions begin

        DEFAULT = 0
        FULLSCREEN = 1 << 1
        BORDERLESS = 1 << 2
        RESIZABLE  = 1 << 3
    end
    export_enum(WindowOptions)

    """
    TODO
    """
    struct Window

        _native_id::WindowID

        function Window(width::Integer, height::Integer, title::Cstring, options::UInt32 = DEFAULT)
            id = ccall((:ts_window_create, _lib), Csize_t, (Csize_t, Csize_t, Cstring, UInt32), width, height, title, options)
            out = new(id)
            finalizer(out) do x::Window
                ccall((:ts_window_destroy, _lib), Cvoid, (Csize_t,), x._native_id)
            end
            return out
        end
    end
    export Window

    """
    `close(::Window) -> Nothing`
    """
    function close(window::Window) ::Nothing
    end
    export close

    """
    `is_open(::Window) -> Bool`
    """
    function is_open(window::Window) ::Bool
    end
    export is_open

    """
    `get_size(::Window) -> Vector2ui`
    """
    function get_size(window::Window) ::Vector2ui
    end
    export get_size

    """
    `get_position(::Window) -> Vector2ui`
    """
    function get_position(window::Window) ::Vector2ui
    end
    export get_position

    """
    `set_hidden(::Window, ::Bool) -> Nothing
    """
    function set_hidden(window::Window, hidden::Bool) ::Nothing
    end
    export set_hidden

    """
    `is_hidden(::Window) -> Bool`
    """
    function is_hidden(window::Window) ::Bool
    end
    export is_hidden

    """
    `minimize(::Window) -> Nothing`
    """
    function minimize(window::Window) ::Nothing
    end
    export minimize

    """
    `is_minimized(::Window) -> Bool`
    """
    function is_minimized(window::Window) ::Bool
    end
    export is_minimizedd

    """
    `maximize(::Window) -> Nothing`
    """
    function maximize(window::Window) ::Nothing
    end
    export maximize

    """
    `is_maximized(::Window) -> Bool`
    """
    function is_maximized(window::Window) ::Bool
    end
    export is_maximized

    """
    `has_focus(::Window) -> Bool``
    """
    function has_focus(window::Window) ::Bool
    end
    export has_focus

    """
    `has_mouse_focus(::Window) -> Bool`
    """
    function has_mouse_focus(window::Window) ::Bool
    end
    export has_mouse_focus

    """
    `clear(::Window) -> Nothing`
    """
    function clear(window::Window) ::Nothing
    end
    export clear

    """
    `render(::Window, ::AbstractShape, ::Transform) ::Nothing`
    """
    function render(window::Window, shape::Shape_t, transform::Transform) ::Nothing where Shape_t <: AbstractShape
    end
    export render

    """
    `flush(::Window) -> Nothing`
    """
    function flush(window::Window) ::Nothing
    end
    export flush

    """
    `set_framerate_limit(::Integer) -> Nothing`
    """
    function set_framerate_limit(n_fps::Integer) ::Nothing
    end
    export set_framerate_limit

    """
    `start_frame(::Window) -> Nothing`
    """
    function start_frame(window::Window) ::Nothing
    end
    export start_frame

    """
    `end_frame(::Window) -> Nothing`
    """
    function end_frame(window::Window) ::Nothing
    end
    export end_frame

    ### TEXTURES ##############################################################

    @enum TextureFilteringMode begin

        NEAREST_NEIGHBOUR = ccall((:ts_texture_filtering_mode_nearest_neighbour), Cint, ())
        LINEAR = ccall((:ts_texture_filtering_mode_linear), Cint, ())
        ANISOTROPIC = ccall((:ts_texture_filtering_mode_anisotropic), Cint, ())
    end
    export_enum(TextureFilteringMode)

    @enum TextureBlendMode begin

        NONE = ccall((:ts_texture_blend_mode_none), Cint, ())
        ALPHA = ccall((:ts_texture_blend_mode_alpha), Cint, ())
        ADD = ccall((:ts_texture_blend_mode_add), Cint, ())
        MULTIPLY = ccall((:ts_texture_blend_mode_multiply), Cint, ())
    end
    export_enum(TextureBlendMode)

    """
    TODO
    """
    abstract type Texture end;
    export Texture

    const TextureID = UInt64

    """
    TODO
    """
    struct RenderTexture <: Texture

        _native_id::TextureID

        # equivalent to ts::RenderTexture::create
        function RenderTexture(window::Window, width::Unsigned, height::Unsigned)

            id = ccall((:ts_texture_create_render_texture, _lib),
                    Csize_t, (Csize_t, Csize_t, Csize_t),
                    window._native_id, width, height)

            out = new(id)
            finalizer(out) do x::RenderTexture
                ccall((:ts_texture_destroy_texture, _lib), Cvoid, (Csize_t,), x._native_id)
            end
        end
    end

    """
    TODO
    """
    struct StaticTexture <: Texture

        _native_id::TextureID

        # equivalent to ts::StaticTexture::load
        function StaticTexture(window::Window, path::String)

            id = ccall((:ts_texture_load_static_texture, _lib), Csize_t, (Csize_t, Cstring), window._native_id, path)

            out = new(id)
            finalizer(out) do x::StaticTexture
                ccall((:ts_texture_destroy_texture, _lib), Cvoid, (Csize_t,), x._native_id)
            end
        end

        # equivalent to ts::StaticTexture::create
        function StaticTexture(window::Window, width::Unsigned, height::Unsigned, color::RGBA)

            id = ccall((:ts_texture_create_static_texture, _lib), Csize_t,
                    (Csize_t, Csize_t, Csize_t, Cfloat, Cfloat, Cfloat, Cfloat),
                    window._native_id, width, height, color.red, color.green, color.blue, color.alpha)

            out = new(id)
            finalizer(out) do x::StaticTexture
                ccall((ts_texture_destroy_texture, _lib), Cvoid, (Csize_t,), x._native_id)
            end
        end
    end

    """
    `unload(::Texture) -> Nothing`
    """
    function unload(texture::Texture) ::Nothing
    end
    
    """
    `set_color(::Texture) -> Nothing`
    """
    function set_color(texture::Texture) ::Nothing
    end
    
    """
    `get_color(::Texture) -> RGBA`
    """
    function get_color(texture::Texture) ::RGBA
    end
    
    """
    `set_blend_mode(::Texture, ::TextureBlendMode) -> RGBA`
    """
    function set_blend_mode(texture::Texture, mode::TextureBlendMode) ::Nothing
    end

    """
    `get_blend_mode(::Texture) -> TextureBlendMode`
    """
    function get_blend_mode(texture::Texture) ::TextureBlendMode
    end

    """
    `set_filtering_mode(::Texture, ::TextureFilteringMode) -> Nothing`
    """
    function set_filtering_mode(texture::Texture, mode::TextureFilteringMode) ::Nothing
    end

    """
    `get_filtering_mode(::Texture) -> TextureFilteringMode`
    """
    function get_filtering_mode(texture::Texture) ::TextureFilteringMode
    end

    """
    `get_size(::Texture) -> Vector2ui`
    """
    function get_size(texture::Texture) ::Vector2ui
    end

    ### TRANSFORM #############################################################

    """
    """
    struct Transform end

    """
    `apply_to(::Transform, ::Vector2f) -> Vector2f`
    """
    function apply_to(transform::Transform, point::Vector2f) ::Vector2f
    end

    """
    `reset(::Transform) -> Nothing`
    """
    function reset(transform::Transform) ::Nothing
    end

    """
    `combine(::Transform, ::Transform) -> Nothing`
    """
    function combine(left::Transform, right::Transform) ::Nothing
    end

    """
    `translate(::Transform, ::Float32, ::Float32) -> Nothing`
    """
    function translate(transform::Transform, x::Float32, y::Float32) ::Nothing
    end

    """
    `rotate(::Transform, ::Angle, ::Vector2f) -> Nothing`
    """
    function rotate(transform::Transform, angle::Angle, origin::Vector2f) ::Nothing
    end

    """
    `scale(::Transform, ::Float32, ::Float32) -> Nothing`
    """
    function scale(transform::Transform, x_scale::Float32, y_scale::Float32) ::Nothing
    end

    """
    `shear(::Transform, ::Float32, ::Float32) -> Nothing`
    """
    function shear(transform::Transform, x_scale::Float32, y_scale::Float32) ::Nothing
    end

    """
    `reflect(::Transform, ::Bool, ::Bool, ::Vector2f) -> Nothing`
    """
    function reflect(transform::Transform, about_x_axis::Bool, about_y_axis::Bool, origin::Vector2f) ::Nothing
    end

    ### GEOMETRY ##############################################################

    """
    TODO
    """
    struct Triangle

        a::Vector2f
        b::Vector2f
        c::Vector2f
    end

    """
    TODO
    """
    struct Rectangle

        top_left::Vector2f
        size::Vector2f
    end

    """
    TODO
    """
    struct Circle

        center::Vector2f
        radius::Float32
    end

    ### SHAPE #################################################################

    """
    """
    abstract type Shape end

    """
    `set_centroid(::Shape, ::Vector2f) -> Nothing`
    """
    function set_centroid(shape::Shape, position::Vector2f) ::Nothing
    end

    """
    `get_centroid(::Shape) -> Vector2f`
    """
    function get_centroid(shape::Shape) ::Vector2f
    end

    """
    `move(::Shape, ::Float32, ::Float32) -> Nothing`
    """
    function move(shape::Shape) ::Nothing
    end

    """
    `set_color(::Shape, ::RGBA) -> Nothing`
    """
    function set_color(shape::Shape, color::RGBA) ::Nothing
    end

    """
    `get_color(::Shape) -> RGBA`
    """
    function get_color(shape::Shape) ::RGBA
    end

    """
    `get_n_vertices(::Shape) -> UInt64`
    """
    function get_n_vertices(shape::Shape) ::UInt64
    end

    """
    `get_texture(::Shape) -> Ref{Texture}`
    """
    function get_texture(shape::Shape) ::Ref{Texture}
    end

    """
    `set_texture(::Shape, ::Texture) -> Nothing`
    """
    function set_texture(shape::Shape, texture::Texture) ::Nothing
    end

    """
    `set_texture_rectangle(::Shape, ::Rectangle) -> Nothing`
    """
    function set_texture_rectangle(shape::Shape, rect::Rectangle) ::Nothing
    end

    """
    `get_texture_rectangle(::Shape) -> Rectangle`
    """
    function get_texture_rectangle(shape::Shape) ::Rectangle
    end

    """
    `get_bounding_box(::Shape) -> Rectangle`
    """
    function get_bounding_box(shape::Shape) ::Rectangle
    end

    """
    `set_vertex_position(::Shape, ::Integer, ::Vector2f) -> Nothing`
    """
    function set_vertex_position(shape::Shape, index::Integer, position::Vector2f) ::Nothing
    end

    """
    `set_vertex_color(::Shape, ::Integer, ::RGBA) -> Nothing`
    """
    function set_vertex_position(shape::Shape, index::Integer, color::RGBA) ::Nothing
    end

    """
    `set_vertex_texture_coordinate(::Shape, ::Integer, ::Vector2f) -> Nothing`
    """
    function set_vertex_texture_coordinate(shape::Shape, index::Integer, coordinate::Vector2f) ::Nothing
    end

    """
    `get_vertex_position(shape::Shape, index::Integer) -> Vector2f`
    """
    function get_vertex_position(shape::Shape, index::Integer) ::Vector2f
    end

    """
    `get_vertex_color(shape::Shape, index::Integer) -> RGBA`
    """
    function get_vertex_color(shape::Shape, index::Integer) ::RGBA
    end

    """
    `get_vertex_texture_coordinate(shape::Shape, index::Integer) -> Vector2f`
    """
    function get_vertex_texture_coordinate(shape::Shape, index::Integer) ::Vector2f
    end

    ### TRIANGLE SHAPE ########################################################

    """
    TODO
    """
    struct TriangleShape

        _native::Ptr{Cvoid}

        function TriangleShape(a::Vector2f, b::Vector2f, c::Vector2f)
        end
    end

    ### RECTANGLE SHAPE #######################################################

    """
    TODO
    """
    struct RectangleShape

        _native::Ptr{Cvoid}

        function RectangleShape(top_left::Vector2f, size::Vector2f)
        end
    end

    """
    `set_top_left(::RectangleShape, ::Vector2f) -> Nothing`
    """
    function set_top_left(rect::RectangleShape, position::Vector2f) ::Nothing
    end

    """
    `get_top_left(::RectangleShape) -> Vector2f`
    """
    function get_top_left(rect::RectangleShape) ::Vector2f
    end

    """
    `get_size(::RectangleShape) -> Vector2f`
    """
    function get_size(rect::RectangleShape) ::Vector2f
    end

    """
    `set_size(::RectangleShape, ::Vector2f) -> Nothing`
    """
    function set_size(rect::RectangleShape, size::Vector2f) ::Nothing
    end

    ### POLYGON SHAPE #########################################################

    """
    TODO
    """
    struct PolygonShape

        _native::Ptr{Cvoid}

        function PolygonShape(positions::Vector2f...)
        end
    end
end
