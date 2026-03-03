#pragma once
#include "../ecs/Component.h"
#include "../components/Transform.h"
#include "../game/Game.h"
#include <cassert>

class Follow : public ecs::Component {
public:

    Follow();
    virtual ~Follow();

    void init();
    void update() override;

private:
    Transform* _tr;
};