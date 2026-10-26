#pragma once

#include <SDL_stdinc.h>
#include "../sdlutils/SDLUtils.h"
#include "../ecs/Component.h"
#include "../utils/Vector2D.h"

const int minT = 3000;
const int maxT = 8000;
const int V = 10;

struct Mortal : public ecs::Component
{
    Vector2D _birthPos;
    int T;

public:

    Mortal(Vector2D birthPos) {
        auto& rand = sdlutils().rand();
        T = rand.nextInt(minT, maxT);   // duración normal     
		_birthPos = birthPos;    
    }

    void crash() {
		T -= (T * V) / 100;
	}

    Vector2D getBirthPos() const {
        return _birthPos;
	}

};

