#ifndef LABEL_H
#define LABEL_H

#include "GameObject.h"

class Label : public GameObject
{

protected:
	SDL_FRect rect;
	Texture* texture;

public:
	Label(Game* game, Texture* texture, float x=0,float y=0, float w=0, float h=0);
	~Label() = default;
	virtual void render() const override;
	virtual void update() override;

	void setPosition(float x, float y) { rect.x = x; rect.y = y; }
	void setSize(float w, float h) { rect.w = w; rect.h = h; }
	SDL_FRect getRect() const { return rect; }
	Texture* getTexture() const { return texture; }
	void setTexture(Texture* tex) { texture = tex; }
};

#endif // LABEL_H