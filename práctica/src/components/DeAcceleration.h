#pragma once
#include "../ecs/Component.h"
#include "../ecs/Entity.h"
#include "../components/Transform.h"
#include <cassert>

class DeAcceleration : public ecs::Component {
public:
	__CMPID_DECL__(ecs::cmp::DEACCELERATION)

		DeAcceleration();
	virtual ~DeAcceleration();

	void initComponent() override;
	void update() override;
private:
	Transform* _tr;
};