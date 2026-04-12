#pragma once

#include "../ecs/Component.h"
#include "../components/Transform.h"
#include "../sdlutils/Texture.h"
#include "../sdlutils/SDLUtils.h"
#include "../game/Game.h"

struct ImageWithFrames : public ecs::Component {

	const Texture* _tex;
	int _frameWidth, _frameHeight;
	int _cols, _rows;
	int _index;
	int _totalFrames;
	float _frameTime;
	float _elapsedTime;
	std::vector<int> _frames;
	Transform* _tr;
	Uint64 _lastFrameTime;
	int _frameDelayMs;

	ImageWithFrames(const Texture* tex, int cols, int rows, float frameTime);
	void initComponent() override;
	void update();
	void render();
	void setFrames(const std::vector<int>& frames);

};

