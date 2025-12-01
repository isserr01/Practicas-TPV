#ifndef BUTTON_H
#define BUTTON_H
#include "Label.h"
#include "EventHandler.h"
#include <functional>
class Button : public Label, public EventHandler
{
public: 
	using Callback = std::function<void()>;

private:
	Callback callback;
	bool mouseOver() const;

public:

	Button(GameState* gameStat, Texture* texture, float x, float y);
	
	void connect(Callback&& func);
	void handleEvent(const SDL_Event& event) override;
	void update() override;
	void render() const override;
	
	
};

#endif // BUTTON_H
