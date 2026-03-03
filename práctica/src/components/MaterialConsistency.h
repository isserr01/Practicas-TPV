#pragma once
#include "../ecs/Component.h"
#include "../ecs/Entity.h"
#include "../sdlutils/SDLUtils.h"

class MaterialConsistency : public ecs::Component {
public:

    MaterialConsistency(int consistency);
    virtual ~MaterialConsistency();

    void update() override;

private:
    int _consistency;
    Uint32 _lastCheck;
};