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
    /// \brief general exception class
    struct Exception : public std::exception
    {
        /// \brief should the runtime end if this exception is thrown
        virtual bool is_fatal() const = 0;

        /// \brief get message
        const char* what() const noexcept override;

        protected:
            std::string _message;
    };

    /// \brief exception wrapper around SDLError
    struct SDLErrorException : public Exception
    {
        /// \brief ctor
        SDLErrorException(const char* sdl_error_message);

        /// \brief should the runtime end if this exception is thrown
        bool is_fatal() const;

        private:
            using Exception::_message;
    };

    /// \brief forward SDL error. Assumes error was actually thrown, do not call this if no error is present on the error stack
    void forward_sdl_error();
}