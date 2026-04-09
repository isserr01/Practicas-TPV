#include "ImageWithFrames.h"
#include "Image.h"
#include "../sdlutils/Texture.h"
#include "../ecs/Entity.h"
#include "../sdlutils/SDLUtils.h"
#include "../game/Game.h"

//ImageWithFrames::ImageWithFrames(const Texture* tex, int cols, int rows, float frameTime)
//	: _cols(cols), _rows(rows),
//	_currentFrame(0), _elapsedTime(0.0f),
//	_frameTime(frameTime),
//	_tex(tex)
//{
//	_totalFrames = _cols * _rows;
//	_tr = nullptr;
//	_frameWidth = _tex->width() / _cols;
//	_frameHeight = _tex->height() / _rows;
//
//}
//
//void ImageWithFrames::initComponent()
//{
//	_tr = Game::Instance()->getMngr()->getComponent<Transform>();
//	assert(_tr != nullptr);
//}
//
//void ImageWithFrames::update()
//{
//	_elapsedTime += sdlutils().deltaTime();
//	if (_elapsedTime >= _frameTime) {
//		_currentFrame = (_currentFrame + 1) % _totalFrames;
//		_elapsedTime = 0.0f;
//	}
//}
//
//void ImageWithFrames::render()
//{
//	if (!_tr) return;
//	SDL_FRect dest = build_sdlfrect(_tr->getPos(), _tr->getWidth(),
//		_tr->getHeight());
//
//	SDL_FRect src{
//		(_currentFrame % _cols) * _frameWidth,
//		(_currentFrame / _cols) * _frameHeight,
//		_frameWidth, _frameHeight
//	};
//
//	_tex->render(src, dest, _tr->getRot());
//
//}






