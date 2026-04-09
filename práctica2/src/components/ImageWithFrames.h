#ifndef IMAGE_WITH_FRAMES_H
#define IMAGE_WITH_FRAMES_H

#include "../ecs/Component.h"
#include "../components/Transform.h"
#include "Image.h"
#include "../sdlutils/Texture.h"
#include "../ecs/Entity.h"
#include "../sdlutils/SDLUtils.h"

struct ImageWithFrames : public ecs::Component {

	const Texture* _tex;
	int _frameWidth, _frameHeight;
	int _cols, _rows;
	int _currentFrame;
	int _totalFrames;
	float _frameTime;
	float _elapsedTime;
	Transform* _tr;

	ImageWithFrames(const Texture* tex, int cols, int rows, float frameTime)
		: _cols(cols), _rows(rows),
		_currentFrame(0), _elapsedTime(0.0f),
		_frameTime(frameTime),
		_tex(tex)
	{
		_totalFrames = _cols * _rows;
		_tr = nullptr;
		_frameWidth = _tex->width() / _cols;
		_frameHeight = _tex->height() / _rows;

	}

	void initComponent()
	{
		//aquí hay que crear una instancia del objeto actual en cuestión :3
		//el objeto siendo el parámetro "e" que le pasamos a getComponent
		assert(_tr != nullptr);
		//_tr = Game::Instance()->getMngr()->getComponent<Transform>(e);
	}

	void update()
	{
		_elapsedTime += sdlutils().deltaTime();
		if (_elapsedTime >= _frameTime) {
			_currentFrame = (_currentFrame + 1) % _totalFrames;
			_elapsedTime = 0.0f;
		}
	}

	void render()
	{
		if (!_tr) return;
	/*	SDL_FRect dest = build_sdlfrect(_tr->getPos(), _tr->getWidth(),
			_tr->getHeight());*/

		SDL_FRect src{
			(_currentFrame % _cols) * _frameWidth,
			(_currentFrame / _cols) * _frameHeight,
			_frameWidth, _frameHeight
		};

		//_tex->render(src, dest, _tr->getRot());

	}

};

#endif // IMAGE_WITH_FRAMES_H