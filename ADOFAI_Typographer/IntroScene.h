#pragma once
#include "Scene.h"
#include "UI_Window.h"

class IntroScene : public Scene {
private:
	SDL_Texture* clif_logo;
	SDL_Texture* software_title;
	SDL_Texture* developed;

	const int intro_duration = 4000;
	int begin_time;

public:
	IntroScene(UI_Window *win);
	~IntroScene();

	void Rendering() override;
	void EventProcess(SDL_Event* evt) override;

	Scene* GetNextScene() override;
};

