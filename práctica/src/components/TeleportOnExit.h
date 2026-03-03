#pragma once
#include "../ecs/Component.h"
#include "../ecs/Entity.h"
#include "../components/Transform.h"
#include "../sdlutils/SDLUtils.h"
#include <cassert>

class TeleportOnExit : public ecs::Component {
public:
    TeleportOnExit() {}
    virtual ~TeleportOnExit() {}

    void init() override;

    void update()override;

private:
    Transform* _tr;
};