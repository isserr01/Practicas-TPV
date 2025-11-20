#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include <SDL3/SDL.h>

class EventHandler
{
public:
    virtual ~EventHandler() {}

    virtual void handleEvent(const SDL_Event& event) = 0;

protected:
    // Función útil para detectar si el ratón está dentro de un rectángulo
    static bool isMouseInside(const SDL_Rect& rect, int mx, int my)
    {
        return (mx >= rect.x && mx <= rect.x + rect.w &&
            my >= rect.y && my <= rect.y + rect.h);
    }
};

#endif