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

    ### WINDOW ################################################################

    """
    TODO
    """

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
            finalizer(out) do x
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

    function get_keyboard_key(str::String) ::Int32
        return ccall((:ts_keyboard_key, _lib), Int64, (Cstring,), str)
    end
    # no export

    function get_mouse_button(str::String) ::Int32
        return ccall((:ts_mouse_button, _lib), Int64, (Cstring,), str)
    end
    # no export

    function get_controller_button(str::String) ::Int32
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
    
        KEY_UNKNOWN = get_keyboard_key("KEY_UNKNOWN")

        NUM_0 = get_keyboard_key("NUM_0")
        NUM_1 = get_keyboard_key("NUM_1")
        NUM_2 = get_keyboard_key("NUM_2")
        NUM_3 = get_keyboard_key("NUM_3")
        NUM_4 = get_keyboard_key("NUM_4")
        NUM_5 = get_keyboard_key("NUM_5")
        NUM_6 = get_keyboard_key("NUM_6")
        NUM_7 = get_keyboard_key("NUM_7")
        NUM_8 = get_keyboard_key("NUM_8")
        NUM_9 = get_keyboard_key("NUM_9")

        A = get_keyboard_key("A")
        B = get_keyboard_key("B")
        C = get_keyboard_key("C")
        D = get_keyboard_key("D")
        E = get_keyboard_key("E")
        F = get_keyboard_key("F")
        G = get_keyboard_key("G")
        H = get_keyboard_key("H")
        I = get_keyboard_key("I")
        J = get_keyboard_key("J")
        K = get_keyboard_key("K")
        L = get_keyboard_key("L")
        M = get_keyboard_key("M")
        N = get_keyboard_key("N")
        O = get_keyboard_key("O")
        P = get_keyboard_key("P")
        Q = get_keyboard_key("Q")
        R = get_keyboard_key("R")
        S = get_keyboard_key("S")
        T = get_keyboard_key("T")
        U = get_keyboard_key("U")
        V = get_keyboard_key("V")
        W = get_keyboard_key("W")
        X = get_keyboard_key("X")
        Y = get_keyboard_key("Y")
        Z = get_keyboard_key("Z")

        ALT = get_keyboard_key("ALT")
        RIGHT_ALT = get_keyboard_key("RIGHT_ALT")

        CTRL = get_keyboard_key("CTRL")
        RIGHT_CTRL = get_keyboard_key("RIGHT_CTRL")
        CAPS = get_keyboard_key("CAPS")

        SHIFT = get_keyboard_key("SHIFT")
        RIGHT_SHIFT = get_keyboard_key("RIGHT_SHIFT")

        BRACKET_LEFT = get_keyboard_key("BRACKET_LEFT")
        BRACKET_RIGHT = get_keyboard_key("BRACKET_RIGHT")

        SLASH = get_keyboard_key("SLASH")
        BACKSLASH = get_keyboard_key("BACKSLASH")

        UP = get_keyboard_key("UP")
        DOWN = get_keyboard_key("DOWN")
        LEFT = get_keyboard_key("LEFT")
        RIGHT = get_keyboard_key("RIGHT")

        RETURN = get_keyboard_key("RETURN")
        ESCAPE = get_keyboard_key("ESCAPE")
        BACKSPACE = get_keyboard_key("BACKSPACE")
        TAB = get_keyboard_key("TAB")
        SPACE = get_keyboard_key("SPACE")

        HASH = get_keyboard_key("HASH")
        PERCENT = get_keyboard_key("PERCENT")
        DOLLAR = get_keyboard_key("DOLLAR")
        AND = get_keyboard_key("AND")
        QUOTE = get_keyboard_key("QUOTE")

        LEFT_PARENTHESIS = get_keyboard_key("LEFT_PARENTHESIS")
        RIGHT_PARENTHESIS= get_keyboard_key("RIGHT_PARENTHESIS")

        ASTERISK = get_keyboard_key("ASTERISK")
        PLUS = get_keyboard_key("PLUS")
        MINUS = get_keyboard_key("MINUS")
        UNDERSCORE = get_keyboard_key("UNDERSCORE")

        COMMA = get_keyboard_key("COMMA")
        PERIOD = get_keyboard_key("PERIOD")
        SEMICOLON = get_keyboard_key("SEMICOLON")
        COLON = get_keyboard_key("COLON")

        LESS_THAN = get_keyboard_key("LESS_THAN")
        GREATER_THAN = get_keyboard_key("GREATER_THAN")

        EQUALS = get_keyboard_key("EQUALS")
        CIRCUMFLEX = get_keyboard_key("CIRCUMFLEX")

        EXCLAMATION_MARK = get_keyboard_key("EXCLAMATION_MARK")
        QUESTION_MARK = get_keyboard_key("QUESTION_MARK")

        F1 = get_keyboard_key("F1")
        F2 = get_keyboard_key("F2")
        F3 = get_keyboard_key("F3")
        F4 = get_keyboard_key("F4")
        F5 = get_keyboard_key("F5")
        F6 = get_keyboard_key("F6")
        F7 = get_keyboard_key("F7")
        F8 = get_keyboard_key("F8")
        F9 = get_keyboard_key("F9")
        F10 = get_keyboard_key("F10")
        F11 = get_keyboard_key("F11")
        F12 = get_keyboard_key("F12")

        AUDIO_MUTE = get_keyboard_key("AUDIO_MUTE")
        AUDIO_STOP = get_keyboard_key("AUDIO_STOP")
        AUDIO_PLAY = get_keyboard_key("AUDIO_PLAY")
        AUDIO_NEXT = get_keyboard_key("AUDIO_NEXT")
        AUDIO_PREV = get_keyboard_key("AUDIO_PREV")
        VOLUME_UP = get_keyboard_key("VOLUME_UP")
        VOLUME_DOWN = get_keyboard_key("VOLUME_DOWN")

        INSERT = get_keyboard_key("INSERT")
        PRINTSCREEN = get_keyboard_key("PRINTSCREEN")
        DELETE = get_keyboard_key("DELETE")
    end
    export_enum(KeyboardKey)

    """
    TODO
    """
    @enum MouseButton begin
    
        MOUSE_LEFT = get_mouse_button("MOUSE_LEFT")
        MOUSE_RIGHT = get_mouse_button("MOUSE_RIGHT")
        MOUSE_MIDDLE = get_mouse_button("MOUSE_MIDDLE")

        MOUSE_X1 = get_mouse_button("MOUSE_X1")
        MOUSE_X2 = get_mouse_button("MOUSE_X2")
    end
    export_enum(MouseButton)

    """
    TODO
    """
    @enum ControllerButton begin

        CONTROLLER_BUTTON_UNKNOWN = get_controller_button("CONTROLLER_BUTTON_UNKNOWN")

        CONTROLLER_BUTTON_A = get_controller_button("CONTROLLER_BUTTON_A")
        CONTROLLER_BUTTON_B = get_controller_button("CONTROLLER_BUTTON_B")
        CONTROLLER_BUTTON_X = get_controller_button("CONTROLLER_BUTTON_X")
        CONTROLLER_BUTTON_Y = get_controller_button("CONTROLLER_BUTTON_Y")

        CONTROLLER_LEFTSTICK_PRESS = get_controller_button("CONTROLLER_LEFTSTICK_PRESS")
        CONTROLLER_RIGHTSTICK_PRESS = get_controller_button("CONTROLLER_RIGHTSTICK_PRESS")
        CONTROLLER_LEFTSHOULDER = get_controller_button("CONTROLLER_LEFTSHOULDER")
        CONTROLLER_RIGHTSHOULDER = get_controller_button("CONTROLLER_RIGHTSHOULDER")

        CONTROLLER_DPAD_UP = get_controller_button("CONTROLLER_DPAD_UP")
        CONTROLLER_DPAD_DOWN = get_controller_button("CONTROLLER_DPAD_DOWN")
        CONTROLLER_DPAD_LEFT = get_controller_button("CONTROLLER_DPAD_LEFT")
        CONTROLLER_DPAD_RIGHT = get_controller_button("CONTROLLER_DPAD_RIGHT")

        CONTROLLER_SELECT = get_controller_button("CONTROLLER_SELECT")
        CONTROLLER_START = get_controller_button("CONTROLLER_START")

        CONTROLLER_GUIDE = get_controller_button("CONTROLLER_GUIDE")
        CONTROLLER_MAX = get_controller_button("CONTROLLER_MAX")

        CONTROLLER_XBOX_X_SHARE = get_controller_button("CONTROLLER_XBOX_X_SHARE")
        CONTROLLER_PS5_MIC = get_controller_button("CONTROLLER_PS5_MIC")
        CONTROLLER_SWITCH_CAPTURE = get_controller_button("CONTROLLER_SWITCH_CAPTURE")

        CONTROLLER_PADDLE_01 = get_controller_button("CONTROLLER_PADDLE_01")
        CONTROLLER_PADDLE_02 = get_controller_button("CONTROLLER_PADDLE_02")
        CONTROLLER_PADDLE_03 = get_controller_button("CONTROLLER_PADDLE_03")
        CONTROLLER_PADDLE_04 = get_controller_button("CONTROLLER_PADDLE_04")

        CONTROLLER_PS5_TOUCHPAD = get_controller_button("CONTROLLER_PS5_TOUCHPAD")
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
            finalizer(out) do x
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
            finalizer(out) do x
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




end
