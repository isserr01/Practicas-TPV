#ifndef TELEPORTONEXIT_H
#define TELEPORTONEXIT_H

#include "../ecs/Component.h"
#include "../ecs/Entity.h"
#include "../components/Transform.h"
#include "../sdlutils/SDLUtils.h"
#include <cassert>
#pragma once

class TeleportOnExit : public ecs::Component {
public:
	__CMPID_DECL__(ecs::cmp::TELEPORTONEXIT)
    TeleportOnExit();
    virtual ~TeleportOnExit();

    void initComponent() override;

    void update()override;

private:
    Transform* _tr;
};

#endif // TELEPORTONEXIT_H