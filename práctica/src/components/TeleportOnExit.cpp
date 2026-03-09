#include "TeleportOnExit.h"


TeleportOnExit::TeleportOnExit() :
	_tr(nullptr)
{
}
TeleportOnExit:: ~TeleportOnExit() {}

void TeleportOnExit::initComponent() {
	_tr = _ent->getComponent<Transform>();
	assert(_tr != nullptr);
}
//calcula si se sale de la pantalla y lo mueve al otro lado
void TeleportOnExit::update() {
	Vector2D& pos = _tr->getPos();

	float w = _tr->getWidth();
	float h = _tr->getHeight();

	bool outside =
		pos.getX() > sdlutils().width() ||
		pos.getX() + w < 0 ||
		pos.getY() > sdlutils().height() ||
		pos.getY() + h < 0;

	if (outside) {

		int screenW = sdlutils().width();
		int screenH = sdlutils().height();

		int side = sdlutils().rand().nextInt(0, 4);

		switch (side) {
		case 0: pos = Vector2D(0, sdlutils().rand().nextInt(0, screenH)); break;
		case 1: pos = Vector2D(screenW, sdlutils().rand().nextInt(0, screenH)); break;
		case 2: pos = Vector2D(sdlutils().rand().nextInt(0, screenW), 0); break;
		case 3: pos = Vector2D(sdlutils().rand().nextInt(0, screenW), screenH); break;
		}
	}
}
