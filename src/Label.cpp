#include "Label.h"
#include "game.h"
#include <SDL3/SDL.h>

Label::Label(GameState* gameStat, Texture* texture, float x, float y)
	: GameObject (gameStat), texture(texture), x(x),y(y) {
}

void Label::render() const {
	SDL_FRect rect{ x, y, (float)texture->getFrameWidth(), (float)texture->getFrameHeight() };
	texture->render(rect);
}

void Label::update(){}