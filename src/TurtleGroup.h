#ifndef TURTLEGROUP_H
#define TURTLEGROUP_H
#include "Platform.h"
#include <istream>

class TurtleGroup : public Platform
{
	int numTurtles;
	int frameNumber;
	bool isSubmersible;
	bool submerged;
	float submergeTimer;
	float submergePeriod;

public:
	TurtleGroup(Game* game, std::istream& in);
	void update() override;
	Collision checkCollision(const SDL_FRect& other) override;
};

#endif // TURTLEGROUP_H