#pragma once
#include "../ecs/Component.h"
#include "../ecs/Entity.h"
#include "../components/Transform.h"
#include "../sdlutils/SDLUtils.h"
#include <cassert>

class TowardDestination : public ecs::Component {
public:

    TowardDestination();
    virtual ~TowardDestination();

    void init();
    void update() override;

private:
    void pickNewDestination();

    Transform* _tr;
    Vector2D _dest;
};