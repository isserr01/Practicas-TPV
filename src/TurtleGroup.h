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
	int frameDuration;

public:
	TurtleGroup(GameState* state, std::istream& in);
	void update() override;
	Collision checkCollision(const SDL_FRect& other) override;
	void render() const override;
};

#endif // TURTLEGROUP_H