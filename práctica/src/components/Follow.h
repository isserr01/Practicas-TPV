#pragma once
#include "../ecs/Component.h"
#include "../components/Transform.h"
#include "../game/Game.h"
#include <cassert>

class Follow : public ecs::Component {
public:
    __CMPID_DECL__(ecs::cmp::FOLLOW)
    Follow();
    virtual ~Follow();

    void initComponent() override;
    void update() override;

private:
    Transform* _tr;
};