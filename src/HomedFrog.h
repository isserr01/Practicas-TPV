#ifndef HOMED_FROG_H
#define HOMED_FROG_H

#include "game.h"
#include "vector2D.h"
#include "texture.h"
#include "Collision.h"
#include "SceneObject.h"
#include <istream>
class Game;
using Point2D = Vector2D <float>;

class HomedFrog : public SceneObject
{


public:
	//constructora
	HomedFrog(GameState* state,Point2D posicion);
	void render() const override;
	void update() override;
	virtual ~HomedFrog();

};
#endif //HOMED_FROG_H

