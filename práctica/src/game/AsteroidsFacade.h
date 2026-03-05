#pragma once
#include "../ecs/ecs.h"

class AsteroidsFacade {
public:
    AsteroidsFacade() {}
    virtual ~AsteroidsFacade() {}

    virtual void create_asteroids() = 0;
    virtual void reset_asteoirds() = 0;
    virtual void reset_asteroids() = 0;
    
};