#ifndef __FIGHTERCTRL_H__
#define __FIGHTERCTRL_H__

#include "../ecs/Component.h"
#include "../ecs/Entity.h"
#include "../components/Transform.h"
#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include <cassert>

class FighterCtrl : public ecs::Component {
public:
	__CMPID_DECL__(ecs::cmp::FIGHTERCTRL)

		FighterCtrl();
	virtual ~FighterCtrl();

	void initComponent() override;
	void update() override;
private:
	Transform* _tr;
	float _thrust;
	float _speedLimit;
};

#endif // __FIGHTERCTRL_H__