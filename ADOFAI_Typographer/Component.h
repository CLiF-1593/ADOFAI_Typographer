#pragma once
#include <SDL.h>
#include "UI_Window.h"
class Component {
protected:
	UI_Window* win;

public:
	Component(UI_Window* win);
	virtual void Rendering() abstract;
	virtual void EventProcess(SDL_Event* evt) abstract;
};

