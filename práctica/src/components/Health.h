#ifndef __HEALTH_H__
#define __HEALTH_H__


#include "../ecs/Component.h"
#include "../ecs/Entity.h"
#include "../components/Transform.h"
#include "../sdlutils/SDLUtils.h"
#include <cassert>

class Health : public ecs::Component {
public:

	__CMPID_DECL__(ecs::cmp::HEALTH)

    Health();
    virtual ~Health();

    void render() override;

    void subLives(int n);
    void resetLives();
    int getLives() const;

private:
    int _lives;
};

#endif // __HEALTH_H__  