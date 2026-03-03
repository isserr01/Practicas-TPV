#include "Health.h"

Health::Health():
    _lives(3)
{
}
Health::~Health() {}

//renderiza vidas en pantalla
void Health::render() {
    auto liveText = &sdlutils().images().at("heart");

    for (int i = 0; i < _lives; i++) {
        SDL_Rect dst = { 10 + i * 40, 10, 30, 30 };
        tex->render(dst);
    }
}

//resta vidas
void Health::subLives(int n) {
    _lives -= n;
}

//resetea vidas (a 3)
void Health::resetLives() {
    _lives = 3;
}
int Health::getLives() const {
    return _lives;
}
