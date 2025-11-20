#include "Button.h"

void Button::handleEvent(const SDL_Event& event) {
	if(event.type == SDL_EVENT_MOUSE_MOTION)
		hovered = isMouseInside(rect, event.motion.x, event.motion.y);

	else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
		if (hovered && onClick)
			onClick();
	}
}
void Button::render() const {
	SDL_Color c = hovered ? hoverColor : color;
	SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
	
}