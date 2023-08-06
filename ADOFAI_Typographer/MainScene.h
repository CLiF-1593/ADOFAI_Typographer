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
	MenuComponent* menu_comp;
	SettingComponent* setting_comp;
	ScreenComponent* screen_comp;
	MotionListComponent* motion_list_comp;
	TimelineComponent* timeline_comp;

	Scene* child_scene;

public:
	MainScene(UI_Window* win);
	~MainScene();

	void Rendering() override;
	void EventProcess(SDL_Event* evt) override;

	Scene* GetNextScene() override;
};

