#pragma once
#include "Label.h"
#include "EventHandler.h"
#include <functional>
class Button : public Label, public EventHandler
{
public: 
	using Callback = std::function<void()>;
private:
	SDL_FRect rect;
	Callback onClick;
	bool clicked = false;
	public:
	Button(GameState* gameStat, Texture* texture, SDL_FRect rect)
		: Label(gameStat, texture, rect) 
	{
		gameStat->addEventListener(this);
	}
	
	// Método para manejar eventos de clic
	bool isClicked(const SDL_Event& event) const {
		if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
			int x = event.button.x;
			int y = event.button.y;
			SDL_FRect box = getBoundingBox();
			if (x >= box.x && x <= box.x + box.w &&
				y >= box.y && y <= box.y + box.h) {
				return true;
			}
		}
		return false;
	}
	void handleEvent(const SDL_Event& event) override {
		if (isClicked(event)) {
			clicked = true;
			if (onClick) {
				onClick();
			}
		}
	}
};

