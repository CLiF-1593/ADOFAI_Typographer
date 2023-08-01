#include "MainScene.h"
#include "Color.h"
#include "Status.h"

MainScene::MainScene(UI_Window* win) : Scene(win) {
	Log::Debug("MainScene", "Constructor", " : Created");
	this->menu_comp = new MenuComponent(this->win);
	this->setting_comp = new SettingComponent(this->win);
	this->screen_comp = new ScreenComponent(this->win);
	this->motion_list_comp = new MotionListComponent(this->win);
	this->timeline_comp = new TimelineComponent(this->win);

	Status::GetEvent();
	Status::RenderOnlyEventOccured();
	Status::EconomyRender();

	this->child_scene = nullptr;
	
	this->menu_comp->AllocateChildScene(&(this->child_scene));
	this->motion_list_comp->AllocateChildScene(&(this->child_scene));
}

MainScene::~MainScene() {
	Log::Debug("MainScene", "Destructor", " : Deleted");
	if(this->child_scene) delete this->child_scene;
	delete this->menu_comp, this->setting_comp , this->screen_comp, this->motion_list_comp, this->timeline_comp;
}

void MainScene::Rendering() {
	this->setting_comp->Rendering();
	this->screen_comp->Rendering();
	this->motion_list_comp->Rendering();
	this->timeline_comp->Rendering();
	this->menu_comp->Rendering();

	if(this->child_scene)
		this->child_scene->Rendering();
}

void MainScene::EventProcess(SDL_Event* evt) {
	if (this->child_scene) {
		this->child_scene->EventProcess(evt);
		if (this->child_scene->IsFinished()) {
			delete this->child_scene;
			this->child_scene = nullptr;
		}
	}
	if (!this->child_scene) {
		this->menu_comp->EventProcess(evt);
		this->setting_comp->EventProcess(evt);
		this->screen_comp->EventProcess(evt);
		this->motion_list_comp->EventProcess(evt);
		this->timeline_comp->EventProcess(evt);
	}
}

Scene* MainScene::GetNextScene() {
	return nullptr;
}
