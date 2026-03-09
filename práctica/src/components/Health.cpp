#include "Health.h"

Health::Health() :
	_lives(3)
{
}
Health::~Health() {}

//renderiza vidas en pantalla
void Health::render() {
	auto liveText = &sdlutils().images().at("heart");

	for (int i = 0; i < _lives; i++) {
		SDL_FRect dst = { 10 + i * 40, 10, 30, 30 };
		liveText->render(dst);
	}
}

//resta vidas
void Health::subLives(int n) {
	_lives -= n;
	if (_lives < 0) {
		_lives = 0;
	}
}

//resetea vidas (a 3)
void Health::resetLives() {
	_lives = 3;
}
int Health::getLives() const {
	return _lives;
}
