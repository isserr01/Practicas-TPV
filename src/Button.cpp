#include "Button.h"
#include "GameState.h"
#include <SDL3/SDL.h>

Button::Button(GameState* gameState, Texture* texture, SDL_FRect rect)
	: Label(gameState, texture, rect),onClick(onClick), clicked(false)
{
	if (texture) {
		
	}
	gameState->addEventListener(this);
}