// This file is part of the course TPV2@UCM - Samir Genaim

#include "RenderSystem.h"

#include "../components/Image.h"
#include "../components/Transform.h"
#include "../ecs/EntityManager.h"
#include "../sdlutils/macros.h"
#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/Texture.h"
#include "GameCtrlSystem.h"
#include"../components/ImageWithFrames.h"

RenderSystem::RenderSystem() {

}

RenderSystem::~RenderSystem() {
}

void RenderSystem::initSystem() {
}

void RenderSystem::update() {
	for (auto e : _mngr->getEntities()) {

		auto anim = _mngr->getComponent<ImageWithFrames>(e);

		if (anim != nullptr)
			anim->update();
	}
	drawMsgs();
	drawStars();
	drawPacMan();
	drawGhosts();
}

void RenderSystem::drawStars() {
	// draw stars
	for (auto e : _mngr->getEntities(ecs::grp::STARS)) {

		auto tr = _mngr->getComponent<Transform>(e);
		auto tex = _mngr->getComponent<Image>(e)->_tex;
		draw(tr, tex,nullptr);
	}
}

void RenderSystem::drawPacMan() {
	auto e = _mngr->getHandler(ecs::hdlr::PACMAN);
	auto tr = _mngr->getComponent<Transform>(e);

	auto anim = _mngr->getComponent<ImageWithFrames>(e);

	draw(tr, anim->_tex, anim);
}

void RenderSystem::drawGhosts() {
	// draw ghosts
	for (auto g : _mngr->getEntities(ecs::grp::GHOSTS)) {

		auto tr = _mngr->getComponent<Transform>(g);
		auto tex = _mngr->getComponent<Image>(g)->_tex;
		auto anim = _mngr->getComponent<ImageWithFrames>(g);
		draw(tr, tex, anim);

	}
}

void RenderSystem::drawMsgs() {
	// draw the score
	//
	auto score = _mngr->getSystem<GameCtrlSystem>()->getScore();

	Texture scoreTex(sdlutils().renderer(), std::to_string(score),
			sdlutils().fonts().at("ARIAL24"), build_sdlcolor(0x444444ff));

	SDL_FRect dest = build_sdlfrect( //
			(sdlutils().width() - scoreTex.width()) / 2.0f, //
			10.0f, //
			scoreTex.width(), //
			scoreTex.height());

	scoreTex.render(dest);

	// draw add stars message
	sdlutils().msgs().at("addstars").render(10, 10);

}

void RenderSystem::draw(Transform *tr, const Texture *tex, ImageWithFrames* anim) {
	SDL_FRect dest = build_sdlfrect(tr->_pos, tr->_width, tr->_height);
	if (anim == nullptr) {
		// Imagen normal
		tex->render(dest, tr->_rot);
		return;
	}
	//assert(tex != nullptr);
	// Imagen animada
	SDL_FRect src{
		(float)((anim->_currentFrame % anim->_cols) * anim->_frameWidth),
		(float)((anim->_currentFrame / anim->_cols) * anim->_frameHeight),
		(float)anim->_frameWidth,
		(float)anim->_frameHeight
	};


	tex->render(src, dest, tr->_rot);

}
