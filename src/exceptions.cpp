#include <include/exceptions.hpp>

namespace ts
{
    const char * Exception::what() const noexcept
    {
        return _message.c_str();
    }

    SDLErrorException::SDLErrorException(const char* sdl_error_message)
    {
        std::stringstream str;
        str << "[SDL_ERROR] " << sdl_error_type << ": " << sdl_error_message << std::endl;
        _message = str.str();
    }

    bool SDLErrorException::is_fatal() const
    {
        return false;
    }

    void forward_sdl_error()
    {
        auto exc = SDLErrorException(SDL_GetError());
        SDL_ClearError();
        throw exc; // sic, clang-tidy is wrong
    }

}

