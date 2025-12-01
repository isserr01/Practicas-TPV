#include "Label.h"
#include "game.h"
#include <SDL3/SDL.h>

Label::Label(GameState* gameStat, Texture* texture, SDL_Rect rect)
	: GameObject (gameStat), texture(texture), rect(rect) {
}

void Label::render() const {
	if (!texture)
		return;
	else {
		texture->render(
			SDL_FRect{ (float)rect.x, (float)rect.y, (float)rect.w, (float)rect.h });
	}
}