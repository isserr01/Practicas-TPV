#pragma once

#include <SDL_stdinc.h>
#include "../sdlutils/SDLUtils.h"
#pragma once

#include <SDL_stdinc.h>
#include "../sdlutils/SDLUtils.h"
#include "../ecs/Component.h"

struct Miracle : public ecs::Component {

    enum State { NORMAL, MIRACULOUS };

    State state;
    Uint32 lastSwitchTime; // tiempo del último cambio
    Uint32 N; // duración normal
    Uint32 M; // duración milagrosa

    Miracle() {
        auto& rand = sdlutils().rand();
        state = NORMAL;
        N = rand.nextInt(10, 20);   // duración normal
        M = 0;                      // se asigna al entrar en milagroso
		lastSwitchTime = sdlutils().virtualTimer().currTime()/500.0f;
    }

    bool isMiraculous() const {
        return state == MIRACULOUS;
    }

    // Devuelve true si cambia de estado
    bool switchState(Uint32 currTime) {

        Uint32 elapsed = currTime - lastSwitchTime;

        if (state == NORMAL) {
            if (elapsed >= (Uint32)N) {
                // Pasar a milagroso
                auto& rand = sdlutils().rand();
                M = rand.nextInt(1, 5);
                state = MIRACULOUS;
                lastSwitchTime = currTime;
                return true;
            }
        }
        else { // MILAGROSO
            if (elapsed >= (Uint32)M) {
                // Volver a normal
                auto& rand = sdlutils().rand();
                N = rand.nextInt(10, 20);
                state = NORMAL;
                lastSwitchTime = currTime;
                return true;
            }
        }

        return false;
    }

    // Reset al empezar ronda
    void reset(Uint32 now) {
        auto& rand = sdlutils().rand();
        state = NORMAL;
        N = rand.nextInt(10, 20);
        M = 0;
        lastSwitchTime = now;
    }
};