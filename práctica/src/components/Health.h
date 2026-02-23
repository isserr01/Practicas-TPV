#pragma once
#include "../ecs/Component.h"
#include "../ecs/Entity.h"
#include "../components/Transform.h"
#include <cassert>

class Health : public ecs::Component {
public:
    Health() {}
    virtual ~Health() {}

    void render() override;

    void subLives(int n);
    void resetLives();
    void getLives() const;

private:
    int _lives;
};