#include "ShowAtOpposieSide.h"

ShowAtOpposieSide::ShowAtOpposieSide()
{
}
ShowAtOpposieSide:: ~ShowAtOpposieSide() {}

void ShowAtOpposieSide::initComponent() {
	_tr = _ent->getComponent<Transform>();
	assert(_tr != nullptr);
}
//calcula si se sale de la pantalla y lo mueve al otro lado
void ShowAtOpposieSide::update() {
	Vector2D& pos = _tr->getPos();

	float w = _tr->getWidth();
	float h = _tr->getHeight();

	if (pos.getX() > sdlutils().width()) {
		pos.setX(-w);
	}
	else if (pos.getX() + w < 0) {
		pos.setX(sdlutils().width());
	}

	if (pos.getY() > sdlutils().height()) {
		pos.setY(-h);
	}
	else if (pos.getY() + h < 0) {
		pos.setY(sdlutils().height());
	}
}
