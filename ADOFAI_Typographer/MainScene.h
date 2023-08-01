#pragma once
#include "Scene.h"
#include "UI_Window.h"
#include "MenuComponent.h"
#include "SettingComponent.h"
#include "ScreenComponent.h"
#include "MotionListComponent.h"
#include "TimelineComponent.h"

class MainScene : public Scene {
private:
	SDL_Texture* clif_logo;
	SDL_Texture* software_title;
	SDL_Texture* developed;	

	MenuComponent* menu_comp;
	SettingComponent* setting_comp;
	ScreenComponent* screen_comp;
	MotionListComponent* motion_list_comp;
	TimelineComponent* timeline_comp;

	const int intro_duration = 4000;
	int begin_time;

	Scene* child_scene;

public:
	MainScene(UI_Window* win);
	~MainScene();

	void Rendering() override;
	void EventProcess(SDL_Event* evt) override;

	Scene* GetNextScene() override;
};

