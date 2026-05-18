// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include "../ecs/System.h"
#include"../components/ImageWithFrames.h"

class Texture;
struct Transform;

class RenderSystem: public ecs::System {
public:

	RenderSystem();
	virtual ~RenderSystem();
	void initSystem() override;
	void update() override;
private:
	void drawStars();
	void drawShields();
	void drawPacMan();
	void drawMsgs();
	void drawUI();
	void drawGhosts();
	void draw(Transform *tr, const Texture *tex, ImageWithFrames* anim);
};

