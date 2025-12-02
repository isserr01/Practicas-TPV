#include "Button.h"
#include "GameState.h"
#include <SDL3/SDL.h>

Button::Button(GameState* gameStat, Texture* texture, float x, float y)
	: Label(gameStat, texture, x, y)
{
	gameStat->addEventListener(this);
}

void Button::connect(Callback&& func)
{
	callback = func;
}

void Button::handleEvent(const SDL_Event& event)
{
	if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
		float mouseX = event.button.x;
		float mouseY = event.button.y;
		SDL_FRect rect{ x, y, texture->getFrameWidth(), texture->getFrameHeight() };
		
		if(mouseX>=rect.x&&mouseX<=rect.x+rect.w&&
		   mouseY>=rect.y&&mouseY<=rect.y+rect.h) {
			if (callback) {
				callback();
			}
		}
	}
}

void Button::update()
{
	float mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);

	SDL_FRect rect{ x, y, texture->getFrameWidth(), texture->getFrameHeight() };


}

void Button::render() const
{
	SDL_FRect rect{ x, y, texture->getFrameWidth(), texture->getFrameHeight() };

	SDL_Color color = (mouseOver())
		? SDL_Color{ 255, 255, 0, 255 }
	: SDL_Color{ 255, 255, 255, 255 };

	texture->render(rect, color);
}

bool Button::mouseOver() const
{
	float mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);

	SDL_FRect rect{ x, y, texture->getFrameWidth(), texture->getFrameHeight() };
	return mouseX >= rect.x && mouseX <= rect.x + rect.w &&
		mouseY >= rect.y && mouseY <= rect.y + rect.h;
}