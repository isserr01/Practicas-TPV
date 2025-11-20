#include "Label.h"
#include "game.h"
#include <SDL3/SDL.h>

Label::Label(Game* game, Texture* texture, float x, float y, float w, float h)
	: GameObject(game), texture(texture)
{
	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;
}

void Label::render() const {
	if(texture)
		texture->render(rect);
	else {
		SDL_Renderer* renderer = nullptr;
		if (renderer) {
			SDL_Rect r { static_cast<int>(rect.x), static_cast<int>(rect.y),
				static_cast<int>(rect.w), static_cast<int>(rect.h) };
			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
			
		}
	}
}