#ifndef LABEL_H
#define LABEL_H

#include "GameObject.h"
#include "texture.h"

class Label : public GameObject
{

protected:
	float x, y;
	Texture* texture;

public:
	Label(GameState* gameStat, Texture* texture, float x, float y);
	virtual ~Label() = default ;
	void render() const override;
	virtual void update() override;
};

#endif // LABEL_H