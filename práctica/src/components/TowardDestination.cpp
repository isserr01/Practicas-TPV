#include "TowardDestination.h"

TowardDestination::TowardDestination() :
    _tr(nullptr),
    _dest()
{
}

TowardDestination::~TowardDestination() {
}

void TowardDestination::init() {
    _tr = _ent->getComponent<Transform>();
    assert(_tr != nullptr);
    pickNewDestination();
}

void TowardDestination::pickNewDestination() {
    _dest = Vector2D(
        sdlutils().rand().nextInt(0, sdlutils().width()),
        sdlutils().rand().nextInt(0, sdlutils().height())
    );
}

void TowardDestination::update() {

    Vector2D pos = _tr->getPos();
    Vector2D& vel = _tr->getVel();

    if ((pos - _dest).magnitude() < 10.0f)
        pickNewDestination();

    vel = vel.rotate(vel.angle(_dest - pos));
}