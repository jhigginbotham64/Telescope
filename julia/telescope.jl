#
# Copyright 2022 Joshua Higginbotham
# Created on 6/3/22 by clem (mail@clemens-cords.com | https://github.com/Clemapfel)
#

module ts

    ### EXPORTS ###############################################################

    export initialize, set_framerate_limit!, get_framerate_limit, start_frame!, end_frame!

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
    export set_color!, set_blend_mode!, get_blend_mode, set_filtering_mode!,
        get_filtering_mode, get_size

    export Transform
    export apply_to, reset!, combine!, translate!, rotate!, scale!, shear!, reflect!

    export Triangle, Rectangle, Trapezoid, Circle
    export Shape, TriangleShape, RectangleShape, CircleShape, PolygonShape,
        set_centroid!, get_centroid, move!, set_color!, get_color, set_texture!,
        set_texture_rectangle!, get_texture_rectangle, get_bounding_box, get_n_vertices,
        set_vertex_position!, set_vertex_color!, set_vertex_texture_coordinates!,
        get_vertex_position, get_vertex_color, get_vertex_texture_coordinates, get_top_left,
        get_size, set_size!, set_top_left, get_radius, set_radius!

    export Window, WindowOptions
    export close!, is_open, get_size, get_position, set_hidden!, is_hidden, minimize!, is_minimized,
        maximize!, is_maximized, has_focus, has_mouse_focus, clear, render!, flush!

    export Camera
    export center_on!, move!, zoom_in!, zoom_out!, rotate!, set_rotation!, get_transform,
        get_center, get_view_area, reset!, set_transform!

    export PhysicsWorld
    export step!, clear_forces!, get_gravity, set_gravity!, next_event!

    export CollisionShape, CollisionType
    export CollisionTriangle, CollisionRectangle, CollisionCircle, CollisionPolygon,
        CollisionLine, CollisionLineSequence
    export set_density!, get_density, set_restitution!, get_restitution, get_centroid,
        get_bounding_box, get_rotation, set_type!, get_type, get_is_hidden, set_is_hidden,
        get_origin, get_center_of_mass_local, get_center_of_mass_global,
        set_linear_velocity!, get_linear_velocity, set_angular_velocity!, get_angular_velocity,
        apply_force_to!, apply_force_to_center!, apply_torque!, apply_linear_impulse_to!,
        apply_linear_impulse_to_center!, get_mass, get_inertia, set_is_bullet!, get_is_bullet,
        get_is_rotation_fixed, set_is_rotation_fixed!, get_id

    export DistanceInformation, RayCastInformation
    export distance_between, ray_cast, is_point_in_shape

    ### COMMON ################################################################

    const _lib = "../libtelescope.so"
    export _lib

    """
    `initialize() -> Bool`
    """
    function initialize() ::Bool
        return ccall((:ts_initialize, _lib), Bool, ())
    end
    export initialize
    
    """
    `assert_zero_based(id) -> Nothing`
    """
    function assert_zero_base(id) ::Nothing
        if (id == 0)
            @warn "Indices that are 0-based in C++ are instead 1-based in Telescope.jl"
        end
    end
    # no export

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

        Vector2{T}(x, y) where T = new{T}(x, y)
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

        Vector3{T}(x, y, z) where T = new{T}(x, y, z)
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

        function RGBA(r::Number, g::Number, b::Number, a::Number)
            fclamp(x) ::Float32 = if x < 0 return 0 elseif x > 1 return 1 else return x end
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
    `HSVA(h::Float32, s::Float32, v::Float32, a::Float32)`
    """
    struct HSVA

        hue::Float32
        saturation::Float32
        value::Float32
        alpha::Float32

        function HSVA(h::Number, s::Number, v::Number, a::Number)
            fclamp(x) ::Float32 = if x < 0 return 0 elseif x > 1 return 1 else return x end
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

        ccall((:ts_hsv_to_rgb, _lib), Cvoid,
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

        Time(ns) = new(ns)
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
        return ccall((:ts_ns_to_nanoseconds, _lib), Csize_t, (Csize_t,), time._count_ns)
    end
    export as_nanoseconds

    """
    `minutes(::Float64) -> Time`
    """
    function minutes(n) ::Time
        ns::Csize_t = ccall((:ts_minutes_to_ns, _lib), Csize_t, (Cdouble,), n)
        return Time(ns)
    end
    export minutes

    """
    `seconds(::Float64) -> Time`
    """
    function seconds(n) ::Time
        ns::Csize_t = ccall((:ts_seconds_to_ns, _lib), Csize_t, (Cdouble,), n)
        return Time(ns)
    end
    export seconds

    """
    `milliseconds(::Float64) -> Time`
    """
    function milliseconds(n) ::Time
        ns::Csize_t = ccall((:ts_milliseconds_to_ns, _lib), Csize_t, (Cdouble,), n)
        return Time(ns)
    end
    export milliseconds

    """
    `microseconds(::Float64) -> Time`
    """
    function microseconds(n) ::Time
        ns::Csize_t = ccall((:ts_microseconds_to_ns, _lib), Csize_t, (Cdouble,), n)
        return Time(ns)
    end
    export microseconds

    """
    `nanoseconds(::UInt64) -> Time`
    """
    function nanoseconds(n) ::Time
        ns::Csize_t = ccall((:ts_nanoseconds_to_ns, _lib), Csize_t, (Csize_t,), n)
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
        out_ns::Csize_t = ccall((:ts_clock_elapsed, _lib), Csize_t, (Csize_t,), clock._native_id)
        return nanoseconds(out_ns)
    end
    export elapsed

    """
    `restart!(::Clock) -> Time`
    """
    function restart!(clock::Clock) ::Time
        out_ns::Csize_t = ccall((:ts_clock_restart, _lib), Csize_t, (Csize_t,), clock._native_id)
        return nanoseconds(out_ns)
    end
    export restart!; restart = restart!

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
    function degrees(n) ::Angle
        return Angle(n % 360.0)
    end
    export degrees

    """
    `radians(::Float32) -> Angle`
    """
    function radians(n) ::Angle
        return Angle(ccall((:ts_radians_to_degrees, _lib), Cfloat, (Cfloat,), n))
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

        LEFT_BRACKET = get_keyboard_key_enum_value("LEFT_BRACKET")
        RIGHT_BRACKET = get_keyboard_key_enum_value("RIGHT_BRACKET")

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
            return Vector2f(x[], y[])
        end
        export get_cursor_position

        """
        `get_scrollwheel() -> Vector2f`
        """
        function get_scrollwheel() ::Vector2f

            x = Ref{Cfloat}(0)
            y = Ref{Cfloat}(0)
            ccall((:ts_mouse_scrollwheel, _lib), Cvoid, (Ref{Cfloat}, Ref{Cfloat}), x, y)
            return Vector2f(x[], y[])
        end
        export get_scrollwheel
        
        ### CONTROLLER #######################################################

        const ControllerID = Csize_t
        export ControllerID

        """
        `is_down(::ControllerButton, [::ControllerID]) -> Bool`
        """
        function is_down(key::ControllerButton, id::ControllerID = UInt64(1)) ::Bool
            assert_zero_base(id);
            return ccall((:ts_controller_is_down, _lib), Bool, (Int64, Csize_t), key, id - 1)
        end
        export is_down

        """
        `has_state_changed(::ControllerButton, [::ControllerID]) -> Bool`
        """
        function has_state_changed(key::ControllerButton, id::ControllerID = UInt64(1)) ::Bool
            assert_zero_base(id);
            return ccall((:ts_controller_has_state_changed, _lib), Bool, (Int64, Csize_t), key, id - 1)
        end
        export has_state_changed

        """
        `was_pressed(::ControllerButton, [::ControllerID]) -> Bool`
        """
        function was_pressed(key::ControllerButton, id::ControllerID = UInt64(1)) ::Bool
            assert_zero_base(id);
            return ccall((:ts_controller_was_pressed, _lib), Bool, (Int64, Csize_t), key, id - 1)
        end
        export was_pressed
        
        """
        `was_released(::ControllerButton, [::ControllerID]) -> Bool`
        """
        function was_released(key::ControllerButton, id::ControllerID = UInt64(1)) ::Bool
            assert_zero_base(id);
            return ccall((:ts_controller_was_released, _lib), Bool, (Int64, Csize_t), key, id - 1)
        end
        export was_released

        """
        `get_controller_axis_left([::ControllerID]) -> Vector2f`
        """
        function get_controller_axis_left(id::ControllerID = UInt64(1)) ::Vector2f

            x = Ref{Cfloat}(0)
            y = Ref{Cfloat}(0)
            ccall((:ts_controller_axis_left, _lib), Cvoid,
                (Csize_t, Ref{Cfloat}, Ref{Cfloat}),
                id, x, y)
            return Vector2f(x[], y[])

        end
        export get_controller_axis_left

        """
        `get_controller_axis_right([::ControllerID]) -> Vector2f`
        """
        function get_controller_axis_right(id::ControllerID = UInt64(1)) ::Vector2f

            x = Ref{Cfloat}(0)
            y = Ref{Cfloat}(0)
            ccall((:ts_controller_axis_right, _lib), Cvoid,
                (Csize_t, Ref{Cfloat}, Ref{Cfloat}),
                id, x, y)
            return Vector2f(x[], y[])

        end
        export get_controller_axis_right

        """
        `get_controller_trigger_left([::ControllerID]) -> Float32`
        """
        function get_controller_trigger_left(id::ControllerID = UInt64(1)) ::Float32
            assert_zero_base(id);
            return ccall((:ts_controller_trigger_left, _lib), Cfloat, (Csize_t,), id - 1)
        end
        export get_controller_trigger_left

        """
        `get_controller_trigger_right([::ControllerID]) -> Float32`
        """
        function get_controller_trigger_right(id::ControllerID = UInt64(1)) ::Float32
            assert_zero_base(id);
            return ccall((:ts_controller_trigger_right, _lib), Cfloat, (Csize_t,), id - 1)
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

        function Sound(path::String, volume_modifier = 1)

            id = ccall((:ts_sound_load, _lib), Csize_t, (Cstring, Cfloat), path, volume_modifier)
            out = new(id)
            finalizer(out) do x::Sound
                ccall((:ts_sound_unload, _lib), Cvoid, (Csize_t,), x._native_id)
            end
            return out
        end
    end
    export Sound

    const ChannelID = Integer;
    export ChannelID

    """
    SoundHandler: control shorter sound clips by playing them in designated channels
    """
    module SoundHandler

        import Main.ts; using Main.ts
        # SoundHandler is its own module for syntactic consistency with C++
        # singleton of the same name, all names are imported into ts. scope

        """
        maximum channel index
        """
        const n_channels = ccall((:ts_sound_get_max_n_channels, _lib), Csize_t, ())
        export n_channels

        """
        `play!(::ChannelID, ::Sound, [::Integer, ::Time]) -> Nothing`
        """
        function play!(channel::ChannelID, sound::Sound, n_loops::Integer = 1, fade_in_duration::Time = seconds(0.0)) ::Nothing

            assert_zero_base(channel);
            ccall((:ts_sound_play, _lib), Cvoid,
                (Csize_t, Csize_t, Csize_t, Cdouble),
                sound._native_id, channel - 1, n_loops, as_milliseconds(fade_in_duration))
        end
        export play!; play = play!
        
        """
        `stop!(::ChannelID, [::Time]) -> Nothing`
        """
        function stop!(channel::ChannelID, fade_out_duration::Time = seconds(0.0)) ::Nothing
            assert_zero_base(channel);
            ccall((:ts_sound_stop, _lib), Cvoid,
                (Csize_t, Cdouble),
                channel - 1, as_milliseconds(fade_out_duration))
        end
        export stop!; stop = stop!
        
        """
        `pause!(::ChannelID) -> Nothing`
        """
        function pause!(channel::ChannelID) ::Nothing
            assert_zero_base(channel);
            ccall((:ts_sound_pause, _lib), Cvoid, (Csize_t,), channel - 1)
        end
        export pause!; pause = pause!
        
        """
        `unpause!(::ChannelID) -> Nothing`
        """
        function unpause!(channel::ChannelID) ::Nothing
            assert_zero_base(channel);
            ccall((:ts_sound_unpause, _lib), Cvoid, (Csize_t,), channel - 1)
        end
        export unpause!; unpause = unpause!

        """
        `force_stop!(::ChannelID) -> Nothing`
        """
        function force_stop!(channel::ChannelID) ::Nothing
            assert_zero_base(channel);
            ccall((:ts_sound_force_stop, _lib), Cvoid, (Csize_t,), channel - 1)
        end
        export force_stop!; force_stop = force_stop!

        """
        `is_playing(::ChannelID) -> Bool`
        """
        function is_playing(channel::ChannelID) ::Bool
            assert_zero_base(channel);
            return ccall((:ts_sound_is_playing, _lib), Bool, (Csize_t,), channel - 1)
        end
        export is_playing

        """
        `is_paused(::ChannelID) -> Bool`
        """
        function is_paused(channel::ChannelID) ::Bool
            assert_zero_base(channel);
            return ccall((:ts_sound_is_paused, _lib), Bool, (Csize_t,), channel - 1)
        end
        export is_paused

        """
        `is_stopped(::ChannelID) -> Bool`
        """
        function is_stopped(channel::ChannelID) ::Bool
            assert_zero_base(channel);
            return ccall((:ts_sound_is_stopped, _lib), Bool, (Csize_t,), channel - 1)
        end
        export is_stopped

        """
        `set_volume!(::ChannelID, ::Float32) -> Nothing`
        """
        function set_volume!(channel::ChannelID, zero_to_one) ::Nothing
            assert_zero_base(channel);
            ccall((:ts_sound_set_volume, _lib), Cvoid, (Csize_t, Cfloat), channel - 1, zero_to_one)
        end
        export set_volume!; set_volume = set_volume!

        """
        `get_volume(::ChannelID) -> Float32`
        """
        function get_volume(channel::ChannelID) ::Float32
            assert_zero_base(channel);
            return ccall((:ts_sound_get_volume, _lib), Cfloat, (Csize_t,), channel - 1)
        end
        export get_volume

        """
        `set_panning!(::ChannelID, ::Angle) -> Nothing`
        """
        function set_panning!(channel::ChannelID, angle::Angle) ::Nothing
            assert_zero_base(channel);
            return ccall((:ts_sound_set_panning, _lib), Cvoid, (Csize_t, Cfloat), channel - 1, as_degrees(angle))
        end
        export set_panning!; set_panning = set_panning!

        """
        `get_panning(::ChannelID) -> Angle`
        """
        function get_panning(channel::ChannelID) ::Angle
            assert_zero_base(channel);
            return degrees(ccall((:ts_sound_get_panning, _lib), Cfloat, (Csize_t,), channel - 1))
        end
        export get_panning
    end
    export SoundHandler

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
                ccall((:ts_music_unload, _lib), Cvoid, (Csize_t,), x._native_id)
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
        function set_volume!(zero_to_one) ::Nothing
            ccall((:ts_music_set_volume, _lib), Cvoid, (Cfloat,), zero_to_one)
        end
        export set_volume!; set_volume = set_volume!

        """
        `get_volume() -> Float32`
        """
        function get_volume() ::Float32
            ccall((:ts_music_get_volume, _lib), Cfloat, ())
        end
        export get_volume

        """
        `play!(::Music, [::Bool, ::Time]) -> Nothing`
        """
        function play!(music::Music, should_loop::Bool = true, fade_in_duration::Time = seconds(0.0)) ::Nothing
            ccall((:ts_music_play, _lib), Cvoid,
                (Csize_t, Bool, Cdouble),
                music._native_id, should_loop, as_milliseconds(fade_in_duration))
        end
        export play!; play = play!

        """
        `play_next!(::Music, [::Bool, ::Time]) -> Nothing
        """
        function play_next!(music::Music, should_loop::Bool = false, fade_in_duration::Time = seconds(0.0)) ::Nothing
            ccall((:ts_music_play_next, _lib), Cvoid,
                (Csize_t, Bool, Cdouble),
                music._native_id, should_loop, as_milliseconds(fade_in_duration))
        end
        export play_next!; play_next = play_next!

        """
        `stop!(::Time) -> Nothing`
        """
        function stop!(fade_out_duration::Time = seconds(0)) ::Nothing
            ccall((:ts_music_stop, _lib), Cvoid, (Cdouble,), as_milliseconds(fade_out_duration))
        end
        export stop!; stop = stop!

        """
        `next!(::Time) -> Nothing`
        """
        function next!(fade_out_duration::Time = seconds(0)) ::Nothing
            ccall((:ts_music_next, _lib), Cvoid, (Cdouble,), as_milliseconds(fade_out_duration))
        end
        export next!; next = next!

        """
        `clear_next!() -> Nothing`
        """
        function clear_next!() ::Nothing
            ccall((:ts_music_clear_next, _lib), Cvoid, ())
        end
        export clear_next!; clear_next = clear_next!

        """
        `force_stop!() -> Nothing`
        """
        function force_stop!() ::Nothing
            ccall((:ts_music_force_stop, _lib), Cvoid, ())
        end
        export force_stop!; force_stop = force_stop!

        """
        `pause!() -> Nothing`
        """
        function pause!() ::Nothing
            ccall((:ts_music_pause, _lib), Cvoid, ())
        end
        export pause!; pause = pause!

        """
        `unpause!() -> Nothing`
        """
        function unpause!() ::Nothing
            ccall((:ts_music_unpause, _lib), Cvoid, ())
        end
        export unpause!; unpause = unpause!

        """
        `skip_to!(::Time) -> Nothing`
        """
        function skip_to!(timestamp::Time) ::Nothing
            ccall((:ts_music_skip_to, _lib), Cvoid, (Cdouble,), as_milliseconds(timestamp))
        end
        export skip_to!; skip_to = skip_to!

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
    export MusicHandler

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
        function RenderTexture(window::AbstractWindow, width, height)

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
                Ptr{Cvoid}, (Csize_t, Cstring), window._native_id, path)

            out = new(native)
            finalizer(out) do x::StaticTexture

                if x._native != Ptr{Cvoid}()
                    ccall((:ts_texture_destroy_texture, _lib), Cvoid, (Ptr{Cvoid},), x._native)
                end
            end
        end

        # equivalent to ts::StaticTexture::create
        function StaticTexture(window::AbstractWindow, width, height, color::RGBA)

            native = ccall((:ts_texture_create_static_texture, _lib),
                Ptr{Cvoid}, (Csize_t, Csize_t, Csize_t),
                window._native_id, width, height)

            out = new(native)
            finalizer(out) do x::StaticTexture
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
    export set_color!; set_color = set_color!
    
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
            texture._native, mode)
    end
    export set_blend_mode!; set_blend_mode = set_blend_mode!

    """
    `get_blend_mode(::Texture) -> TextureBlendMode`
    """
    function get_blend_mode(texture::Texture) ::TextureBlendMode
        out::Cint = ccall((:ts_texture_get_blend_mode, _lib), Cint, (Ptr{Cvoid},), texture._native)
        return TextureBlendMode(out)
    end
    export get_blend_mode

    """
    `set_filtering_mode!(::Texture, ::TextureFilteringMode) -> Nothing`
    """
    function set_filtering_mode!(texture::Texture, mode::TextureFilteringMode) ::Nothing
        ccall((:ts_texture_set_filtering_mode, _lib), Cvoid,
            (Ptr{Cvoid}, Cint),
            texture._native, mode)
    end
    export set_filtering_mode!; set_filtering_mode = set_filtering_mode!

    """
    `get_filtering_mode(::Texture) -> TextureFilteringMode`
    """
    function get_filtering_mode(texture::Texture) ::TextureFilteringMode
        out::Cint = ccall((:ts_texture_get_filtering_mode, _lib), Cint, (Ptr{Cvoid},), texture._native)
        return TextureFilteringMode(out)
    end
    export get_filtering_mode

    """
    `get_size(::Texture) -> Vector2ui`
    """
    function get_size(texture::Texture) ::Vector2ui

        x = Ref{Csize_t}(0)
        y = Ref{Csize_t}(0)

        ccall((:ts_texture_get_size, _lib), Cvoid,
            (Ptr{Cvoid}, Ref{Csize_t}, Ref{Csize_t}),
            texture._native, x, y)

        return Vector2ui(x[], y[])

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

        Transform(native::Ptr{Cvoid}) = new(native) # sic, don't finalize
    end
    export Transform

    """
    `getindex(::Transform, ::Integer, ::Integer) -> Float32`

    index 1-based
    """
    function Base.getindex(transform::Transform, x::Integer, y::Integer) ::Float32

        x -= 1
        y -= 1
        return ccall((:ts_transform_get, _lib), Cfloat,
            (Ptr{Cvoid}, Csize_t, Csize_t),
            transform._native, x, y)
    end

    """
    `setindex!(::Transform, ::Float32, ::Integer, ::Integer) -> Float32`

    index 1-based
    """
    function Base.setindex!(transform::Transform, value::Number, x::Integer, y::Integer) ::Nothing

        x -= 1
        y -= 1

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

        return Vector2f(x[], y[])
    end
    export apply_to

    """
    `reset!(::Transform) -> Nothing`
    """
    function reset!(transform::Transform) ::Nothing
        ccall((:ts_transform_reset, _lib), Cvoid, (Ptr{Cvoid},), transform._native)
    end
    export reset!; reset = reset!

    """
    `combine!(::Transform, ::Transform) -> Nothing`
    """
    function combine!(left::Transform, right::Transform) ::Nothing
        ccall((:ts_transform_combin, _lib), Cvoid, (Ptr{Cvoid}, Ptr{Cvoid}), left._native, right._native)
    end
    export combine!; combine = combine!

    """
    `translate!(::Transform, ::Float32, ::Float32) -> Nothing`
    """
    function translate!(transform::Transform, x::Number, y::Number) ::Nothing
        ccall((:ts_transform_translate, _lib), Cvoid, (Ptr{Cvoid}, Cfloat, Cfloat), transform._native, x, y)
    end
    export translate!; translate = translate!

    """
    `rotate!(::Transform, ::Angle, [::Vector2f]) -> Nothing`
    """
    function rotate!(transform::Transform, angle::Angle, origin::Vector2f = Vector2f(0, 0)) ::Nothing
        ccall((:ts_transform_rotate, _lib), Cvoid,
            (Ptr{Cvoid}, Cfloat, Cfloat, Cfloat),
            transform._native, as_degrees(angle), origin.x, origin.y)
    end
    export rotate!; rotate = rotate!

    """
    `scale!(::Transform, ::Float32, ::Float32) -> Nothing`
    """
    function scale!(transform::Transform, x_scale::Number, y_scale::Number) ::Nothing
        ccall((:ts_transform_scale, _lib), Cvoid,
            (Ptr{Cvoid}, Cfloat, Cfloat),
            transform._native, x_scale, y_scale)
    end
    export scale!; scale = scale!

    """
    `shear!(::Transform, ::Float32, ::Float32) -> Nothing`
    """
    function shear!(transform::Transform, x_scale::Number, y_scale::Number) ::Nothing
        ccall((:ts_transform_shear, _lib), Cvoid,
            (Ptr{Cvoid}, Cfloat, Cfloat),
            transform._native, x_scale, y_scale)
    end
    export shear!; shear = shear!

    """
    `reflect!(::Transform, ::Bool, ::Bool, [::Vector2f]) -> Nothing`
    """
    function reflect!(transform::Transform, about_x_axis::Bool, about_y_axis::Bool, origin::Vector2f = Vector2f(0, 0)) ::Nothing
        ccall((:ts_transform_reflect, _lib), Cvoid,
            (Ptr{Cvoid}, Bool, Bool, Cfloat, Cfloat),
            transform._native, about_x_axis, about_y_axis, origin.x, origin.y)
    end
    export reflect!; reflect = reflect!

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
    export set_centroid!; set_centroid = set_centroid!

    """
    `get_centroid(::Shape) -> Vector2f`
    """
    function get_centroid(shape::Shape) ::Vector2f

        x = Ref{Cfloat}(0)
        y = Ref{Cfloat}(0)

        ccall((:ts_shape_get_centroid, _lib), Cvoid,
            (Ptr{Cvoid}, Ref{Cfloat}, Ref{Cfloat}),
            shape._native, x, y)

        return Vector2f(x[], y[])
    end
    export get_centroid

    """
    `move!(::Shape, ::Float32, ::Float32) -> Nothing`
    """
    function move!(shape::Shape, x::Number, y::Number) ::Nothing
        ccall((:ts_shape_move, _lib), Cvoid, (Ptr{Cvoid}, Cfloat, Cfloat), shape._native, x, y)
    end
    export move!; move = move!

    """
    `set_color!(::Shape, ::RGBA) -> Nothing`
    """
    function set_color!(shape::Shape, color::RGBA) ::Nothing

        ccall((:ts_shape_set_color, _lib), Cvoid,
            (Ptr{Cvoid}, Cfloat, Cfloat, Cfloat, Cfloat),
            shape._native, color.red, color.green, color.blue, color.alpha)
    end
    export set_color!; set_color = set_color!

    """
    `set_texture!(::Shape, ::Texture) -> Nothing`
    """
    function set_texture!(shape::Shape, texture::Texture) ::Nothing
        ccall((:ts_shape_set_texture, _lib), Cvoid,
            (Ptr{Cvoid}, Ptr{Cvoid}),
            shape._native, texture._native)
    end
    export set_texture!; set_texture = set_texture!

    """
    `set_texture_rectangle!(::Shape, ::Rectangle) -> Nothing`
    """
    function set_texture_rectangle!(shape::Shape, rect::Rectangle) ::Nothing
        ccall((:ts_shape_set_texture_rectangle, _lib), Cvoid,
            (Ptr{Cvoid}, Cfloat, Cfloat, Cfloat, Cfloat),
            shape._native, rect.top_left.x, rect.top_left.y, rect.size.x, rect.size.y)
    end
    export set_texture_rectangle!; set_texture_rectangle = set_texture_rectangle!

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

        index -= 1;

        ccall((:ts_shape_set_vertex_position, _lib), Cvoid,
            (Ptr{Cvoid}, Csize_t, Cfloat, Cfloat),
            shape._native, convert(Csize_t, index), position.x, position.y)
    end
    export set_vertex_position!; set_vertex_position = set_vertex_position!

    """
    `set_vertex_color!(::Shape, ::Integer, ::RGBA) -> Nothing`

    index 1-based
    """
    function set_vertex_color!(shape::Shape, index::Integer, color::RGBA) ::Nothing

        index -= 1;

        ccall((:ts_shape_set_vertex_color, _lib), Cvoid,
            (Ptr{Cvoid}, Csize_t, Cfloat, Cfloat, Cfloat, Cfloat),
            shape._native, convert(Csize_t, index), color.red, color.green, color.blue, color.alpha)
    end
    export set_vertex_color!; set_vertex_color = set_vertex_color!

    """
    `set_vertex_texture_coordinates!(::Shape, ::Integer, ::Vector2f) -> Nothing`

    index 1-based
    """
    function set_vertex_texture_coordinates!(shape::Shape, index::Integer, coordinate::Vector2f) ::Nothing

        index -= 1;

        ccall((:ts_shape_set_vertex_texture_coordinates, _lib), Cvoid,
            (Ptr{Cvoid}, Csize_t, Cfloat, Cfloat),
            shape._native, convert(Csize_t, index), coordinate.x, coordinate.y)
    end
    export set_vertex_texture_coordinates!; set_vertex_texture_coordinates = set_vertex_texture_coordinates!

    """
    `get_vertex_position(shape::Shape, index::Integer) -> Vector2f`

    index 1-based
    """
    function get_vertex_position(shape::Shape, index::Integer) ::Vector2f

        assert_zero_base(channel);
        index -= 1;

        if index > get_n_vertices(shape)
            throw(BoundsError(shape, index))
        end

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

    index 1-based
    """
    function get_vertex_color(shape::Shape, index::Integer) ::RGBA

        assert_zero_base(channel);
        index -= 1;

        if index > get_n_vertices(shape)
            throw(BoundsError(shape, index))
        end

        r = Ref{Cfloat}(-1)
        g = Ref{Cfloat}(-1)
        b = Ref{Cfloat}(-1)
        a = Ref{Cfloat}(-1)

        ccall((:ts_shape_get_vertex_color, _lib), Cvoid,
            (Ptr{Cvoid}, Csize_t, Ref{Cfloat}, Ref{Cfloat}, Ref{Cfloat}, Ref{Cfloat}),
            shape._native, convert(Csize_t, index), r, g, b, a)

        return RGBA(r[], g[], b[], a[])
    end
    export get_vertex_color

    """
    `get_vertex_texture_coordinates(shape::Shape, index::Integer) -> Vector2f`

    index 1-based
    """
    function get_vertex_texture_coordinates(shape::Shape, index::Integer) ::Vector2f

        assert_zero_base(channel);
        index -= 1;

        if index > get_n_vertices(shape)
            throw(BoundsError(shape, index))
        end

        x = Ref{Cfloat}(-1)
        y = Ref{Cfloat}(-1)

        ccall((:ts_shape_get_vertex_texture_coordinates, _lib), Cvoid,
            (Ptr{Cvoid}, Csize_t, Ref{Cfloat}, Ref{Cfloat}),
            shape._native, convert(Csize_t, index), x, y)

        return Vector2f(x[], y[])
    end
    export get_vertex_texture_coordinates

    ### TRIANGLE SHAPE ########################################################

    """
    TriangleShape

    ## Members
    (no public members)

    ## Constructors
    `TriangleShape(::Vector2f, ::Vector2f, ::Vector2f)`
    `TriangleShape(triangle::Triangle)`
    """
    mutable struct TriangleShape <: Shape

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

        function TriangleShape(triangle::Triangle)
            TriangleShape(triangle.a, triangle.b, triangle.c)
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
    `RectangleShape(::Rectangle)`
    """
    mutable struct RectangleShape <: Shape

        _native::Ptr{Cvoid}

        function RectangleShape(top_left::Vector2f, size::Vector2f)

            native = ccall((:ts_shape_create_rectangle, _lib), Ptr{Cvoid},
                (Cfloat, Cfloat, Cfloat, Cfloat),
                top_left.x, top_left.y, size.x, size.y)

            out = new(native)
            finalizer(out) do x::RectangleShape
                ccall((:ts_shape_destroy_rectangle, _lib), Cvoid, (Ptr{Cvoid},), x._native)
            end
        end

        function RectangleShape(rectangle::Rectangle)
            RectangleShape(rectangle.top_left, rectangle.size)
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
    export set_top_left!; set_top_left = set_top_left!

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
    export set_size!; set_size = set_size!

    """
    `get_size(::RectangleShape) -> Vector2f`
    """
    function get_size(rect::RectangleShape) ::Vector2f

        x = Ref{Cfloat}(-1)
        y = Ref{Cfloat}(-1)
        ccall((:ts_shape_rectangle_get_size, _lib), Cvoid,
            (Ptr{Cvoid}, Ref{Cfloat}, Ref{Cfloat}),
            rect._native, x, y)

        return Vector2f(x[], y[])
    end
    export get_size

    ### CIRCLE SHAPE ##########################################################

    """
    CircleShape

    ## Members
    (no public members)

    ## Constructors
    `CircleShape(center::Vector2f, radius::Float32, [n_vertices::UInt64])
    `CircleShape(::Circle, [n_vertices::UInt64])`
    """
    mutable struct CircleShape <: Shape

        _native::Ptr{Cvoid}

        function CircleShape(center::Vector2f, radius::Number, n_vertices::Number = 32)

            native = ccall((:ts_shape_create_circle, _lib), Ptr{Cvoid},
                (Cfloat, Cfloat, Cfloat, Csize_t),
                center.x, center.y, radius, n_vertices)

            out = new(native)
            finalizer(out) do x::CircleShape
                ccall((:ts_shape_destroy_circle, _lib), Cvoid, (Ptr{Cvoid},), x._native)
            end
        end

        function CircleShape(circle::Circle, n_vertices::Number = 32)
            CircleShape(circle.center, circle.radius, n_vertices)
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
    function set_radius!(circle::CircleShape, radius::Number) ::Nothing
       ccall((:ts_shape_circle_set_radius, _lib), Cvoid,
        (Ptr{Cvoid}, Cfloat), circle._native, radius)
    end
    export set_radius!; set_radius = set_radius!

    ### POLYGON SHAPE #########################################################

    """
    PolygonShape

    ## Members
    (no public members)

    ## Constructors
    `PolygonShape(points::Vector2f...)`
    """
    mutable struct PolygonShape <: Shape

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
                (Ptr{Cfloat}, Ptr{Cfloat}, Csize_t), xs, ys, n)

            out = new(native)
            finalizer(out) do x::PolygonShape
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
    `Window(::String, width::Integer, height::Integer, title::String, [options::UInt32])`
    `Window(width::Integer, height::Integer, title::String, [options::UInt32])`
    """
    mutable struct Window <: AbstractWindow

        _native_id::Csize_t

        function Window(title::String, width::Integer, height::Integer, options::UInt32 = UInt32(DEFAULT))
            id = ccall((:ts_window_create, _lib), Csize_t, (Cstring, Csize_t, Csize_t, UInt32), title, width, height, options)
            out = new(id)
            finalizer(out) do x::Window
                ccall((:ts_window_destroy, _lib), Cvoid, (Csize_t,), x._native_id)
            end
            return out
        end

        function Window(title::String, width::Integer, height::Integer, options::WindowOptions = DEFAULT)
            Window(title, width, height, UInt32(options))
        end

        function Window(width::Integer, height::Integer, options::WindowOptions = DEFAULT)
            Window("", width, height, options)
        end
    end
    export Window

    """
    `close!(::Window) -> Nothing`
    """
    function close!(window::Window) ::Nothing
        ccall((:ts_window_close, _lib), Cvoid, (Csize_t,), window._native_id)
    end
    export close!; close = close!

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

        x = Ref{Csize_t}(0)
        y = Ref{Csize_t}(0)
        ccall((:ts_window_get_size, _lib), Cvoid,
            (Csize_t, Ref{Csize_t}, Ref{Csize_t}),
            window._native_id, x, y)

        return Vector2ui(x[], y[])
    end
    export get_size

    """
    `get_position(::Window) -> Vector2ui`
    """
    function get_position(window::Window) ::Vector2ui

        x = Ref{Csize_t}(0)
        y = Ref{Csize_t}(0)
        ccall((:ts_window_get_position, _lib), Cvoid,
            (Csize_t, Ref{Csize_t}, Ref{Csize_t}),
            window._native_id, x, y)

        return Vector2ui(x[], y[])
    end
    export get_position

    """
    `set_hidden!(::Window, ::Bool) -> Nothing
    """
    function set_hidden!(window::Window, hidden::Bool) ::Nothing
        ccall((:ts_window_set_hidden, _lib), Cvoid, (Csize_t, Bool), window._native_id, hidden)
    end
    export set_hidden!; set_hidden = set_hidden!

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
    export minimize!; minimize = minimize!

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
    export maximize!; maximize = maximize!

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
    export clear!; clear = clear!

    """
    `render!(::Window, ::Shape, [::Transform]) -> Nothing`
    """
    function render!(window::Window, shape::Shape, transform::Transform = Transform()) ::Nothing
        ccall((:ts_shape_render_to_window, _lib), Cvoid,
            (Ptr{Cvoid}, Csize_t, Ptr{Cvoid}),
            shape._native, window._native_id, transform._native)
    end
    export render!; render = render!

    """
    `render!(::RenderTexture, ::Shape, [::Transform]) -> Nothing`
    """
    function render!(render_texture::RenderTexture, shape::Shape, transform::Transform = Transform()) ::Nothing
        ccall((:ts_shape_render_to_texture, _lib), Cvoid,
            (Ptr{Cvoid}, Ptr{Cvoid}, Ptr{Cvoid}),
            shape._native, render_texture._native, transform._native)
    end
    export render!; render = render!

    """
    `flush!(::Window) -> Nothing`
    """
    function flush!(window::Window) ::Nothing
        ccall((:ts_window_flush, _lib), Cvoid, (Csize_t,), window._native_id)
    end
    export flush!; flush = flush!

    """
    `set_icon(::Window, ::String) -> Nothing`
    """
    function set_icon!(window::Window, path::String) ::Nothing
        ccall((:ts_window_set_icon, _lib), Cvoid, (Csize_t, Cstring), window._native_id, path)
    end
    export set_icon!; set_icon = set_icon!

    """
    `set_framerate_limit!(::Integer) -> Nothing`
    """
    function set_framerate_limit!(n_fps::Integer) ::Nothing
        ccall((:ts_set_framerate_limit, _lib), Cvoid, (Csize_t,), convert(Csize_t, n_fps))
    end
    export set_framerate_limit!; set_framerate_limit = set_framerate_limit!

    """
    `get_framerate_limit() -> Csize_t`
    """
    function get_framerate_limit() ::Csize_t
        return ccall((:ts_get_framerate_limit, _lib), Csize_t, ())
    end
    export get_framerate_limit

    """
    `start_frame!(::Window) -> Time`
    """
    function start_frame!(window::Window) ::Time

        ms = ccall((:ts_start_frame, _lib), Cdouble, (Csize_t,), window._native_id)
        return milliseconds(ms)
    end
    export start_frame!; start_frame = start_frame!

    """
    `end_frame!(::Window) -> Nothing`
    """
    function end_frame!(window::Window) ::Nothing
        ccall((:ts_end_frame, _lib), Cvoid, (Csize_t,), window._native_id)
    end
    export end_frame!; end_frame = end_frame!

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
    export center_on!; center_on = center_on!

    """
    `move!(::Camera, ::Float32, Float32) -> Nothing`
    """
    function move!(camera::Camera, x_offset::Number, y_offset::Number) ::Nothing
        ccall((:ts_window_camera_move, _lib), Cvoid,
            (Csize_t, Cfloat, Cfloat),
            camera._native_window_id, x_offset, y_offset)
    end
    export move!; move = move!

    """
    `zoom_in!(::Camera, ::Float32) -> Nothing`
    """
    function zoom_in!(camera::Camera, factor::Number) ::Nothing
        ccall((:ts_window_camera_zoom_in, _lib), Cvoid,
            (Csize_t, Cfloat),
            camera._native_window_id, factor)
    end
    export zoom_in!; zoom_in = zoom_in!

    """
    `zoom_out!(::Camera, ::Float32) -> Nothing`
    """
    function zoom_out!(camera::Camera, factor::Number) ::Nothing
        ccall((:ts_window_camera_zoom_out, _lib), Cvoid,
            (Csize_t, Cfloat),
            camera._native_window_id, factor)
    end
    export zoom_out!; zoom_out = zoom_out!

    """
    `set_zoom!(::Camera, ::Float32) -> Nothing`
    """
    function set_zoom!(camera::Camera, factor::Number) ::Nothing
        ccall((:ts_window_camera_set_zoom, _lib), Cvoid,
            (Csize_t, Cfloat),
            camera._native_window_id, factor)
    end
    export set_zoom!; set_zoom = set_zoom!

    """
    `rotate!(::Camera, ::Angle) -> Nothing`
    """
    function rotate!(camera::Camera, angle::Angle) ::Nothing
        ccall((:ts_window_camera_rotate, _lib), Cvoid,
            (Csize_t, Cfloat),
            camera._native_window_id, as_degrees(angle))
    end
    export rotate!; rotate = rotate!

    """
    `set_rotation!(::Camera, ::Angle) -> Nothing`
    """
    function set_rotation!(camera::Camera, angle::Angle) ::Nothing
        ccall((:ts_window_camera_set_rotation, _lib), Cvoid,
            (Csize_t, Cfloat),
            camera._native_window_id, as_degrees(angle))
    end
    export set_rotation!; set_rotation = set_rotation!

    """
    `get_transform(::Camera) -> Transform`
    """
    function get_transform(camera::Camera) ::Transform
        return Transform(ccall((:ts_window_camera_get_transform, _lib), Ptr{Cvoid},
            (Csize_t,), camera._native_window_id))
    end
    export get_transform;

    """
    `set_transform!(::Camera, ::Transform) -> Nothing`
    """
    function set_transform!(camera::Camera, transform::Transform) ::Nothing
        ccall((:ts_window_camera_set_transform, _lib), Cvoid,
            (Csize_t, Ptr{Cvoid}),
            camera._native_window_id, transform._native)
    end
    export set_transform!; set_transform = set_transform!

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
    `reset!(::Camera) -> Nothing`
    """
    function reset!(camera::Camera) ::Nothing
        ccall((:ts_window_camera_reset, _lib), Cvoid, (Csize_t,), camera._native_window_id)
    end
    export reset!; reset = reset!

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

        return Trapezoid(
            Vector2f(top_left_x[], top_left_y[]),
            Vector2f(top_right_x[], top_right_y[]),
            Vector2f(bottom_left_x[], bottom_left_y[]),
            Vector2f(bottom_right_x[], bottom_right_y[]))
    end
    export get_view_area
    
    ### PHYSICS ##########################################################################
    
    """
    PhysicsWorld
    
    ### Members
    (no public members)

    ### Constructors
    `PhysicsWorld()`
    """
    mutable struct PhysicsWorld

        _native_id::Csize_t

        function PhysicsWorld()

            id = ccall((:ts_physics_world_create, _lib), Csize_t, ())
            out = new(id)
            finalizer(out) do x::PhysicsWorld
                ccall((:ts_physics_world_destroy, _lib), Cvoid, (Csize_t,), x._native_id)
            end
            return out
        end
    end
    
    """
    `step!(::PhysicsWorld, ::Time, [::Integer, ::Integer]) -> Nothing`
    """
    function step!(world::PhysicsWorld, time::Time, velocity_iterations::Integer = 8, position_iterations::Integer = 3) ::Nothing
        ccall((:ts_physics_world_step, _lib), Cvoid,
            (Csize_t, Cfloat, Cint, Cint),
            world._native_id, as_milliseconds(time), velocity_iterations, position_iterations)
    end
    export step!; step = step!
    
    """
    `clear_forces!(::PhysicsWorld) -> Nothing
    """
    function clear_forces!(world::PhysicsWorld) ::Nothing
        ccall((:ts_physics_world_clear_forces, _lib), Cvoid, (Csize_t,), world._native_id)
    end
    export clear_forces!; clear_forces = clear_forces!
    
    """
    `get_gravity(::PhysicsWorld) -> Vector2f`
    """
    function get_gravity(world::PhysicsWorld) ::Vector2f

        x = Ref{Cfloat}(0)
        y = Ref{Cfloat}(0)

        ccall((:ts_physics_world_get_gravity, _lib), Cvoid,
            (Csize_t, Ref{Cfloat}, Ref{Cfloat}),
            world._native_id, x, y)

        return Vector2f(x[], y[])
    end
    export get_gravity
    
    """
    `set_gravity!(::PhysicsWorld, ::Vector2f) -> Nothing`
    """
    function set_gravity!(world::PhysicsWorld, gravity::Vector2f) ::Nothing
        ccall((:ts_physics_world_set_gravity, _lib), Cvoid,
            (Csize_t, Cfloat, Cfloat),
            world._native_id, gravity.x, gravity.y)
    end
    export set_gravity!; set_gravity = set_gravity!

    """
    CollisionShape (Abstract Interface)

    ### Expected Members
    _native::Ptr{Cvoid}
    """
    abstract type CollisionShape end
    export CollisionShape

    """
    enum CollisionType <: UInt64
    """
    @enum CollisionType begin

        STATIC = ccall((:ts_collision_type_static, _lib), Csize_t, ())
        KINEMATIC = ccall((:ts_collision_type_kinematic, _lib), Csize_t, ())
        DYNAMIC = ccall((:ts_collision_type_dynamic, _lib), Csize_t, ())
    end
    @export_enum(CollisionType)

    """
    CollisionPolygon

    ### Constructors

    CollisionTriangle(::PhysicsWorld, ::CollisionType, a::Vector2f, b::Vector2f, c::Vector2f)
    CollisionTriangle(::PhysicsWorld, ::CollisionType, triangle::Triangle)
    CollisionTriangle(::PhysicsWorld, ::CollisionType, top_left::Vector2f, size::Vector2f)

    CollisionRectangle(::PhysicsWorld, ::CollisionType, top_left::Vector2f, size::Vector2f)
    CollisionRectangle(::PhysicsWorld, ::CollisionType, rectangle::Rectangle)
    CollisionRectangle(::PhysicsWorld, ::CollisionType, rectangle::RectangleShape)

    CollisionPolygon(::PhysicsWorld, ::CollisionType, vertices::Vector{Vector2f})
    """
    mutable struct CollisionPolygon <: CollisionShape

        _native::Ptr{Cvoid}

        # as triangle
        function CollisionPolygon(world::PhysicsWorld, type::CollisionType, a::Vector2f, b::Vector2f, c::Vector2f)

            native = ccall((:ts_collision_triangle_create, _lib), Ptr{Cvoid},
                (Csize_t, Csize_t, Cfloat, Cfloat, Cfloat, Cfloat, Cfloat, Cfloat),
                world._native_id, Csize_t(type), a.x, a.y, b.x, b.y, c.x, c.y)

            out = new(native)
            finalizer(out) do x::CollisionPolygon
                ccall((:ts_collision_shape_destroy, _lib), Cvoid, (Ptr{Cvoid},), x._native)
            end
            return out;
        end

        # as triangle
        function CollisionPolygon(world::PhysicsWorld, type::CollisionType, triangle::Triangle)
            return CollisionPolygon(world, type, triangle.a, triangle.b, triangle.c)
        end

        # as triangle
        function CollisionPolygon(world::PhysicsWorld, type::CollisionType, triangle::TriangleShape)

            a = get_vertex_position(triangle, 1)
            b = get_vertex_position(triangle, 2)
            c = get_vertex_position(triangle, 3)

            return CollisionPolygon(world, type, a, b, c)
        end

        # as rectangle
        function CollisionPolygon(world::PhysicsWorld, type::CollisionType, top_left::Vector2f, size::Vector2f)

            native = ccall((:ts_collision_rectangle_create, _lib), Ptr{Cvoid},
                (Csize_t, Csize_t, Cfloat, Cfloat, Cfloat, Cfloat),
                world._native_id, Csize_t(type), top_left.x, top_left.y, size.x, size.y)

            out = new(native)
            finalizer(out) do x::CollisionPolygon
                ccall((:ts_collision_shape_destroy, _lib), Cvoid, (Ptr{Cvoid},), out._native)
            end
        end

        # as rectangle
        function CollisionPolygon(world::PhysicsWorld, type::CollisionType, rectangle::Rectangle)
            return CollisionPolygon(world, type, rectangle.top_left, rectangle.size)
        end

        # as rectangle
        function CollisionPolygon(world::PhysicsWorld, type::CollisionType, rectangle::RectangleShape)
            return CollisionPolygon(world, type, get_top_left(rectangle), get_size(rectangle))
        end

        # as polygon
        function CollisionPolygon(world::PhysicsWorld, type::CollisionType, vertices::Vector{Vector2f})

            xs = Float32[]
            ys = Float32[]

            for v in vertices
                push!(xs, v.x)
                push!(ys, v.y)
            end

            native = ccall((:ts_collision_polygon_create, _lib), Ptr{Cvoid},
                (Csize_t, Csize_t, Ptr{Cfloat}, Ptr{Cfloat}, Csize_t),
                world._native_id, Csize_t(type), xs, ys, length(vertices))

            out = new(native)
            finalizer(out) do x::CollisionPolygon
                ccall((:ts_collision_shape_destroy, _lib), Cvoid, (Ptr{Cvoid},), out._native)
            end
            return out
        end
    end
    export CollisionPolygon

    function CollisionTriangle(world::PhysicsWorld, type::CollisionType, a::Vector2f, b::Vector2f, c::Vector2f) ::CollisionPolygon
        return CollisionPolygon(world, type, a, b, c)
    end
    export CollisionTriangle

    function CollisionTriangle(world::PhysicsWorld, type::CollisionType, triangle::Triangle) ::CollisionPolygon
        return CollisionPolygon(world, type, triangle)
    end
    export CollisionTriangle

    function CollisionTriangle(world::PhysicsWorld, type::CollisionType, triangle::TriangleShape) ::CollisionPolygon
        return CollisionPolygon(world, type, triangle)
    end
    export CollisionTriangle

    function CollisionRectangle(world::PhysicsWorld, type::CollisionType, top_left::Vector2f, size::Vector2f) ::CollisionPolygon
        return CollisionPolygon(world, type, top_left, size)
    end
    export CollisionRectangle

    function CollisionRectangle(world::PhysicsWorld, type::CollisionType, rectangle::Rectangle) ::CollisionPolygon
        return CollisionPolygon(world, type, rectangle)
    end
    export CollisionRectangle

    function CollisionRectangle(world::PhysicsWorld, type::CollisionType, rectangle::RectangleShape) ::CollisionPolygon
        return CollisionPolygon(world, type, rectangle)
    end
    export CollisionRectangle

    """
    CollisionCircle

    ### Members
    (no public members)

    ### Constructors
    `CollisionCircle(::PhysicsWorld, ::CollisionType, ::Vector2f, ::Float32)`
    `CollisionCircle(::PhysicsWorld, ::CollisionType, ::Circle)`
    `CollisionCircle(::PhysicsWorld, ::CollisionType, ::CircleShape)`
    """
    mutable struct CollisionCircle <: CollisionShape

        _native::Ptr{Cvoid}

        function CollisionCircle(world::PhysicsWorld, type::CollisionType, center::Vector2f, radius::Float32)

            native = ccall((:ts_collision_circle_create, _lib), Ptr{Cvoid},
                (Csize_t, Csize_t, Cfloat, Cfloat, Cfloat),
                world._native_id, Csize_t(type), center.x, center.y, radius)

            out = new(native)
            finalizer(out) do x::CollisionCircle
                ccall((:ts_collision_shape_destroy, _lib), Cvoid, (Ptr{Cvoid},), out._native)
            end
            return out
        end

        function CollisionCircle(world::PhysicsWorld, type::CollisionType, circle::Circle)
            CollisionCircle(world, type, circle.center, circle.radius)
        end

        function CollisionCircle(world::PhysicsWorld, type::CollisionType, circle::CircleShape)
            CollisionCircle(world, type, get_centroid(circle), get_radius(circle))
        end
    end
    export CollisionCircle

    """
    CollisionLine

    ### Members
    (no public members)

    ### Constructors
    `CollisionLine(::PhysicsWorld, ::CollisionType, ::Vector2f, ::Vector2f, [is_two_sided::Bool])`
    """
    mutable struct CollisionLine <: CollisionShape

        _native::Ptr{Cvoid}
        is_two_sided::Bool

        function CollisionLine(world::PhysicsWorld, type::CollisionType, a::Vector2f, b::Vector2f, is_two_sided::Bool = true)

            native = ccall((:ts_collision_line_create, _lib), Ptr{Cvoid},
                (Csize_t, Csize_t, Cfloat, Cfloat, Cfloat, Cfloat, Bool),
                world._native_id, Csize_t(type), a.x, a.y, b.x, b.y, is_two_sided)
            out = new(native, is_two_sided)
            finalizer(out) do x::CollisionLine
                ccall((:ts_collision_shape_destroy, _lib), Cvoid, (Ptr{Cvoid},), out._native)
            end
            return out
        end
    end
    export CollisionLine

    """
    CollisionLineSequence

    ### Members
    (no public members)

    ### Constructors
    `CollisionLineSequence(::PhysicsWorld, ::CollisionType, ::Vector{Vector2f})`
    """
    mutable struct CollisionLineSequence <: CollisionShape

        _native::Ptr{Cvoid}

        function CollisionLineSequence(world::PhysicsWorld, type::CollisionType, vertices::Vector{Vector2f})

            xs = Float32[]
            ys = Float32[]

            for v in vertices
                push!(xs, v.x)
                push!(ys, v.y)
            end

            native = ccall((:ts_collision_line_sequence_create, _lib), Ptr{Cvoid},
                (Csize_t, Csize_t, Ptr{Cfloat}, Ptr{Cfloat}, Csize_t),
                world._native_id, Csize_t(type), xs, ys, length(vertices))

            out = new(native)
            finalizer(out) do x::CollisionLineSequence
                ccall((:ts_collision_shape_destroy, _lib), Cvoid, (Ptr{Cvoid},), out._native)
            end
            return out
        end
    end
    export CollisionLineSequence

    """
    `set_density!(::CollisionShape, ::Float32) -> Nothing`
    """
    function set_density!(shape::CollisionShape, value::Number) ::Nothing
        ccall((:ts_collision_shape_set_density, _lib), Cvoid, (Ptr{Cvoid}, Cfloat), shape._native, value)
    end
    export set_density!; set_density = set_density!

    """
    `get_density(::CollisionShape) -> Float32`
    """
    function get_density(shape::CollisionShape) ::Float32
        return ccall((:ts_collision_shape_get_density, _lib), Cfloat, (Ptr{Cvoid},), shape._native)
    end
    export get_density

    """
    `set_friction!(::CollisionShape, ::Float32) -> Nothing`
    """
    function set_friction!(shape::CollisionShape, value::Number) ::Nothing
        ccall((:ts_collision_shape_set_friction, _lib), Cvoid, (Ptr{Cvoid}, Cfloat), shape._native, value)
    end
    export set_friction!; set_friction = set_friction!

    """
    `get_friction(::CollisionShape) -> Float32`
    """
    function get_friction(shape::CollisionShape) ::Float32
        ccall((:ts_collision_shape_get_friction, _lib), Cfloat, (Ptr{Cvoid},), shape._native)
    end
    export get_friction

    """
    `set_restitution!(::CollisionShape, ::Float32) -> Nothing`
    """
    function set_restitution!(shape::CollisionShape, value::Number) ::Nothing
        ccall((:ts_collision_shape_set_restitution, _lib), Cvoid,
            (Ptr{Cvoid}, Cfloat),
            shape._native, value)
    end
    export set_restitution!; set_restitution = set_restitution!

    """
    `get_restitution(::CollisionShape) -> Float32`
    """
    function get_restitution(shape::CollisionShape) ::Float32
        return ccall((:ts_collision_shape_get_restitution, _lib), Cfloat, (Ptr{Cvoid},), shape._native)
    end
    export get_restitution

    """
    `get_centroid(::CollisionShape) -> Vector2f`
    """
    function get_centroid(shape::CollisionShape) ::Vector2f

        x = Ref{Cfloat}(0)
        y = Ref{Cfloat}(0)

        ccall((:ts_collision_shape_get_centroid, _lib), Cvoid,
            (Ptr{Cvoid}, Ref{Cfloat}, Ref{Cfloat}),
            shape._native, x, y)

        return Vector2f(x[], y[])
    end
    export get_centroid

    """
    `get_bounding_box(::CollisionShape) -> Rectangle`
    """
    function get_bounding_box(shape::CollisionShape) ::Rectangle

        x = Ref{Cfloat}(0)
        y = Ref{Cfloat}(0)
        width = Ref{Cfloat}(0)
        height = Ref{Cfloat}(0)

        ccall((:ts_collision_shape_get_bounding_box, _lib), Cvoid,
            (Ptr{Cvoid}, Ref{Cfloat}, Ref{Cfloat}, Ref{Cfloat}, Ref{Cfloat}),
            shape._native, x, y, width, height)

        return Rectangle(Vector2f(x[], y[]), Vector2f(width[], height[]))
    end
    export get_bounding_box

    """
    `get_rotation(::CollisionShape) -> Angle`
    """
    function get_rotation(shape::CollisionShape) ::Angle
        return degrees(ccall((:ts_collision_shape_get_rotation, _lib), Cfloat,
            (Ptr{Cvoid},), shape._native))
    end
    export get_rotation

    """
    `set_type!(::CollisionShape, ::CollisionType) -> Nothing`
    """
    function set_type!(shape::CollisionShape, type::CollisionType) ::Nothing
        ccall((:ts_collision_shape_set_type, _lib), Cvoid,
            (Ptr{Cvoid}, Csize_t), shape._native, Csize_t(type))
    end
    export set_type!; set_type = set_type!

    """
    `get_type(::CollisionShape) -> CollisionType`
    """
    function get_type(shape::CollisionShape) ::CollisionType
        return CollisionType(ccall((:ts_collision_shape_get_type, _lib), Csize_t,
            (Ptr{Cvoid},), shape._native))
    end
    export get_type

    """
    `set_is_hidden(::CollisionShape, ::Bool) -> Nothing`
    """
    function set_is_hidden(shape::CollisionShape, value::Bool) ::Nothing
        ccall((:ts_collision_shape_set_is_hidden, _lib), Cvoid, (Ptr{Cvoid}, Bool), shape._native, value)
    end

    """
    `get_is_hidden(::CollisionShape) -> Bool`
    """
    function get_is_hidden(shape::CollisionShape) ::Bool
        return ccall((:ts_collision_shape_get_is_hidden, _lib), Bool, (Ptr{Cvoid},), shape._native)
    end

    """
    `get_origin(::CollisionShape) -> Vector2f`
    """
    function get_origin(shape::CollisionShape) ::Vector2f

        x = Ref{Cfloat}(0)
        y = Ref{Cfloat}(0)

        ccall((:ts_collision_shape_get_origin, _lib), Cvoid,
            (Ptr{Cvoid}, Ref{Cfloat}, Ref{Cfloat}), shape._native, x, y)

        return Vector2f(x[], y[])
    end
    export get_origin

    """
    `get_center_of_mass_local(::CollisionShape) -> Vector2f`
    """
    function get_center_of_mass_local(shape::CollisionShape) ::Vector2f

        x = Ref{Cfloat}(0)
        y = Ref{Cfloat}(0)

        ccall((:ts_collision_shape_get_center_of_mass_local, _lib), Cvoid,
            (Ptr{Cvoid}, Ref{Cfloat}, Ref{Cfloat}), shape._native, x, y)

        return Vector2f(x[], y[])
    end
    export get_center_of_mass_local

    """
    `get_center_of_mass_global(::CollisionShape) -> Vector2f`
    """
    function get_center_of_mass_global(shape::CollisionShape) ::Vector2f

        x = Ref{Cfloat}(0)
        y = Ref{Cfloat}(0)

        ccall((:ts_collision_shape_get_center_of_mass_global, _lib), Cvoid,
            (Ptr{Cvoid}, Ref{Cfloat}, Ref{Cfloat}), shape._native, x, y)

        return Vector2f(x[], y[])
    end
    export get_center_of_mass_global

    """
    `set_linear_velocity!(::CollisionShape, ::Vector2f) -> Nothing`
    """
    function set_linear_velocity!(shape::CollisionShape, velocity::Vector2f) ::Nothing
        ccall((:ts_collision_shape_set_linear_velocity, _lib), Cvoid,
            (Ptr{Cvoid}, Cfloat, Cfloat), shape._native, velocity.x, velocity.y)
    end
    export set_linear_velocity!; set_linear_velocity = set_linear_velocity!

    """
    `get_linear_velocity(::CollisionShape) -> Vector2f`
    """
    function get_linear_velocity(shape::CollisionShape) ::Vector2f
        x = Ref{Cfloat}(0)
        y = Ref{Cfloat}(0)

        ccall((:ts_collision_shape_get_linear_velocity, _lib), Cvoid,
            (Ptr{Cvoid}, Ref{Cfloat}, Ref{Cfloat}), shape._native, x, y)

        return Vector2f(x[], y[])
    end
    export get_linear_velocity

    """
    `set_angular_velocity!(::CollisionShape, ::Float32) -> Nothing`
    """
    function set_angular_velocity!(shape::CollisionShape, value::Number) ::Nothing
        ccall((:ts_collision_shape_set_angular_velocity, _lib), Cvoid,
            (Ptr{Cvoid}, Cfloat), shape._native, value)
    end
    export set_angular_velocity!; set_angular_velocity = set_angular_velocity!

    """
    `get_angular_velocity(::CollisionShape) -> Float32`
    """
    function get_angular_velocity(shape::CollisionShape) ::Float32
        return ccall((:ts_collision_shape_get_angular_velocity, _lib), Cfloat,
            (Ptr{Cvoid},), shape._native)
    end
    export get_angular_velocity

    """
    `apply_force_to(::CollisionShape, force::Float32, point::Vector2f) -> Nothing`
    """
    function apply_force_to!(shape::CollisionShape, force::Vector2f, point::Vector2f) ::Nothing
        ccall((:ts_collision_shape_apply_force_to, _lib), Cvoid,
            (Ptr{Cvoid}, Cfloat, Cfloat, Cfloat, Cfloat),
            shape._native, force.x, force.y, point.x, point.y)
    end
    export apply_force_to!; apply_force_to = apply_force_to!

    """
    `apply_force_to_center(::CollisionShape, force::Vector2f) -> Nothing`
    """
    function apply_force_to_center!(shape::CollisionShape, force::Vector2f) ::Nothing
        ccall((:ts_collision_shape_apply_force_to_center, _lib), Cvoid,
            (Ptr{Cvoid}, Cfloat, Cfloat), shape._native, force.x, force.y)
    end
    export apply_force_to_center!; apply_force_to_center = apply_force_to_center!

    """
    apply_torque!(::CollisionShape, ::Float32) -> Nothing
    """
    function apply_torque!(shape::CollisionShape, value::Number) ::Nothing
        ccall((:ts_collision_shape_apply_torque, _lib), Cvoid, (Ptr{Cvoid}, Cfloat), shape._native, value)
    end
    export apply_torque!; apply_torque = apply_torque!

    """
    `apply_linear_impulse_to!(::CollisionShape, force::Vector2f, point::Vector2f) -> Nothing`
    """
    function apply_linear_impulse_to!(shape::CollisionShape, impulse::Vector2f, point::Vector2f) ::Nothing
        ccall((:ts_collision_shape_apply_linear_impulse_to, _lib), Cvoid,
            (Ptr{Cvoid}, Cfloat, Cfloat, Cfloat, Cfloat),
            shape._native, impulse.x, impulse.y, point.x, point.y)
    end
    export apply_linear_impulse_to!; apply_linear_impulse_to = apply_linear_impulse_to!

    """
    `apply_linear_impulse_to_center!(::CollisionShape, force::Vector2f) -> Nothing`
    """
    function apply_linear_impulse_to_center!(shape::CollisionShape, impulse::Vector2f) ::Nothing
        ccall((:ts_collision_shape_apply_linear_impulse_to_center, _lib), Cvoid,
            (Ptr{Cvoid}, Cfloat, Cfloat), shape._native, impulse.x, impulse.y)
    end
    export apply_linear_impulse_to_center!; apply_linear_impulse_to_center = apply_linear_impulse_to_center!

    """
    `get_mass(::CollisionShape) -> Float32`
    """
    function get_mass(shape::CollisionShape) ::Float32
        return ccall((:ts_collision_shape_get_mass, _lib), Cfloat, (Ptr{Cvoid},), shape._native)
    end
    export get_mass

    """
    `get_inertia(::CollisionShape) -> Float32`
    """
    function get_inertia(shape::CollisionShape) ::Float32
        return ccall((:ts_collision_shape_get_inertia, _lib), Cfloat, (Ptr{Cvoid},), shape._native)
    end
    export get_inertia

    """
    `set_is_bullet!(::CollisionShape, ::Bool) -> Nothing`
    """
    function set_is_bullet!(shape::CollisionShape, value::Bool) ::Nothing
        ccall((:ts_collision_shape_set_is_bullet, _lib), Cvoid, (Ptr{Cvoid}, Bool), shape._native, value)
    end
    export set_is_bullet!; set_is_bullet = set_is_bullet!

    """
    `is_bullet(::CollisionShape) -> Bool`
    """
    function get_is_bullet(shape::CollisionShape) ::Bool
        return ccall((:ts_collision_shape_get_is_bullet, _lib), Bool, (Ptr{Cvoid},), shape._native)
    end
    export is_bullet

    """
    `is_rotation_fixed(::CollisionShape) -> Bool`
    """
    function get_is_rotation_fixed(shape::CollisionShape) ::Bool
        return ccall((:ts_collision_shape_get_is_rotation_fixed, _lib), Bool, (Ptr{Cvoid},), shape._native)
    end
    export get_is_rotation_fixed

    """
    `set_rotation_fixed!(::CollisionShape, ::Bool) -> Nothing`
    """
    function set_is_rotation_fixed!(shape::CollisionShape, value::Bool) ::Nothing
        ccall((:ts_collision_shape_set_is_rotation_fixed, _lib), Cvoid, (Ptr{Cvoid}, Bool), shape._native, value)
    end
    export set_is_rotation_fixed!; set_is_rotation_fixed = set_is_rotation_fixed!

    """
    `get_id(::CollisionShape) -> UInt64`
    """
    function get_id(shape::CollisionShape) ::UInt64
        return ccall((:ts_collision_shape_get_id, _lib), Csize_t, (Ptr{Cvoid},), shape._native)
    end
    export get_id

    """
    DistanceInformation

    ### Members
    distance::Float32
    closest_points::Pair{Vector2f, Vector2f}

    ### Constructors
    (no public constructors)
    """
    struct DistanceInformation

        distance::Float32
        closest_points::Pair{Vector2f, Vector2f}

        function DistanceInformation(distance::Float32, point_a::Vector2f, point_b::Vector2f)
            return new(distance, Pair{Vector2f, Vector2f}(point_a, point_b))
        end
    end
    export DistanceInformation

    """
    `distance_between(::PhysicsWorld, ::Shape, ::Shape) -> DistanceInformation`
    """
    function distance_between(world::PhysicsWorld, shape_a::CollisionShape, shape_b::CollisionShape) ::DistanceInformation

        distance = Ref{Cfloat}(-1)
        a_x = Ref{Cfloat}(-1)
        a_y = Ref{Cfloat}(-1)
        b_x = Ref{Cfloat}(-1)
        b_y = Ref{Cfloat}(-1)

        ccall((:ts_physics_world_distance_between, _lib), Cvoid,
            (Csize_t, Ptr{Cvoid}, Ptr{Cvoid}, Ref{Cfloat}, Ref{Cfloat}, Ref{Cfloat}, Ref{Cfloat}, Ref{Cfloat}),
            world._native_id, shape_a._native, shape_b._native, distance, a_x, a_y, b_x, b_y)

        return DistanceInformation(distance[], Vector2f(a_x[], a_y[]), Vector2f(b_x[], b_y[]))
    end
    export distance_between

    """
    RayCastInformation

    ### Members
    are_colliding::Bool
    normal_vector::Vector2f
    contact_point::Vector2f

    ### Constructors
    (no public constructors)
    """
    struct RayCastInformation

        are_colliding::Bool
        normal_vector::Vector2f
        contact_point::Vector2f
    end
    export RayCastInformation

    """
    `ray_cast(::PhysicsWorld, ::CollisionShape, ray_start::Vector2f, ray_end::Vector2f) -> RayCastInformation`
    """
    function ray_cast(world::PhysicsWorld, shape::CollisionShape, ray_start::Vector2f, ray_end::Vector2f, multiplier::Number = 1) ::RayCastInformation

        normal_x = Ref{Cfloat}(-1)
        normal_y = Ref{Cfloat}(-1)
        hit_x = Ref{Cfloat}(-1)
        hit_y = Ref{Cfloat}(-1)

        colliding = ccall((:ts_physics_world_ray_cast, _lib), Bool,
            (Csize_t, Ptr{Cvoid}, Cfloat, Cfloat, Cfloat, Cfloat, Cfloat, Ref{Cfloat}, Ref{Cfloat}, Ref{Cfloat}, Ref{Cfloat}),
            world._native_id, shape._native, ray_start.x, ray_start.y, ray_end.x, ray_end.y, multiplier, normal_x, normal_y, hit_x, hit_y)

        return RayCastInformation(colliding, Vector2f(normal_x[], normal_y[]), Vector2f(hit_x[], hit_y[]))
    end
    export ray_cast

    """
    `is_point_in_shape(::CollisionShape, ::Vector2f) -> Bool`
    """
    function is_point_in_shape(world::PhysicsWorld, shape::CollisionShape, point::Vector2f) ::Bool
        return ccall((:ts_physics_world_is_point_in_shape, _lib), Bool,
            (Csize_t, Ptr{Cvoid}, Cfloat, Cfloat),
            world._native_id, shape._native, point.x, point.y)
    end
    export is_point_in_shape

    """
    enum CollisionEventType <: Bool
    """
    @enum CollisionEventType begin

        CONTACT_START = true
        CONTACT_END = false
    end
    @export_enum CollisionEventType

    """
    CollisionEvent

    ### Members
    type::CollisionEventType
    shape_a_id::Csize_t
    shape_b_id::Csize_t

    ### Constructors
    CollisionEvent()
    """
    mutable struct CollisionEvent

        type::CollisionEventType
        shape_a_id::Union{Csize_t, Nothing}
        shape_b_id::Union{Csize_t, Nothing}

        CollisionEvent() = new(CollisionEventType(false), nothing, nothing)
    end
    export CollisionEvent

    """
    `next_event!(::PhysicsWorld, ::Ref{CollisionEvent}) -> Bool`
    """
    function next_event!(world::PhysicsWorld, event::CollisionEvent) ::Bool

        a_id = Ref{Csize_t}()
        b_id = Ref{Csize_t}()
        type = Ref{Bool}()

        new_event = ccall((:ts_physics_world_next_event, _lib), Bool,
            (Csize_t, Ref{Bool}, Ref{Csize_t}, Ref{Csize_t}),
            world._native_id, type, a_id, b_id)

        if !new_event
            event.type = CONTACT_END
            event.shape_a_id = nothing
            event.shape_b_id = nothing
            return false
        end

        if type[]
            event.type = CONTACT_START
        else
            event.type = CONTACT_END
        end

        event.shape_a_id = a_id[]
        event.shape_b_id = b_id[]

        return true
    end
    export next_event!; next_event = next_event!

    ### TEST #############################################################################

    module _test

        import Main.ts; using Main.ts
        import Test; using Test

        function compare(a::Vector2f, b::Vector2f) ::Bool
            return abs(a.x - b.x) < 0.1 && abs(a.y - b.y) < 0.1
        end

        function compare(a::RGBA, b::RGBA) ::Bool
            return abs(a.red - b.red) < 0.01 &&
                   abs(a.green - b.green) < 0.01 &&
                   abs(a.blue - b.blue) < 0.01 &&
                   abs(a.alpha - b.alpha) < 0.01
        end

        function run()

            test_icon = "../docs/_static/favicon.png"
            @test ts.initialize()

            @testset "Colors" begin

                rgb_in = RGBA(1.0, 0.5, 0.5, 1.0)
                hsv = as_hsv(rgb_in)
                rgb = as_rgb(hsv)
                @test rgb.red == rgb_in.red
                @test rgb.green == rgb_in.green
                @test rgb.blue == rgb_in.blue
                @test rgb.alpha == rgb_in.alpha
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
                sleep(0.01)
                @test as_seconds(elapsed(clock)) > 0.005
                @test as_seconds(restart(clock)) > 0.005
                @test as_seconds(elapsed(clock)) < 0.05
            end

            @testset "Angle" begin

                ns = 123
                angle = degrees(ns)
                @test as_degrees(angle) == ns
                @test as_degrees(radians(as_radians(angle))) == ns
            end

            @testset "Window" begin

                window = ts.Window("", 1, 1, ts.DEFAULT)

                @test is_open(window)
                @test has_focus(window) == false
                @test has_mouse_focus(window) == false

                @test get_size(window) == Vector2ui(1, 1)
                @test get_position(window).x >= 0
                @test get_position(window).y >= 0

                set_hidden!(window, true)
                @test is_hidden(window) == true
                set_hidden!(window, false)
                @test is_hidden(window) == false

                minimize!(window)
                @test is_minimized(window) == true

                maximize!(window)
                @test is_maximized(window) == true

                clear!(window)

                shape = RectangleShape(Vector2f(0, 0), Vector2f(10, 10));
                render!(window, shape)
                flush!(window)

                set_icon!(window, test_icon)

                set_framerate_limit!(60)
                @test get_framerate_limit() == 60

                start_frame!(window)
                end_frame!(window)

                close!(window)
            end

            @testset "Camera" begin

                window = Window(1, 1)
                camera = Camera(window)

                @test camera._native_window_id == window._native_id

                center_on!(camera, Vector2f(50, 50))
                @test get_center(camera) == Vector2f(50, 50)

                center_on!(camera, Vector2f(0, 0))
                move!(camera, 50, 50)
                @test get_center(camera) == Vector2f(50, 50)

                zoom_in!(camera, 0.5)
                zoom_out!(camera, 2)
                set_zoom!(camera, 1)
                rotate!(camera, degrees(90))
                set_rotation!(camera, degrees(0))

                transform = get_transform(camera)
                reset!(transform)
                set_transform!(camera, transform)

                reset!(camera)
                area = get_view_area(camera)
            end

            @testset "Textures" begin

                for e in instances(TextureFilteringMode)
                    @test Int32(e) > -2
                end

                for e in instances(TextureBlendMode)
                    @test Int32(e) > -2
                end

                window = Window(1, 1)

                render = RenderTexture(window, 100, 100)
                @test render._native != Ptr{Cvoid}()

                static = StaticTexture(window, 100, 100, RGBA(0.5, 1, 1, 1))
                @test static._native != Ptr{Cvoid}()

                static = StaticTexture(window, test_icon)
                @test static._native != Ptr{Cvoid}()

                set_color!(render, RGBA(0, 0, 0, 1))
                @test get_color(render) == RGBA(0, 0, 0, 1)

                set_blend_mode!(render, ts.ADD)
                @test get_blend_mode(render) == ts.ADD

                set_filtering_mode!(render, ts.LINEAR)
                @test get_filtering_mode(render) == ts.LINEAR

                @test get_size(render) == Vector2ui(100, 100)
            end

            @testset "InputHandler" begin

                # don't actually query input, just check if the function crashes
                # asking the user to press specific buttons during the test would be annoying

                function test_input(x)
                    @test InputHandler.was_pressed(x) == false || was_pressed(x) == true
                    @test InputHandler.was_released(x) == false || was_released(x) == true
                    @test InputHandler.is_down(x) == false || is_down(x) == true
                    @test InputHandler.has_state_changed(x) == false || has_state_changed(x) == true
                end

                @testset "Keyboard" begin
                    for e in instances(KeyboardKey)
                        test_input(e)
                    end
                end

                @testset "Mouse" begin
                    for e in instances(MouseButton)
                        test_input(e)
                    end

                    cursor_pos = get_cursor_position()
                    scrollwheel_pos = get_scrollwheel()
                end

                @testset "Controller" begin
                    for e in instances(ControllerButton)
                        test_input(e)
                    end

                    InputHandler.get_controller_axis_left()
                    InputHandler.get_controller_axis_right()
                    InputHandler.get_controller_trigger_left()
                    InputHandler.get_controller_trigger_right()
                end
            end

            @testset "Audio" begin

                @testset "Music" begin

                    ms = ts.MusicHandler

                    @test ms.sample_rate > 0
                    music = Music("../test/otherworldy_foe.mp3")
                    @test music._native_id != Ptr{Cvoid}()

                    ms.play!(music)
                    ms.set_volume!(0)
                    ms.play_next!(music)

                    ms.skip_to!(seconds(0.1))

                    @test ms.is_playing()

                    ms.pause!()
                    @test ms.is_paused()
                    ms.unpause!()

                    ms.next!()

                    ms.set_volume!(0)
                    @test ms.get_volume() == 0

                    ms.play_next!(music)
                    ms.clear_next!()

                    ms.stop!()
                    @test ms.is_stopped()

                    ms.play!(music)
                    ms.force_stop!()

                    @test ms.is_stopped();
                end

                @testset "Sound" begin

                    sh = ts.SoundHandler
                    @test sh.n_channels > 0

                    sound = Sound("../test/ok_desu_ka.mp3", 0)

                    for i in 1:sh.n_channels
                        sh.play!(i, sound)
                        sh.stop!(i)
                    end

                    sh.play!(1, sound)
                    sh.set_volume!(1, 0)
                    @test sh.get_volume(1) == 0

                    sh.set_panning!(1, degrees(90))
                    @test as_degrees(sh.get_panning(1)) == 90

                    @test sh.is_playing(1)
                    sh.pause!(1)
                    @test sh.is_paused(1)
                    sh.unpause!(1)
                    @test sh.is_playing(1)
                    sh.stop!(1)
                    @test sh.is_stopped(1)

                    sh.force_stop!(1)
                end
            end

            @testset "Shapes" begin

                rectangle = RectangleShape(Vector2f(0, 0), Vector2f(1, 1))
                @test get_top_left(rectangle) == Vector2f(0, 0)
                @test get_size(rectangle) == Vector2f(1, 1)

                set_top_left!(rectangle, Vector2f(1, 1))
                @test get_top_left(rectangle) == Vector2f(1, 1)
                set_size!(rectangle, Vector2f(0.5, 0.5))
                @test get_size(rectangle) == Vector2f(0.5, 0.5)

                circle = CircleShape(Vector2f(1, 1), 1, 16)
                @test get_radius(circle) == 1
                set_radius!(circle, 2)
                @test get_radius(circle) == 2

                polygon = PolygonShape(Vector2f(0, 0), Vector2f(1, 0), Vector2f(1, 1), Vector2f(0, 1))
                triangle = TriangleShape(Vector2f(0, 1), Vector2f(1, 0), Vector2f(0.5, 0.5))

                shapes = Shape[triangle, rectangle, triangle, polygon]

                for shape in shapes

                    set_centroid!(shape, Vector2f(12, 15))
                    @test compare(get_centroid(shape), Vector2f(12, 15))

                    set_centroid!(shape, Vector2f(0, 0))
                    move!(shape, 10.0, 10.0)
                    @test get_centroid(shape) == Vector2f(10, 10)

                    set_color!(shape, RGBA(0.5, 0.5, 0.5, 1))
                    @test compare(get_vertex_color(shape, 1), RGBA(0.5, 0.5, 0.5, 1))

                    window = Window(1, 1)
                    set_texture!(shape, StaticTexture(window, 100, 100, RGBA(1, 1, 1, 1)))
                    close!(window)

                    set_texture_rectangle!(shape, Rectangle(Vector2f(0, 0), Vector2f(0, 0)))
                    @test get_texture_rectangle(shape) == Rectangle(Vector2f(0, 0), Vector2f(0, 0))

                    get_bounding_box(shape)
                    @test get_n_vertices(shape) > 0

                    for vertex_i in 1:get_n_vertices(shape)

                        set_vertex_position!(shape, 1, Vector2f(50, 50))
                        @test compare(get_vertex_position(shape, 1), Vector2f(50, 50))

                        set_vertex_color!(shape, 1, RGBA(0.5, 0.5, 0.5, 1))
                        @test compare(get_vertex_color(shape, 1), RGBA(0.5, 0.5, 0.5, 1))

                        set_vertex_texture_coordinates!(shape, 1, Vector2f(1, 1))
                        @test compare(get_vertex_texture_coordinates(shape, 1), Vector2f(1, 1))
                    end
                end
            end

            @testset "Transform" begin

                transform = Transform()

                for x in 1:3, y in 1:3
                    if x == y
                        @test transform[x, y] == 1
                    else
                        @test transform[x, y] == 0
                    end
                end

                point = Vector2f(12, 15)
                apply_to(transform, point)
                @test point.x == 12 && point.y == 15

                transform[1, 1] = 999
                @test transform[1, 1] == 999

                reset!(transform)
                @test transform[1, 1] == 1

                translate!(transform, 1, 1)
                point = Vector2f(0, 0)
                point = apply_to(transform, point)
                @test point.x == 1 && point.y == 1
                reset!(transform)

                point = Vector2f(1, 1)
                rotate!(transform, degrees(180), Vector2f(0, 0))
                point = apply_to(transform, point)
                @test round(point.x) == -1 && round(point.y) == -1
                reset!(transform)

                point = Vector2f(1, 1)
                scale!(transform, 2, 2)
                point = apply_to(transform, point)
                @test round(point.x) == 2 && round(point.y) == 2
                reset!(transform)

                point = Vector2f(1, 1)
                shear!(transform, 1, 1)
                point = apply_to(transform, point)
                @test round(point.x) == 2 && round(point.y) == 2
                reset!(transform)

                point = Vector2f(1, 1)
                reflect!(transform, true, true, Vector2f(0, 0))
                point = apply_to(transform, point)
                @test point.x == -1 && point.y == -1
                reset!(transform)
            end

            @testset "Physics" begin

                world = PhysicsWorld()
                @test world._native_id != 0

                set_gravity!(world, Vector2f(-1, 1))
                @test compare(get_gravity(world), Vector2f(-1, 1))

                clear_forces!(world)
                step!(world, seconds(1))

                rect_geometry = Rectangle(Vector2f(0, 0,), Vector2f(100, 100))
                rect_shape = RectangleShape(rect_geometry)
                rect = CollisionRectangle(world, ts.DYNAMIC, rect_geometry)
                @test rect._native != Ptr{Cvoid}()
                rect = CollisionRectangle(world, ts.DYNAMIC, rect_shape)
                @test rect._native != Ptr{Cvoid}()

                tri_geometry = Triangle(Vector2f(50, 50), Vector2f(150, 150), Vector2f(0, 75))
                tri_shape = TriangleShape(tri_geometry)

                tri = CollisionTriangle(world, ts.DYNAMIC, tri_geometry)
                @test tri._native != Ptr{Cvoid}()
                tri = CollisionTriangle(world, ts.DYNAMIC, tri_shape)
                @test tri._native != Ptr{Cvoid}()

                circ_geometry = Circle(Vector2f(50, 50), 100)
                circ_shape = CircleShape(circ_geometry)
                circ = CollisionCircle(world, ts.DYNAMIC, circ_geometry)
                @test circ._native != Ptr{Cvoid}()
                circ = CollisionCircle(world, ts.DYNAMIC, circ_shape)
                @test circ._native != Ptr{Cvoid}()

                line = CollisionLine(world, ts.KINEMATIC, Vector2f(50, 50), Vector2f(10, 10))
                @test line._native != Ptr{Cvoid}()

                polygon = CollisionPolygon(world, ts.DYNAMIC, [Vector2f(50, 50), Vector2f(12, 15), Vector2f(1322, 12), Vector2f(1415, 22)])
                @test polygon._native != Ptr{Cvoid}()

                linesequence = CollisionLineSequence(world, ts.KINEMATIC, [Vector2f(50, 50), Vector2f(12, 15), Vector2f(1322, 12), Vector2f(1415, 22)])
                @test linesequence._native != Ptr{Cvoid}()

                shapes = [rect, tri, circ, polygon] #line, linesequence, ]

                for shape in shapes

                    set_density!(shape, 12)
                    @test get_density(shape) == 12

                    set_friction!(shape, 12)
                    @test get_friction(shape) == 12

                    set_restitution!(shape, 12)
                    @test get_restitution(shape) == 12

                    @test get_centroid(shape) != Vector2f(0, 0)
                    aabb = get_bounding_box(shape)
                    @test compare(get_centroid(shape), Vector2f(aabb.top_left.x + aabb.size.x * 0.5, aabb.top_left.y + aabb.size.y * 0.5))

                    get_rotation(shape)._degrees

                    set_type!(shape, ts.KINEMATIC)
                    @test get_type(shape) == ts.KINEMATIC

                    get_origin(shape)
                    get_center_of_mass_local(shape)
                    get_center_of_mass_global(shape)

                    set_linear_velocity!(shape, Vector2f(12, 13))
                    step!(world, seconds(0.01));
                    get_linear_velocity(shape).x > 0 && get_linear_velocity(shape).y > 0
                    set_linear_velocity!(shape, Vector2f(0, 0))

                    set_angular_velocity!(shape, 12)
                    @test get_angular_velocity(shape) == 12
                    set_angular_velocity!(shape, 0)

                    apply_force_to!(shape, Vector2f(12, 12), Vector2f(0, 0))
                    apply_force_to_center!(shape, Vector2f(12, 12))

                    apply_torque!(shape, 12)
                    apply_linear_impulse_to!(shape, Vector2f(12, 13), Vector2f(0, 0));
                    apply_linear_impulse_to_center!(shape, Vector2f(12, 13))

                    @test get_mass(shape) >= 0
                    @test get_inertia(shape) >= 0

                    set_is_bullet!(shape, true)
                    @test get_is_bullet(shape)
                    set_is_bullet!(shape, false)

                    set_is_rotation_fixed!(shape, true)
                    @test get_is_rotation_fixed(shape) == true

                    @test get_id(shape) > 0

                    set_is_hidden(shape, true)
                    @test get_is_hidden(shape) == true
                    set_is_hidden(shape, false)

                    @test is_point_in_shape(world, shape, get_centroid(shape))

                    raycast = ray_cast(world, shape, Vector2f(0, 0), get_centroid(shape), 100000)
                    #@test raycast.are_colliding == true #TODO

                    distance = distance_between(world, shape, rect)
                    @test distance.distance >= 0
                end

                event = CollisionEvent()
                @test next_event!(world, event)
                @test event.shape_a_id != nothing && event.shape_b_id != nothing
            end
        end
        # no export
    end

    """
    `runtests() -> Nothing`
    """
    function runtests() ::Nothing

        printstyled("Running tests...\n", bold = true);
        Main.ts._test.run();
        printstyled("done.\n", bold = true);
    end
    # no export
end