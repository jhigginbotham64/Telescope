// 
// Copyright 2022 Joshua Higginbotham
// Created on 20.05.22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <exception>
#include <string>
#include <sstream>

#include <SDL2/SDL_error.h>

namespace ts
{
    //
    struct Exception : public std::exception
    {
        //
        virtual bool is_fatal() const = 0;

        //
        const char* what() const noexcept override;

        protected:
            std::string _message;
    };

    //
    struct SDLErrorException : public Exception
    {
        //
        SDLErrorException(const char* sdl_error_message);

        //
        bool is_fatal() const;

        private:
            using Exception::_message;
    };

    //
    // warning: assumes error was thrown, do not call if no error is present
    void forward_sdl_error();
}