#ifndef IMAGE_WITH_FRAMES_H
#define IMAGE_WITH_FRAMES_H

#include "../ecs/Component.h"
#include "../components/Transform.h"

class Texture;

class ImageWithFrames : public ecs::Component {


private:
	const Texture* _tex;
	int _frameWidth, _frameHeight;
	int _cols, _rows;
	int _currentFrame;
	int _totalFrames;
	float _frameTime;
	float _elapsedTime;
	Transform* _tr;

public:
	__CMPID_DECL__(ecs::cmp::IMAGEWITHFRAMES)

		ImageWithFrames(const Texture* tex, int cols, int rows, float frameTime = 50);
	virtual ~ImageWithFrames() {};
	void initComponent() override;
	void update() override;
	void render() override;
};

#endif // IMAGE_WITH_FRAMES_H