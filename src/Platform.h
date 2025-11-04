#ifndef PLATFORM_H
#define PLATFORM_H

#include "Crosser.h"
#include "Collision.h"

class Platform : public Crosser
{
public:
	using Crosser::Crosser;
	Collision checkCollision(const SDL_FRect& other) override;

};

#endif //PLATFORM_h
