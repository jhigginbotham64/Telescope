#
# Copyright 2022 Clemens Cords
# Created on 6/3/22 by clem (mail@clemens-cords.com)
#

module ts

    ### EXPORTS ###############################################################

    export initialize, set_framerate_limit!, start_frame!, end_frame!

    export Vector2, Vector2f, Vector2i, Vector2ui, Vector3, Vector3f, Vector3i, Vector3ui

    export Angle
    export degrees, radians, as_degrees, as_radians

    export RGBA, HSVA
    export as_hsv, as_rgb

    export Time, Clock
    export as_minutes, as_seconds, as_milliseconds, as_microseconds, as_nanoseconds,
        minutes, seconds, milliseconds, microseconds, nanoseconds, elapsed, restart

    export InputHandler, KeyboardKey, MouseButton, ControllerButton
    export is_down, has_state_changed, was_pressed, was_released, get_cursor_position,
        get_scrollwheel, get_controller_axis_left, get_controller_axis_right,
        get_controller_trigger_left, get_controller_trigger_right

    export Sound, SoundHandler
    export n_channels, play!, stop!, pause!, unpause!, force_stop!, is_playing, is_stopped,
        set_volume!, get_volume, set_panning!, get_panning

    export Music, MusicHandler
    export sample_rate, set_volume!, get_volume, play!, play_next!, stop!, next!, clear_next!,
        force_stop!, pause!, unpause!, skip_to!, is_playing, is_paused, is_stopped

    export Texture, RenderTexture, StaticTexture, TextureFilteringMode, TextureBlendMode
    export set_color!, get_color, set_blend_mode!, get_blend_mode, set_filtering_mode!,
        get_filtering_mode, get_size

    export Transform
    export apply_to, reset!, combine!, translate!, rotate!, scale!, shear!, reflect!

    export Triangle, Rectangle, Trapezoid, Circle
    export Shape, TriangleShape, RectangleShape, CircleShape, PolygonShape,
        set_centroid!, get_centroid, move!, set_color!, get_color, set_texture!,
        set_texture_rectangle!, get_texture_rectangle, get_bounding_box, get_n_vertices,
        set_vertex_position!, set_vertex_color!, set_vertex_texture_coordinate!,
        get_vertex_position, get_vertex_color, get_vertex_texture_coordinate, get_top_left,
        get_size, set_size!, set_top_left, get_radius, set_radius!

    export Window, WindowOptions
    export close!, is_open, get_size, get_position, set_hidden!, is_hidden, minimize!, is_minimized,
        maximize!, is_maximized, has_focus, has_mouse_focus, clear, render!, flush!

    export Camera
    export center_on!, move!, zoom_in!, zoom_out!, rotate!, set_rotation!, get_transform,
        get_center, get_view_area


    ### COMMON ################################################################

    const _lib = "./libtelescope.so"
    export _lib

    """
    `initialize() -> Bool`
    """
    function initialize() ::Bool
        return ccall((:ts_initialize, _lib), Bool, ())
    end
    export initialize

    ### VECTOR ################################################################

    """
    Vector2{<: Any}

    ## Members
    + x::T
    + y::T

    ## Constructors
    `Vector2{T}(::T, ::T)`
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
    Vector3{<: Any}

    ## Members
    + x::T
    + y::T
    + z::T

    ## Constructors
    `Vector3{T}(::T, ::T, ::T)`
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
    RGBA

    ## Members
    + red::Float32
    + green::Float32
    + blue::Float32
    + alpha::Float32

    ## Constructors
    `RGBA(r::Float32, g::Float32, b::Float32, a::Float32)`
    """
    struct RGBA

        red::Float32
        green::Float32
        blue::Float32
        alpha::Float32

        function RGBA(r::AbstractFloat, g::AbstractFloat, b::AbstractFloat, a::AbstractFloat)
            fclamp(x::AbstractFloat) = if x < 0 return 0 elseif x > 1 return 1 else return x end
            new(fclamp(r), fclamp(g), fclamp(b), fclamp(a))
        end
    end
    export RGBA
    
    """
    HSVA

    ## Members
    + hue::Float32
    + saturation::Float32
    + value::Float32
    + alpha::Float32

    ## Constructors
    `RGBA(h::Float32, s::Float32, v::Float32, a::Float32)`
    """
    struct HSVA

        hue::Float32
        saturation::Float32
        value::Float32
        alpha::Float32

        function RGBA(h::AbstractFloat, s::AbstractFloat, b::AbstractFloat, a::AbstractFloat)
            fclamp(x::AbstractFloat) = if x < 0 return 0 elseif x > 1 return 1 else return x end
            new(fclamp(h), fclamp(s), fclamp(v), fclamp(a))
        end
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
    Time

    ## Members
    (no public member)

    ## Constructors
    (no public constructors)
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
    Clock

    ## Members
    (no public members)

    ## Constructors
    `Clock()`
    """
    mutable struct Clock

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
    export elapsed

    """
    `restart!(::Clock) -> Time`
    """
    function restart!(clock::Clock) ::Time
        out_ns::Csize_t = ccall((:ts_clock_restart, _lib), Cdouble, (Csize_t,), clock._native_id)
        return nanoseconds(out_ns)
    end
    export restart!

    ### ANGLE ################################################################

    """
    Angle

    ## Members
    (no public members)

    ## Constructors
    (no public constructors)
    """
    struct Angle
        _degrees::Float32
    end
    export Angle

    """
    `degrees(::Float32) -> Angle`
    """
    function degrees(n::Float32) ::Angle
        return Angle(n % 360.0)
    end
    export degrees

    """
    `radians(::Float32) -> Angle`
    """
    function radians(n::Float32) ::Angle
        return Angle(ccall((:ts_radians_to_degree, _lib), Cfloat, (Cfloat,), n))
    end
    export radians

    """
    `as_degrees(::Angle) -> Float32`
    """
    function as_degrees(angle::Angle) ::Float32
        return angle._degrees % 360.0;
    end
    export as_degrees

    """
    `as_radians(::Angle) -> Float32`
    """
    function as_radians(angle::Angle) ::Float32
        return ccall((:ts_degrees_to_radians, _lib), Cfloat, (Cfloat,), angle._degrees)
    end
    export as_radians

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

    macro export_enum(enum_symbol::Symbol)

        enum = __module__.eval(enum_symbol)
        __module__.eval(Expr(:export, Symbol(enum_symbol)))
        for e in instances(enum)
            __module__.eval(Expr(:export, Symbol(e)))
        end
    end
    export @export_enum

    """
    enum KeyboardKey <: Int32
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
    @export_enum(KeyboardKey)

    """
    enum MouseButton <: Int32
    """
    @enum MouseButton begin
    
        MOUSE_LEFT = get_mouse_button_enum_value("MOUSE_LEFT")
        MOUSE_RIGHT = get_mouse_button_enum_value("MOUSE_RIGHT")
        MOUSE_MIDDLE = get_mouse_button_enum_value("MOUSE_MIDDLE")

        MOUSE_X1 = get_mouse_button_enum_value("MOUSE_X1")
        MOUSE_X2 = get_mouse_button_enum_value("MOUSE_X2")
    end
    @export_enum(MouseButton)

    """
    enum ControllerButton <: Int32
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
        # CONTROLLER_PS5_MIC = CONTROLLER_XBOX_X_SHARE
        # CONTROLLER_SWITCH_CAPTURE = CONTROLLER_XBOX_X_SHARE

        CONTROLLER_PADDLE_01 = get_controller_button_enum_value("CONTROLLER_PADDLE_01")
        CONTROLLER_PADDLE_02 = get_controller_button_enum_value("CONTROLLER_PADDLE_02")
        CONTROLLER_PADDLE_03 = get_controller_button_enum_value("CONTROLLER_PADDLE_03")
        CONTROLLER_PADDLE_04 = get_controller_button_enum_value("CONTROLLER_PADDLE_04")

        CONTROLLER_PS5_TOUCHPAD = get_controller_button_enum_value("CONTROLLER_PS5_TOUCHPAD")
    end
    @export_enum(ControllerButton)

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
        `get_cursor_position() -> Vector2f`
        """
        function get_cursor_position() ::Vector2f

            x = Ref{Cfloat}(0)
            y = Ref{Cfloat}(0)
            ccall((:ts_mouse_cursor_position, _lib), Cvoid, (Ref{Cfloat}, Ref{Cfloat}), x, y)
            return Vector2f(x, y)
        end
        export get_cursor_position

        """
        `get_scrollwheel() -> Vector2f`
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
        `get_controller_axis_left([::ControllerID]) -> Vector2f`
        """
        function get_controller_axis_left(id::ControllerID = 0) ::Vector2f

            x = Ref{Cfloat}(0)
            y = Ref{Cfloat}(0)
            ccall((:ts_controller_axis_left, _lib), Cvoid, (Ref{Cfloat}, Ref{Cfloat}), x, y)
            return Vector2f(x, y)

        end
        export get_controller_axis_left

        """
        `get_controller_axis_right([::ControllerID]) -> Vector2f`
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
    import Main.ts.InputHandler; using Main.ts.InputHandler

    ### SOUND ################################################################

    """
    Sound

    ## Members
    (no public members)

    ## Constructors
    `Sound(file_path::String, volume::Cfloat)`
    """
    mutable struct Sound

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
    export ChannelID

    """
    SoundHandler: control shorter sound clips by playing them in designated channels
    """
    module SoundHandler

        import Main.ts; using Main.ts

        """
        maximum channel index
        """
        const n_channels = ccall((:ts_sound_get_max_n_channels, _lib), Csize_t, ())
        export n_channels

        """
        `play!(::ChannelID, ::Sound, ::Integer, ::Time) -> Nothing`
        """
        function play!(channel::ChannelID, sound::Sound, n_loops::Integer, fade_in_duration::Time) ::Nothing
            ccall((:ts_sound_play, _lib), Cvoid,
                (Csize_t, Csize_t, Csize_t, Cdouble),
                sound._native_id, channel, n_loops, as_milliseconds(fade_in_duration))
        end
        export play
        
        """
        `stop!(::ChannelID, ::Time) -> Nothing`
        """
        function stop!(channel::ChannelID, fade_out_duration::Time) ::Nothing
            ccall((:ts_sound_stop, _lib), Cvoid,
                (Csize_t, Cdouble),
                channel, as_milliseconds(fade_out_duration))
        end
        export stop
        
        """
        `pause!(::ChannelID) -> Nothing`
        """
        function pause!(channel::ChannelID) ::Nothing
            ccall((:ts_sound_pause, _lib), Cvoid, (Csize_t,), channel)
        end
        export pause
        
        """
        `unpause!(::ChannelID) -> Nothing`
        """
        function unpause!(channel::ChannelID) ::Nothing
            ccall((:ts_sound_unpause, _lib), Cvoid, (Csize_t,), channel)
        end
        export unpause

        """
        `force_stop!(::ChannelID) -> Nothing`
        """
        function force_stop!(channel::ChannelID) ::Nothing
            ccall((:ts_sound_force_stop, _lib), Cvoid, (Csize_t,), channel)
        end
        export force_stop

        """
        `is_playing(::ChannelID) -> Bool`
        """
        function is_playing(channel::ChannelID) ::Bool
            return ccall((:ts_sound_is_playing, _lib), Bool, (Csize_t,), channel)
        end
        export is_playing

        """
        `is_paused(::ChannelID) -> Bool`
        """
        function is_paused(channel::ChannelID) ::Bool
            return ccall((:ts_sound_is_paused, _lib), Bool, (Csize_t,), channel)
        end
        export is_paused

        """
        `is_stopped(::ChannelID) -> Bool`
        """
        function is_stopped(channel::ChannelID) ::Bool
            return ccall((:ts_sound_is_stopped, _lib), Bool, (Csize_t,), channel)
        end
        export is_stopped

        """
        `set_volume!(::ChannelID, ::Float32) -> Nothing`
        """
        function set_volume!(channel::ChannelID, zero_to_one::Float32) ::Nothing
            ccall((:ts_sound_set_volume, _lib), Cvoid, (Csize_t, Cfloat), channel, zero_to_one)
        end
        export set_volume

        """
        `get_volume(::ChannelID) -> Float32`
        """
        function get_volume(channel::ChannelID) ::Float32
            return ccall((:ts_sound_get_volume, _lib), Cvoid, (Csize_t,), channel)
        end
        export get_volume

        """
        `set_panning!(::ChannelID, ::Angle) -> Nothing`
        """
        function set_panning!(channel::ChannelID, angle::Angle) ::Nothing
            return ccall((:ts_sound_set_panning, _lib), Cvoid, (Csize_t, Cfloat), channel, as_degrees(angle))
        end
        export set_panning

        """
        `get_panning(::ChannelID) -> Angle`
        """
        function get_panning(channel::ChannelID) ::Angle
            return degrees(ccall((:ts_sound_get_panning, _lib), Cfloat, (Csize_t,), channel))
        end
        export get_panning
    end
    import Main.ts.SoundHandler; using Main.ts.SoundHandler

    ### MUSIC ################################################################

    """
    Music

    ## Members
    (no public members)

    ## Constructors
    `Music(path::String)`
    """
    mutable struct Music

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
    MusicHandler: control playback of longer sound files, only one can be active at a time
    """
    module MusicHandler

        import Main.ts; using Main.ts

        """
        sample rate of music sound files
        """
        const sample_rate = ccall((:ts_music_sample_rate, _lib), Csize_t, ())
        export sample_rate

        """
        `set_volume!(::Float32) -> Nothing`
        """
        function set_volume!(zero_to_one::Float32) ::Nothing
            ccall((:ts_music_set_volume, _lib), Cvoid, (Cfloat,), zero_to_one)
        end
        export set_volume

        """
        `get_volume() -> Float32`
        """
        function get_volume() ::Float32
            ccall((:ts_music_get_volume, _lib), Cfloat, ())
        end
        export get_volume

        """
        `play!(::Music, ::Bool, ::Time) -> Nothing`
        """
        function play!(music::Music, should_loop::Bool, fade_in_duration::Time) ::Nothing
            ccall((:ts_music_play, _lib), Cvoid,
                (Csize_t, Bool, Cdouble),
                music._native_id, should_loop, as_milliseconds(fade_in_duration))
        end
        export play

        """
        `play_next!(::Music, ::Bool, ::Time) -> Nothing
        """
        function play_next!(music::Music, should_loop::Bool, fade_in_duration::Time) ::Nothing
            ccall((:ts_music_play_next, _lib), Cvoid,
                (Csize_t, Bool, Cdouble),
                music._native_id, should_loop, as_milliseconds(fade_in_duration))
        end
        export play_next

        """
        `stop!(::Time) -> Nothing`
        """
        function stop!(fade_out_duration::Time) ::Nothing
            ccall((:ts_music_stop, _lib), Cvoid, (Cdouble,), as_milliseconds(fade_out_duration))
        end
        export stop

        """
        `next!(::Time) -> Nothing`
        """
        function next!(fade_out_duration::Time) ::Nothing
            ccall((:ts_music_next, _lib), Cvoid, (Cdouble,), as_milliseconds(fade_out_duration))
        end
        export next

        """
        `clear_next!() -> Nothing`
        """
        function clear_next!() ::Nothing
            ccall((:ts_music_clear_next, _lib), Cvoid, ())
        end
        export clear_next

        """
        `force_stop!() -> Nothing`
        """
        function force_stop!() ::Nothing
            ccall((:ts_music_force_stop, _lib), Cvoid, ())
        end
        export force_stop

        """
        `pause!() -> Nothing`
        """
        function pause!() ::Nothing
            ccall((:ts_music_pause, _lib), Cvoid, ())
        end
        export pause

        """
        `unpause!() -> Nothing`
        """
        function unpause!() ::Nothing
            ccall((:ts_music_unpause, _lib), Cvoid, ())
        end
        export unpause

        """
        `skip_to!(::Time) -> Nothing`
        """
        function skip_to!(timestamp::Time) ::Nothing
            ccall((:ts_music_skip_to, _lib), Cvoid, (Cdouble,), as_milliseconds(timestamp))
        end
        export skip_to

        """
        `is_playing() -> Bool`
        """
        function is_playing() ::Bool
            return ccall((:ts_music_is_playing, _lib), Bool, ())
        end
        export is_playing

        """
        `is_paused() -> Bool`
        """
        function is_paused() ::Bool
            return ccall((:ts_music_is_paused, _lib), Bool, ())
        end
        export is_paused

        """
        `is_stopped() -> Bool`
        """
        function is_stopped() ::Bool
            return ccall((:ts_music_is_stopped, _lib), Bool, ())
        end
        export is_stopped
    end

    ### TEXTURES ##############################################################

    """
    enum TextureFilteringMode <: Int32
    """
    @enum TextureFilteringMode begin

        NEAREST_NEIGHBOUR = ccall((:ts_texture_filtering_mode_nearest_neighbour, _lib), Cint, ())
        LINEAR = ccall((:ts_texture_filtering_mode_linear, _lib), Cint, ())
        ANISOTROPIC = ccall((:ts_texture_filtering_mode_anisotropic, _lib), Cint, ())
    end
    @export_enum(TextureFilteringMode)

    """
    enum TextureBlendMode <: Int32
    """
    @enum TextureBlendMode begin

        NONE = ccall((:ts_texture_blend_mode_none, _lib), Cint, ())
        ALPHA = ccall((:ts_texture_blend_mode_alpha, _lib), Cint, ())
        ADD = ccall((:ts_texture_blend_mode_add, _lib), Cint, ())
        MULTIPLY = ccall((:ts_texture_blend_mode_multiply, _lib), Cint, ())
    end
    @export_enum(TextureBlendMode)

    """
    Texture (Abstract Interface)

    ## Expected Members
    + _native::Ptr{Cvoid}
    """
    abstract type Texture end
    export Texture

    """
    AbstractWindow (Abstract Interface)
    
    ## Expected Members
    + _native_id::Csize_t
    """
    abstract type AbstractWindow end
    # no export

    """
    RenderTexture

    ## Members
    (no public members)

    ## Constructors
    `RenderTexture(::Window, width::Unsigned, height::Unsigned)`
    """
    mutable struct RenderTexture <: Texture

        _native::Ptr{Cvoid}

        # equivalent to ts::RenderTexture::create
        function RenderTexture(window::AbstractWindow, width::Unsigned, height::Unsigned)

            native = ccall((:ts_texture_create_render_texture, _lib),
                    Ptr{Cvoid}, (Csize_t, Csize_t, Csize_t),
                    window._native_id, width, height)

            out = new(native)
            finalizer(out) do x::RenderTexture
                ccall((:ts_texture_destroy_texture, _lib), Cvoid, (Ptr{Cvoid},), x._native)
            end
        end
    end
    export RenderTexture

    """
    `render(::`
    """

    """
    StaticTexture

    ## Members
    (no public members)

    ##
    `StaticTexture(::Window, path::String)`
    `StaticTexture(::Window, width::Unsigned, height::Unsigned, ::RGBA)`
    """
    mutable struct StaticTexture <: Texture

        _native::Ptr{Cvoid}

        # equivalent to ts::StaticTexture::load
        function StaticTexture(window::AbstractWindow, path::String)

            native = ccall((:ts_texture_load_static_texture, _lib),
                Ptr{Cvoid}, (Csize_t, Csize_t, Csize_t),
                window._native_id, width, height)

            out = new(native)
            finalizer(out) do x::RenderTexture
                ccall((:ts_texture_destroy_texture, _lib), Cvoid, (Ptr{Cvoid},), x._native)
            end
        end

        # equivalent to ts::StaticTexture::create
        function StaticTexture(window::AbstractWindow, width::Unsigned, height::Unsigned, color::RGBA)

            native = ccall((:ts_texture_create_static_texture, _lib),
                Ptr{Cvoid}, (Csize_t, Csize_t, Csize_t),
                window._native_id, width, height)

            out = new(native)
            finalizer(out) do x::RenderTexture
                ccall((:ts_texture_destroy_texture, _lib), Cvoid, (Ptr{Cvoid},), x._native)
            end
        end
    end
    export StaticTexture

    """
    `set_color!(::Texture) -> Nothing`
    """
    function set_color!(texture::Texture, color::RGBA) ::Nothing
        ccall((:ts_texture_set_color, _lib), Cvoid,
            (Ptr{Cvoid}, Cfloat, Cfloat, Cfloat, Cfloat),
            texture._native, color.red, color.green, color.blue, color.alpha)
    end
    export set_color!
    
    """
    `get_color(::Texture) -> RGBA`
    """
    function get_color(texture::Texture) ::RGBA

        r = Ref{Cfloat}(-1)
        g = Ref{Cfloat}(-1)
        b = Ref{Cfloat}(-1)
        a = Ref{Cfloat}(-1)

        ccall((:ts_texture_get_color, _lib), Cvoid,
            (Ptr{Cvoid}, Ref{Cfloat}, Ref{Cfloat}, Ref{Cfloat}, Ref{Cfloat}),
            texture._native, r, g, b, a)

        return RGBA(r[], g[], b[], a[])
    end
    export get_color
    
    """
    `set_blend_mode!(::Texture, ::TextureBlendMode) -> RGBA`
    """
    function set_blend_mode!(texture::Texture, mode::TextureBlendMode) ::Nothing
        ccall((:ts_texture_set_blend_mode, _lib), Cvoid,
            (Ptr{Cvoid}, Cint),
            texture._native,
            convert(Cint, mode))
    end
    export set_blend_mode!

    """
    `get_blend_mode(::Texture) -> TextureBlendMode`
    """
    function get_blend_mode(texture::Texture) ::TextureBlendMode
        out::Cint = ccall((:ts_texture_get_blend_mode, _lib), Cint, (Ptr{Cvoid},), texture._native)
        return convert(TextureBlendMode, out)
    end
    export get_blend_mode

    """
    `set_filtering_mode!(::Texture, ::TextureFilteringMode) -> Nothing`
    """
    function set_filtering_mode!(texture::Texture, mode::TextureFilteringMode) ::Nothing
        ccall((:ts_texture_set_filtering_mode, _lib), Cvoid,
            (Ptr{Cvoid}, Cint),
            texture._native,
            convert(Cint, mode))
    end
    export set_filtering_mode!

    """
    `get_filtering_mode(::Texture) -> TextureFilteringMode`
    """
    function get_filtering_mode(texture::Texture) ::TextureFilteringMode
        out::Cint = ccall((:ts_texture_get_filtering_mode, _lib), Cint, (Ptr{Cvoid},), texture._native)
        return convert(TextureBlendMode, out)
    end
    export get_filtering_mode

    """
    `get_size(::Texture) -> Vector2ui`
    """
    function get_size(texture::Texture) ::Vector2ui

        x = Ref{Csize_t}(0)
        y = Ref{Csize_t}(0)

        ccall((:ts_texture_get_size, _lib), Cvoid,
            (Ptr{Cvoid}, Ref{Cfloat}, Ref{Cfloat}),
            texture._native, x, y)

        return Vector2f(x[], y[])

    end
    export get_size

    ### TRANSFORM #############################################################

    """
    Transform

    ## Members
    (no public members)

    ## Constructors
    `Transform()`
    """
    mutable struct Transform

        _native::Ptr{Cvoid}

        function Transform()

            ptr = ccall((:ts_transform_create, _lib), Ptr{Cvoid}, ())
            out = new(ptr)
            finalizer(out) do x::Transform
                ccall((:ts_transform_destroy, _lib), Cvoid, (Ptr{Cvoid},), x._native)
            end
        end
    end
    export Transform

    """
    `getindex(::Transform, ::Integer, ::Integer) -> Float32`
    """
    function Base.getindex(transform::Transform, x::Integer, y::Integer) ::Float32

        return ccall((:ts_transform_get, _lib), Cfloat,
            (Ptr{Cvoid}, Csize_t, Csize_t),
            transform._native, x, y)
    end

    """
    `setindex!(::Transform, ::Float32, ::Integer, ::Integer) -> Float32`
    """
    function Base.setindex!(transform::Transform, value::Float32, x::Integer, y::Integer) ::Nothing

        ccall((:ts_transform_set, _lib), Cvoid,
            (Ptr{Cvoid}, Csize_t, Csize_t, Cfloat),
            transform._native, x, y, value)
        return nothing
    end

    """
    `apply_to(::Transform, ::Vector2f) -> Vector2f`
    """
    function apply_to(transform::Transform, point::Vector2f) ::Vector2f

        x = Ref{Float32}(0)
        y = Ref{Float32}(0)
        ccall((:ts_transform_apply_to, _lib), Cvoid,
            (Ptr{Cvoid}, Cfloat, Cfloat, Ref{Cfloat}, Ref{Cfloat}),
            transform._native, point.x, point.y, x, y)

        return Vector2f{x[], y[]}
    end
    export apply_to

    """
    `reset!(::Transform) -> Nothing`
    """
    function reset!(transform::Transform) ::Nothing
        ccall((:ts_transform_reset, _lib), Cvoid, (Ptr{Cvoid},), transform._native)
    end
    export reset!

    """
    `combine!(::Transform, ::Transform) -> Nothing`
    """
    function combine!(left::Transform, right::Transform) ::Nothing
        ccall((:ts_transform_combin, _lib), Cvoid, (Ptr{Cvoid}, Ptr{Cvoid}), left._native, right._native)
    end
    export combine!

    """
    `translate!(::Transform, ::Float32, ::Float32) -> Nothing`
    """
    function translate!(transform::Transform, x::Float32, y::Float32) ::Nothing
        ccall((:ts_transform_translate, _lib), Cvoid, (Ptr{Cvoid}, Cfloat, Cfloat), transform._native, x, y)
    end
    export translate!

    """
    `rotate!(::Transform, ::Angle, ::Vector2f) -> Nothing`
    """
    function rotate!(transform::Transform, angle::Angle, origin::Vector2f) ::Nothing
        ccall((:ts_transform_rotate, _lib), Cvoid,
            (Ptr{Cvoid}, Cfloat, Cfloat, Cfloat),
            transform._native, as_degrees(angle), origin.x, origin.y)
    end
    export rotate!

    """
    `scale!(::Transform, ::Float32, ::Float32) -> Nothing`
    """
    function scale!(transform::Transform, x_scale::Float32, y_scale::Float32) ::Nothing
        ccall((:ts_transform_scale, _lib), Cvoid,
            (Ptr{Cvoid}, Cfloat, Cfloat),
            transform._native, x_scale, y_scale)
    end
    export scale!

    """
    `shear!(::Transform, ::Float32, ::Float32) -> Nothing`
    """
    function shear!(transform::Transform, x_scale::Float32, y_scale::Float32) ::Nothing
        ccall((:ts_transform_shear, _lib), Cvoid,
            (Ptr{Cvoid}, Cfloat, Cfloat),
            transform._native, x_scale, y_scale)
    end
    export shear!

    """
    `reflect!(::Transform, ::Bool, ::Bool, ::Vector2f) -> Nothing`
    """
    function reflect!(transform::Transform, about_x_axis::Bool, about_y_axis::Bool, origin::Vector2f) ::Nothing
        ccall((:ts_transform_reflect, _lib), Cvoid,
            (Ptr{Cvoid}, Bool, Bool, Cfloat, Cfloat),
            transform._native, about_x_axis, about_y_axis, origin.x, origin.y)
    end
    export reflect!

    ### GEOMETRY ##############################################################

    """
    Triangle

    ## Members
    + a::Vector2f
    + b::Vector2f
    + c::Vector2f

    ## Constructors
    `Triangle(::Vector2f, ::Vector2f, ::Vector2f)`
    """
    struct Triangle

        a::Vector2f
        b::Vector2f
        c::Vector2f
    end
    export Triangle

    """
    Rectangle

    ## Members
    + top_left::Vector2f
    + size::Vector2f

    ## Constructors
    `Rectangle(::Vector2f, ::Vector2f)`
    """
    struct Rectangle

        top_left::Vector2f
        size::Vector2f
    end
    export Rectangle

    """
    Trapezoid

    ## Members
    + top_left::Vector2f
    + top_right::Vector2f
    + bottom_left::Vector2f
    + bottom_right::Vector2f

    ## Constructors
    `Trapezoid(::Vector2f, ::Vector2f, ::Vector2f, ::Vector2f)`
    """
    struct Trapezoid

        top_left::Vector2f
        top_right::Vector2f
        bottom_left::Vector2f
        bottom_right::Vector2f
    end
    export Trapezoid

    """
    Circle

    ## Members
    + center::Vector2f
    + radius::Float32

    ## Constructors
    `Circle(::Vector2f, ::Float32)`
    """
    struct Circle

        center::Vector2f
        radius::Float32
    end
    export Circle

    ### SHAPE #################################################################

    """
    Shape (Abstract Interface)

    ## Expected Members
    + _native::Ptr{Cvoid}
    """
    abstract type Shape end
    export Shape

    """
    `set_centroid!(::Shape, ::Vector2f) -> Nothing`
    """
    function set_centroid!(shape::Shape, position::Vector2f) ::Nothing

        ccall((:ts_shape_set_centroid, _lib), Cvoid,
            (Ptr{Cvoid}, Cfloat, Cfloat),
            shape._native, position.x, position.y)
    end
    export set_centroid!

    """
    `get_centroid(::Shape) -> Vector2f`
    """
    function get_centroid(shape::Shape) ::Vector2f

        x = Ref{Cint}(0)
        y = Ref{Cint}(0)

        ccall((:ts_shape_get_centroid, _lib), Cvoid,
            (Ptr{Cvoid}, Ref{Cint}, Ref{Cint}),
            shape._native, x, y)
    end
    export get_centroid

    """
    `move!(::Shape, ::Float32, ::Float32) -> Nothing`
    """
    function move!(shape::Shape, x::Float32, y::Float32) ::Nothing
        ccall((:ts_shape_move, _lib), Cvoid, (Ptr{Cvoid}, Cfloat, Cfloat), shape._native, x, y)
    end
    export move!

    """
    `set_color!(::Shape, ::RGBA) -> Nothing`
    """
    function set_color!(shape::Shape, color::RGBA) ::Nothing

        ccall((:ts_shape_set_color, _lib), Cvoid,
            (Ptr{Cvoid}, Cfloat, Cfloat, Cfloat, Cfloat),
            shape._native, color.red, color.green, color.blue, color.alpha)
    end
    export set_color!

    """
    `get_color(::Shape) -> RGBA`
    """
    function get_color(shape::Shape, vertex_index::UInt64) ::RGBA

        r = Ref{Cfloat}(-1)
        g = Ref{Cfloat}(-1)
        b = Ref{Cfloat}(-1)
        a = Ref{Cfloat}(-1)

        ccall((:ts_shape_get_vertex_color, _lib), Cvoid,
            (Ptr{Cvoid}, Csize_t, Ref{Cfloat}, Ref{Cfloat}, Ref{Cfloat}, Ref{Cfloat}),
            shape._native, vertex_index, r, g, b, a)

        return RGBA(r[], g[], b[], a[])
    end
    export get_color

    """
    `set_texture!(::Shape, ::Texture) -> Nothing`
    """
    function set_texture!(shape::Shape, texture::Texture) ::Nothing
        ccall((:ts_shape_set_texture, _lib), Cvoid,
            (Ptr{Cvoid}, Ptr{Cvoid}),
            shape._native, texture._native)
    end
    export set_texture!

    """
    `set_texture_rectangle!(::Shape, ::Rectangle) -> Nothing`
    """
    function set_texture_rectangle!(shape::Shape, rect::Rectangle) ::Nothing
        ccall((:ts_shape_set_texture_rectangle, _lib), Cvoid,
            (Ptr{Cvoid}, Cfloat, Cfloat, Cfloat, Cfloat),
            shape._native, rect.top_left.x, rect.top_left.y, rect.size.x, rect.size.y)
    end
    export set_texture_rectangle!

    """
    `get_texture_rectangle(::Shape) -> Rectangle`
    """
    function get_texture_rectangle(shape::Shape) ::Rectangle

        x = Ref{Cfloat}(-1)
        y = Ref{Cfloat}(-1)
        width = Ref{Cfloat}(-1)
        height = Ref{Cfloat}(-1)

        ccall((:ts_shape_get_texture_rectangle, _lib), Cvoid,
            (Ptr{Cvoid}, Ref{Cfloat}, Ref{Cfloat}, Ref{Cfloat}, Ref{Cfloat}),
            shape._native, x, y, width, height)

        return Rectangle(Vector2f(x[], y[]), Vector2f(width[], height[]))
    end
    export get_texture_rectangle

    """
    `get_bounding_box(::Shape) -> Rectangle`
    """
    function get_bounding_box(shape::Shape) ::Rectangle

        x = Ref{Cfloat}(-1)
        y = Ref{Cfloat}(-1)
        width = Ref{Cfloat}(-1)
        height = Ref{Cfloat}(-1)

        ccall((:ts_shape_get_texture_rectangle, _lib), Cvoid,
            (Ptr{Cvoid}, Ref{Cfloat}, Ref{Cfloat}, Ref{Cfloat}, Ref{Cfloat}),
            shape._native, x, y, width, height)

        return Rectangle(Vector2f(x[], y[]), Vector2f(width[], height[]))
    end
    export get_bounding_box

    """
    `get_n_vertices(::Shape) -> UInt64`
    """
    function get_n_vertices(shape::Shape) ::UInt64
        return ccall((:ts_shape_get_n_vertices, _lib), Csize_t, (Ptr{Cvoid},), shape._native)
    end
    export get_n_vertices

    """
    `set_vertex_position!(::Shape, ::Integer, ::Vector2f) -> Nothing`
    """
    function set_vertex_position!(shape::Shape, index::Integer, position::Vector2f) ::Nothing
        ccall((:ts_shape_set_vertex_position, _lib), Cvoid,
            (Ptr{Cvoid}, Csize_t, Cfloat, Cfloat),
            shape._native, convert(Csize_t, index), position.x, position.y)
    end
    export set_vertex_position!

    """
    `set_vertex_color!(::Shape, ::Integer, ::RGBA) -> Nothing`
    """
    function set_vertex_color!(shape::Shape, index::Integer, color::RGBA) ::Nothing
        ccall((:ts_shape_set_vertex_color, _lib), Cvoid,
            (Ptr{Cvoid}, Csize_t, Cfloat, Cfloat, Cfloat, Cfloat),
            shape._native, convert(Csize_t, index), color.red, color.green, color.blue, color.alpha)
    end
    export set_vertex_color!

    """
    `set_vertex_texture_coordinate!(::Shape, ::Integer, ::Vector2f) -> Nothing`
    """
    function set_vertex_texture_coordinate!(shape::Shape, index::Integer, coordinate::Vector2f) ::Nothing
        ccall((:ts_shape_set_vertex_texture_coordinate, _lib), Cvoid,
            (Ptr{Cvoid}, Csize_t, Cfloat, Cfloat),
            shape._native, convert(Csize_t, index), coordinate.x, coordinate.y)
    end
    export set_vertex_texture_coordinate!

    """
    `get_vertex_position(shape::Shape, index::Integer) -> Vector2f`
    """
    function get_vertex_position(shape::Shape, index::Integer) ::Vector2f

        x = Ref{Cfloat}(-1)
        y = Ref{Cfloat}(-1)

        ccall((:ts_shape_get_vertex_position, _lib), Cvoid,
            (Ptr{Cvoid}, Csize_t, Ref{Cfloat}, Ref{Cfloat}),
            shape._native, convert(Csize_t, index), x, y)

        return Vector2f(x[], y[])
    end
    export get_vertex_position

    """
    `get_vertex_color(shape::Shape, index::Integer) -> RGBA`
    """
    function get_vertex_color(shape::Shape, index::Integer) ::RGBA

        r = Ref{Cfloat}(-1)
        g = Ref{Cfloat}(-1)
        b = Ref{Cfloat}(-1)
        a = Ref{Cfloat}(-1)

        ccall((:ts_get_vertex_color, _lib), Cvoid,
            (Ptr{Cvoid}, Csize_t, Ref{Cfloat}, Ref{Cfloat}, Ref{Cfloat}, Ref{Cfloat}),
            shape._native, convert(Csize_t, index), r, g, b, a)

        return RGBA(r[], g[], b[], a[])
    end
    export get_vertex_color

    """
    `get_vertex_texture_coordinate(shape::Shape, index::Integer) -> Vector2f`
    """
    function get_vertex_texture_coordinate(shape::Shape, index::Integer) ::Vector2f

        x = Ref{Cfloat}(-1)
        y = Ref{Cfloat}(-1)

        ccall((:ts_shape_get_vertex_texture_coordinate, _lib), Cvoid,
            (Ptr{Cvoid}, Csize_t, Ref{Cfloat}, Ref{Cfloat}),
            shape._native, convert(Csize_t, index), x, y)

        return Vector2f(x[], y[])
    end
    export get_vertex_texture_coordinate

    ### TRIANGLE SHAPE ########################################################

    """
    TriangleShape

    ## Members
    (no public members)

    ## Constructors
    `TriangleShape(::Vector2f, ::Vector2f, ::Vector2f)`
    """
    mutable struct TriangleShape

        _native::Ptr{Cvoid}

        function TriangleShape(a::Vector2f, b::Vector2f, c::Vector2f)

            native = ccall((:ts_shape_create_triangle, _lib), Ptr{Cvoid},
                (Cfloat, Cfloat, Cfloat, Cfloat, Cfloat, Cfloat),
                a.x, a.y, b.x, b.y, c.x, c.y)

            out = new(native)
            finalizer(out) do x::TriangleShape
                ccall((:ts_shape_destroy_triangle, _lib), Cvoid, (Ptr{Cvoid},), x._native)
            end

            return out
        end
    end
    export TriangleShape

    ### RECTANGLE SHAPE #######################################################

    """
    RectangleShape

    ## Members
    (no public members)

    ## Constructors
    `RectangleShape(top_left::Vector2f, size::Vector2f)`
    """
    mutable struct RectangleShape

        _native::Ptr{Cvoid}

        function RectangleShape(top_left::Vector2f, size::Vector2f)

            native = ccall((:ts_shape_create_rectangle, _lib), Ptr{Cvoid},
                (Cfloat, Cfloat, Cfloat, Cfloat),
                top_left.x, top_left.y, size.x, size.y)

            out = new(native)
            finalize(out) do x::RectangleShape
                ccall((:ts_shape_destroy_rectangle, _lib), Cvoid, (Ptr{Cvoid},), x._native)
            end
        end
    end
    export RectangleShape

    """
    `set_top_left!(::RectangleShape, ::Vector2f) -> Nothing`
    """
    function set_top_left!(rect::RectangleShape, position::Vector2f) ::Nothing
        ccall((:ts_shape_rectangle_set_top_left, _lib), Cvoid,
            (Ptr{Cvoid}, Cfloat, Cfloat),
             rect._native, position.x, position.y)
    end
    export set_top_left!

    """
    `get_top_left(::RectangleShape) -> Vector2f`
    """
    function get_top_left(rect::RectangleShape) ::Vector2f

        x = Ref{Cfloat}(-1)
        y = Ref{Cfloat}(-1)
        ccall((:ts_shape_rectangle_get_top_left, _lib), Cvoid,
            (Ptr{Cvoid}, Ref{Cfloat}, Ref{Cfloat}),
            rect._native, x, y)

        return Vector2f(x[], y[])
    end
    export get_top_left

     """
    `set_size!(::RectangleShape, ::Vector2f) -> Nothing`
    """
    function set_size!(rect::RectangleShape, size::Vector2f) ::Nothing
        ccall((:ts_shape_rectangle_set_size, _lib), Cvoid,
            (Ptr{Cvoid}, Cfloat, Cfloat),
            rect._native, size.x, size.y)
    end
    export set_size!

    """
    `get_size(::RectangleShape) -> Vector2f`
    """
    function get_size(rect::RectangleShape) ::Vector2f

        x = Ref{Cfloat}(-1)
        y = Ref{Cfloat}(-1)
        ccall((:ts_shape_rectangle_get_size, _lib), Cvoid,
            (Ptr{Cvoid}, Ref{Cfloat}, Ref{Cfloat}),
            rect._native, x, y)
    end
    export get_size

    ### CIRCLE SHAPE ##########################################################

    """
    CircleShape

    ## Members
    (no public members)

    ## Constructors
    `CircleShape(center::Vector2f, radius::Float32, [n_vertices::UInt64])
    """
    mutable struct CircleShape

        _native::Ptr{Cvoid}

        function CircleShape(center::Vector2f, radius::Float32, n_vertices::Csize_t = 32)

            native = ccall((:ts_shape_create_circle, _lib), Ptr{Cvoid},
                (Cfloat, Cfloat, Cfloat, Csize_t),
                center.x, center.y, radius, n_vertices)

            out = new(native)
            finalizer(out) do x::CircleShape
                ccall((:ts_shape_destroy_circle, _lib), Cvoid, (Ptr{Cvoid},), x._native)
            end
        end
    end
    export CircleShape

    """
    `get_radius(::CircleShape) -> Float32`
    """
    function get_radius(circle::CircleShape) ::Float32
        return ccall((:ts_shape_circle_get_radius, _lib), Cfloat, (Ptr{Cvoid},), circle._native)
    end
    export get_radius

    """
    `set_radius!(::CircleShape) -> Float32`
    """
    function set_radius!(circle::CircleShape, radius::Float32) ::Nothing
       ccall((:ts_shape_circle_set_radius, _lib), Cvoid,
        (Ptr{Cvoid}, Cfloat), circle._native, radius)
    end
    export set_radius!

    ### POLYGON SHAPE #########################################################

    """
    PolygonShape

    ## Members
    (no public members)

    ## Constructors
    `PolygonShape(points::Vector2f...)`
    """
    mutable struct PolygonShape

        _native::Ptr{Cvoid}

        function PolygonShape(positions::Vector2f...)

            n = length(positions)

            xs = Cfloat[]
            ys = Cfloat[]

            sizehint!(xs, n)
            sizehint!(ys, n)

            for pos in positions
                push!(xs, pos.x)
                push!(ys, pos.y)
            end

            native = ccall((:ts_shape_create_polygon, _lib), Ptr{Cvoid},
                (Ptr{Cfloat}, Ptr{Cfloat}, Csize_t),
                pointer_from_objret(xs), pointer_from_objref(ys), n)

            out = new(native)
            finalize(out) do x::PolygonShape
                ccall((:ts_shape_destroy_polygon, _lib), Cvoid, (Ptr{Cvoid},), x._native)
            end
        end
    end
    export PolygonShape

    ### WINDOW ################################################################

    """
    enum WindowOptions <: UInt32
    """
    @enum WindowOptions begin

        DEFAULT = UInt32(0)
        FULLSCREEN = UInt32(1) << 1
        BORDERLESS = UInt32(1) << 2
        RESIZABLE  = UInt32(1) << 3
    end
    @export_enum(WindowOptions)

    """
    Window

    ## Members
    (no public members)

    ## Constructors
    `Window(width::Integer, height::Integer, title::String, [options::UInt32])`
    """
    mutable struct Window <: AbstractWindow

        _native_id::Csize_t

        function Window(width::Integer, height::Integer, title::String, options::UInt32 = UInt32(DEFAULT))
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
    `close!(::Window) -> Nothing`
    """
    function close!(window::Window) ::Nothing
        ccall((:ts_window_close, _lib), Cvoid, (Csize_t,), window._native_id)
    end
    export close

    """
    `is_open(::Window) -> Bool`
    """
    function is_open(window::Window) ::Bool
        ccall((:ts_window_is_open, _lib), Bool, (Csize_t,), window._native_id)
    end
    export is_open

    """
    `get_size(::Window) -> Vector2ui`
    """
    function get_size(window::Window) ::Vector2ui

        x = Ref{Csize_t}(-1)
        y = Ref{Csize_t}(-1)
        ccall((:ts_window_get_size, _lib), Cvoid, (Ref{Csize_t}, Ref{Csize_t}), x, y)
        return Vector2ui(x[], y[])
    end
    export get_size

    """
    `get_position(::Window) -> Vector2ui`
    """
    function get_position(window::Window) ::Vector2ui

        x = Ref{Csize_t}(-1)
        y = Ref{Csize_t}(-1)
        ccall((:ts_window_get_position, _lib), Cvoid, (Ref{Csize_t}, Ref{Csize_t}), x, y)
        return Vector2ui(x[], y[])
    end
    export get_position

    """
    `set_hidden!(::Window, ::Bool) -> Nothing
    """
    function set_hidden!(window::Window, hidden::Bool) ::Nothing
        ccall((:ts_window_set_hidden, _lib), Cvoid, (Csize_t, Bool), window._native_id, hidden)
    end
    export set_hidden

    """
    `is_hidden(::Window) -> Bool`
    """
    function is_hidden(window::Window) ::Bool
        return ccall((:ts_window_is_hidden, _lib), Bool, (Csize_t,), window._native_id)
    end
    export is_hidden

    """
    `minimize!(::Window) -> Nothing`
    """
    function minimize!(window::Window) ::Nothing
        ccall((:ts_window_minimize, _lib), Cvoid, (Csize_t,), window._native_id)
    end
    export minimize

    """
    `is_minimized(::Window) -> Bool`
    """
    function is_minimized(window::Window) ::Bool
        return ccall((:ts_window_is_minimized, _lib), Bool, (Csize_t,), window._native_id)
    end
    export is_minimizedd

    """
    `maximize!(::Window) -> Nothing`
    """
    function maximize!(window::Window) ::Nothing
        ccall((:ts_window_maximize, _lib), Cvoid, (Csize_t,), window._native_id)
    end
    export maximize

    """
    `is_maximized(::Window) -> Bool`
    """
    function is_maximized(window::Window) ::Bool
        return ccall((:ts_window_is_maximized, _lib), Bool, (Csize_t,), window._native_id)
    end
    export is_maximized

    """
    `has_focus(::Window) -> Bool``
    """
    function has_focus(window::Window) ::Bool
        return ccall((:ts_window_has_focus, _lib), Bool, (Csize_t,), window._native_id)
    end
    export has_focus

    """
    `has_mouse_focus(::Window) -> Bool`
    """
    function has_mouse_focus(window::Window) ::Bool
        return ccall((:ts_window_has_mouse_focus, _lib), Bool, (Csize_t,), window._native_id)
    end
    export has_mouse_focus

    """
    `clear!(::Window) -> Nothing`
    """
    function clear!(window::Window) ::Nothing
        ccall((:ts_window_clear, _lib), Cvoid, (Csize_t,), window._native_id)
    end
    export clear

    """
    `render!(::Window, ::Shape, ::Transform) -> Nothing`
    """
    function render!(window::Window, shape::Shape_t, transform::Transform) ::Nothing where Shape_t <: Shape
        ccall((:ts_shape_render_to_window, _lib), Cvoid,
            (Ptr{Cvoid}, Csize_t, Ptr{Cvoid}),
            shape._native, window._native_id, transform._native)
    end
    export render!

    """
    `render!(::RenderTexture, ::Shape, ::Transform) -> Nothing`
    """
    function render!(render_texture::RenderTexture, shape::Shape_t, transform::Transform) ::Nothing where Shape_t <: Shape
        ccall((:ts_shape_render_to_texture, _lib), Cvoid,
            (Ptr{Cvoid}, Ptr{Cvoid}, Ptr{Cvoid}),
            shape._native, render_texture._native, transform._native)
    end
    export render!

    """
    `flush!(::Window) -> Nothing`
    """
    function flush!(window::Window) ::Nothing
        ccall((:ts_window_flush, _lib), Cvoid, (Csize_t,), window._native_id)
    end
    export flush!

    """
    `set_icon(::Window, ::String) -> Nothing`
    """
    function set_icon!(window::Window, path::String) ::Nothing
        ccall((:ts_window_set_icon, _lib), Cvoid, (Csize_t, Cstring), window._native_id, path)
    end

    """
    `set_framerate_limit!(::Integer) -> Nothing`
    """
    function set_framerate_limit!(n_fps::Integer) ::Nothing
        ccall((:ts_set_framerate_limit, _lib), Cvoid, (Csize_t,), convert(Csize_t, n_fps))
    end
    export set_framerate_limit

    """
    `start_frame!(::Window) -> Nothing`
    """
    function start_frame!(window::Window) ::Nothing
        ccall((:ts_start_frame, _lib), Cvoid, (Csize_t,), window._native_id)
    end
    export start_frame

    """
    `end_frame!(::Window) -> Nothing`
    """
    function end_frame!(window::Window) ::Nothing
        ccall((:ts_end_frame, _lib), Cvoid, (Csize_t,), window._native_id)
    end
    export end_frame

    ### CAMERA ################################################################

    """
    Camera

    ## Members
    (no public members)

    ## Constructors
    `Camera(::Window)`
    """
    struct Camera

        _native_window_id::Csize_t

        Camera(window::Window) = new(window._native_id)
    end
    export Camera

    """
    `center_on!(::Camera, ::Vector2f) -> Nothing`
    """
    function center_on!(camera::Camera, point::Vector2f) ::Nothing
        ccall((:ts_window_camera_center_on, _lib), Cvoid,
            (Csize_t, Cfloat, Cfloat),
            camera._native_window_id, point.x, point.y)
    end
    export center_on!

    """
    `move!(::Camera, ::Float32, Float32) -> Nothing`
    """
    function move!(camera::Camera, x_offset::Float32, y_offset::Float32) ::Nothing
        ccall((:ts_window_camera_move, _lib), Cvoid,
            (Csize_t, Cfloat, Cfloat),
            camera._native_window_id, x_offset, y_offset)
    end
    export move!

    """
    `zoom_in!(::Camera, ::Float32) -> Nothing`
    """
    function zoom_in!(camera::Camera, factor::Float32) ::Nothing
        ccall((:ts_window_camera_zoom_in, _lib), Cvoid,
            (Csize_t, Cfloat),
            camera._native_window_id, factor)
    end
    export zoom_in!

    """
    `zoom_out!(::Camera, ::Float32) -> Nothing`
    """
    function zoom_out!(camera::Camera, factor::Float32) ::Nothing
        ccall((:ts_window_camera_zoom_out, _lib), Cvoid,
            (Csize_t, Cfloat),
            camera._native_window_id, factor)
    end
    export zoom_out!

    """
    `set_zoom!(::Camera, ::Float32) -> Nothing`
    """
    function set_zoom!(camera::Camera, factor::Float32) ::Nothing
        ccall((:ts_window_camera_set_zoom, _lib), Cvoid,
            (Csize_t, Cfloat),
            camera._native_window_id, factor)
    end
    export set_zoom!

    """
    `rotate!(::Camera, ::Angle) -> Nothing`
    """
    function rotate!(camera::Camera, angle::Angle) ::Nothing
        ccall((:ts_window_camera_rotate, _lib), Cvoid,
            (Csize_t, Cfloat),
            camera._native_window_id, as_degrees(angle))
    end
    export rotate!

    """
    `set_rotation!(::Camera, ::Angle) -> Nothing`
    """
    function set_rotation!(camera::Camera, angle::Angle) ::Nothing
        ccall((:ts_window_camera_set_rotation, _lib), Cvoid,
            (Csize_t, Cfloat),
            camera._native_window_id, as_degrees(angle))
    end
    export set_rotation!

    """
    `get_transform(::Camera) -> Transform`
    """
    function get_transform(camera::Camera) ::Transform
        return Transform(ccall((:ts_window_camera_get_transform, _lib), Ptr{Cvoid},
            (Csize_t,), camera._native_window_id))
    end
    export get_transform

    """
    `get_center(::Camera) -> Vector2f`
    """
    function get_center(camera::Camera) ::Vector2f

        x = Ref{Cfloat}(-1)
        y = Ref{Cfloat}(-1)
        ccall((:ts_window_camera_get_center, _lib), Cvoid,
            (Csize_t, Ref{Cfloat}, Ref{Cfloat}),
            camera._native_window_id, x, y)

        return Vector2f(x[], y[])
    end
    export get_center

    """
    `get_view_area(::Camera) -> Trapezoid`
    """
    function get_view_area(camera::Camera) ::Trapezoid

        top_left_x = Ref{Cfloat}(-1)
        top_left_y = Ref{Cfloat}(-1)
        top_right_x = Ref{Cfloat}(-1)
        top_right_y = Ref{Cfloat}(-1)
        bottom_left_x = Ref{Cfloat}(-1)
        bottom_left_y  = Ref{Cfloat}(-1)
        bottom_right_x =  Ref{Cfloat}(-1)
        bottom_right_y = Ref{Cfloat}(-1)

        ccall((:ts_window_camera_get_view_area, _lib), Cvoid,
            (Csize_t, Ref{Cfloat}, Ref{Cfloat}, Ref{Cfloat}, Ref{Cfloat}, Ref{Cfloat}, Ref{Cfloat}, Ref{Cfloat}, Ref{Cfloat}),
            camera._native_window_id, top_left_x, top_left_y, top_right_x, top_right_y, bottom_left_x, bottom_left_y, bottom_right_x, bottom_right_y)
    end
    export get_view_area

    ### TODO #############################################################################

    module test

        import Main.ts; using Main.ts
        import Test; using Test

        function run()

            @test ts.initialize()

            @testset "Colors" begin

                rgb_in = RGBA(1, 0.5, 0.5, 1)
                hsv = as_hsv(rgb)
                rgb = as_rgb(hsv)
                @test rgb.r == rgb_in.r
                @test rgb.g == rgb_in.g
                @test rgb.b == rgb_in.b
                @test rgb.a == rgb_in.a
            end

            @testset "Time" begin

                ns = 123456789
                time = Time(ns)

                @test as_minutes(minutes(ns)) == ns
                @test as_seconds(seconds(ns)) == ns
                @test as_milliseconds(milliseconds(ns)) == ns
                @test as_microseconds(microseconds(ns)) == ns
                @test as_nanoseconds(nanoseconds(ns)) == ns

                clock = Clock()
                sleep(0.3)
                @test elapsed(clock).as_seconds > 0.2
                @test restart(clock).as_seconds > 0.2
                @test elapsed(clock).as_seconds < 0.05
            end

            @testset "Angle" begin

                ns = 123
                angle = degrees(ns)
                @test as_degrees(angle) == ns
                @test as_degrees(radians(as_radians(angle))) == ns
            end

            window::Union{Window, Nothing} = nothing

            @testset "Window" begin

                window = ts.Window(0, 0, "TEST IN PROGRESS", DEFAULT)

                @test is_open(window)
                @test has_focus(window) == true
                @test has_mouse_focus(window) == false

                @test get_size(window) == Vector2f(0, 0)
                @test get_position(window).x >= 0
                @test get_position(window).y >= 0

                set_hidden!(window, true)
                @test is_hidden(window) == false
                set_hidden!(window, false)
                @test is_hidden(window) == true

                minimize!(window)
                @test is_minimized(window) == true

                maximize!(window)
                @test is_maximized(window) == true

                clear!(window)
            end

        end
        # no export
    end
end
