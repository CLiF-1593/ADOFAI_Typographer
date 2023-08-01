#pragma once
#include <SDL.h>
#include "UI_Window.h"
#include "Log.h"
#include "Scene.h"

class Component {
private:
	Scene** scene_ptr;

protected:
	UI_Window* win;

	void SetChildScene(Scene* scene);

public:
	Component(UI_Window* win);
	virtual ~Component();

	virtual void Rendering() abstract;
	virtual void EventProcess(SDL_Event* evt) abstract;

	void AllocateChildScene(Scene** scene_ptr);
};

