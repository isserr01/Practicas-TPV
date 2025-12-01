#ifndef LABEL_H
#define LABEL_H

#include "GameObject.h"
#include "texture.h"

class Label : public GameObject
{

protected:
	SDL_Rect rect;
	Texture* texture;

public:
	Label(GameState* gameStat, Texture* texture, SDL_Rect rect);
	virtual void render() const override;

};

#endif // LABEL_H