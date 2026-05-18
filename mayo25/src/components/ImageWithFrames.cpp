#include "ImageWithFrames.h"
#include "Image.h"
#include "../sdlutils/Texture.h"
#include "../ecs/Entity.h"
#include "../sdlutils/SDLUtils.h"
#include "../game/Game.h"

ImageWithFrames::ImageWithFrames(const Texture* tex, int cols, int rows, float frameTime)
	: _tex(tex), _cols(cols), _rows(rows),
	 _frameTime(frameTime),
	_index(0),
	_elapsedTime(0.0f), _tr(nullptr),
	_lastFrameTime(0),
	_frameDelayMs(static_cast<int>(frameTime * 1000))
{
	_totalFrames = _cols * _rows;
	_frameWidth = _tex->width() / _cols;
	_frameHeight = _tex->height() / _rows;
}

void ImageWithFrames::initComponent()
{
	_tr = Game::Instance()->getMngr()->getComponent<Transform>(_ent);
	assert(_tr != nullptr);
}

void ImageWithFrames::update()
{
	if (_frames.empty()) return;

	Uint64 currentTime = sdlutils().currTime();  // Esto devuelve el tiempo registrado

	if (_lastFrameTime == 0) {
		_lastFrameTime = currentTime;
		return;
	}

	// Verificar si ha pasado suficiente tiempo
	if (currentTime - _lastFrameTime >= _frameDelayMs) {
		_index = (_index + 1) % _frames.size();
		_lastFrameTime = currentTime;

	}
}


void ImageWithFrames::render()
{
	if (_tr == nullptr || _frames.empty()) return;

	int frame = _frames[_index];

	SDL_FRect src{
		float((frame % _cols) * _frameWidth),
		float((frame / _cols) * _frameHeight),
		float(_frameWidth),
		float(_frameHeight)
	};

	SDL_FRect dest{
		_tr->_pos.getX(),
		_tr->_pos.getY(),
		_tr->_width,
		_tr->_height
	};

	_tex->render(src, dest, _tr->_rot);
}

void ImageWithFrames::setFrames(const std::vector<int>& frames)
{
	_frames = frames;
	_index = 0;
}






