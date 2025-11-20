#ifndef BUTTON_H
#define BUTTON_H

#include "EventHandler.h"
#include <SDL3/SDL.h>
#include <functional>
#include <utility>


class Button : public EventHandler
{
	SDL_Renderer* renderer;
	SDL_Rect rect;
	SDL_Color color;
	SDL_Color hoverColor;

	bool hovered = false;
	std::function<void()> onClick;

public:

	Button(SDL_Renderer* r, int x, int y, int w, int h, std::function<void()> click)
		: renderer(r), rect{ x, y, w, h }, onClick(std::move(click))
	{
		color = { 70, 130, 180, 255 };       
		hoverColor = { 100, 149, 237, 255 };

	}

	void handleEvent(const SDL_Event& event) override;
	void render() const;
};

#endif // BUTTON_H
