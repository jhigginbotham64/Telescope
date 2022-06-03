#
#
#
#

#module ts

    macro _ccall(name::QuoteNode, return_t::Symbol, args...)

        return :(ccall(($name, "./libtelescope.so"), $return_t, $(Expr(:tuple, typeof.(args)...)), $(args...)))
    end

    function get_keyboard_key(str::String) ::Int32

    end

    function get_mouse_button(str::String) ::Int32

    end

    function get_controller_button(str::String) ::Int32

    end


#end
