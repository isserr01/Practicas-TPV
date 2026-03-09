#pragma once
#include "../ecs/Component.h"
#include "../ecs/Entity.h"
#include "../components/Transform.h"
#include "../sdlutils/SDLUtils.h"
#include <cassert>

class ShowAtOpposieSide : public ecs::Component {
public:
	__CMPID_DECL__(ecs::cmp::SHOWATOPPOSITESIDE)

		ShowAtOpposieSide();
	virtual ~ShowAtOpposieSide();

	void initComponent() override;

	void update()override;

private:
	Transform* _tr;
};