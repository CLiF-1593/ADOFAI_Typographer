#include "MainScene.h"
#include "Color.h"
#include "Status.h"

MainScene::MainScene(UI_Window* win) : Scene(win) {
	this->menu_comp = new MenuComponent(this->win);
	this->setting_comp = new SettingComponent(this->win);
	this->screen_comp = new ScreenComponent(this->win);
	this->motion_list_comp = new MotionListComponent(this->win);
	this->timeline_comp = new TimelineComponent(this->win);
	Status::GetEvent();
	Status::RenderOnlyEventOccured();
}

MainScene::~MainScene() {
	delete this->menu_comp, this->setting_comp , this->screen_comp, this->motion_list_comp, this->timeline_comp;
}

void MainScene::Rendering() {
	this->setting_comp->Rendering();
	this->screen_comp->Rendering();
	this->motion_list_comp->Rendering();
	this->timeline_comp->Rendering();
	this->menu_comp->Rendering();
}

void MainScene::EventProcess(SDL_Event* evt) {
	this->menu_comp->EventProcess(evt);
	this->setting_comp->EventProcess(evt);
	this->screen_comp->EventProcess(evt);
	this->motion_list_comp->EventProcess(evt);
	this->timeline_comp->EventProcess(evt);
}

Scene* MainScene::GetNextScene() {
	return nullptr;
}
