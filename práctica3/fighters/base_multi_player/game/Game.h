// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

#include <vector>


class Fighter;
class Bullets;

class Game {
public:
	Game();
	virtual ~Game();
	void init();
	void start();
private:

	void check_collisions();


	Bullets *_bm;
	Fighter *_fighters;

};

