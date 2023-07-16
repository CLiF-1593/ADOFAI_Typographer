#include "MainScene.h"
#include "Color.h"
#include "Status.h"

MainScene::MainScene(UI_Window* win) : Scene(win) {
	this->menu_comp = new MenuComponent(this->win);
	Status::GetEvent();
	Status::RenderOnlyEventOccured();
}

MainScene::~MainScene() {
	delete this->menu_comp;
}

void MainScene::Rendering() {
	this->menu_comp->Rendering();
}

void MainScene::EventProcess(SDL_Event* evt) {
	this->menu_comp->EventProcess(evt);
}

Scene* MainScene::GetNextScene() {
	return nullptr;
}
