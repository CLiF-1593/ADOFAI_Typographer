#pragma once
#include "UI_Window.h"
class Scene {
protected:
	UI_Window* win;
	bool finish;

public:
	Scene(UI_Window* win);
	virtual ~Scene();

	virtual void Rendering() abstract;
	virtual void EventProcess(SDL_Event* evt) abstract;

	bool IsFinished();
	virtual Scene* GetNextScene() abstract;
};

