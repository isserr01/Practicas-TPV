#pragma once
#include "../ecs/Component.h"
#include "../ecs/Entity.h"
#include "../components/Transform.h"
#include "../sdlutils/SDLUtils.h"
#include <cassert>

class ShowAtOpposieSide : public ecs::Component {
public:
    ShowAtOpposieSide() {}
    virtual ~ShowAtOpposieSide() {}

    void init() override;

    void update()override;

private:
    Transform* _tr;
}; 