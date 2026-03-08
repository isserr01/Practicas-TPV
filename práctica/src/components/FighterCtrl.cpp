#include "FighterCtrl.h"

constexpr float PI = 3.14159265f;



FighterCtrl::FighterCtrl() :
    _tr(nullptr), _thrust(0.05f), _speedLimit(1.0f) {
}

 FighterCtrl::~FighterCtrl() {}

 void FighterCtrl::initComponent() {
     _tr = _ent->getComponent<Transform>();
     assert(_tr != nullptr);


 }
 void FighterCtrl::update() {
     auto& ihdlr = ih();

     //rotacion si pulsas derecha o izquierda
     if (ihdlr.isKeyDown(SDL_SCANCODE_LEFT)) {
         _tr->setRot(_tr -> getRot() - 1.0f);
     }
     if (ihdlr.isKeyDown(SDL_SCANCODE_RIGHT)) {
         _tr->setRot(_tr->getRot() + 1.0f);
     }

     //aceleracion si pulsas arriba
     if (ihdlr.isKeyDown(SDL_SCANCODE_UP)) {

         Vector2D& vel = _tr->getVel();
         float rot = _tr->getRot();

         Vector2D newVel =
             vel + Vector2D(0, -1).rotate(rot) * _thrust;

         if (newVel.magnitude() > _speedLimit)
             newVel = newVel.normalize() * _speedLimit;

         vel = newVel;
         _tr->getVel().setY(newVel.getY());
         _tr->getVel().setX(newVel.getX());

         sdlutils().soundEffects().at("thrust").play();
     }
 }
