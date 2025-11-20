#ifndef GAME_ERROR_H
#define GAME_ERROR_H
#include <stdexcept>
#include <string>
#include <SDL3/SDL.h>

class GameError : public std::logic_error
{


public:
      using logic_error::logic_error;
};


class SDLError : public GameError {
public:
    SDLError(const std::string& msg)
        : GameError(msg + ": " + SDL_GetError())
    {
    }
};

class FileNotFoundError : public GameError {
public:
     FileNotFoundError(const std::string& filename)
         : GameError("No se pudo abrir el archivo: " + filename)
     {
     }
};


class FileFormatError : public GameError {
public:
     FileFormatError(const std::string& filename, const std::string& msg)
         : GameError("Error en archivo '" + filename + ": " + msg)
      {
      }
};

#endif // GAME_ERROR_H

