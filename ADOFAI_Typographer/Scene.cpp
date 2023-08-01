#include "Scene.h"
#include "Log.h"

Scene::Scene(UI_Window* win) {
	Log::Debug("Scene", "Constructor", " : Created");
	this->win = win;
	this->finish = false;
}

Scene::~Scene() {
	Log::Debug("Scene", "Destructor", " : Deleted");
}

bool Scene::IsFinished() {
	return this->finish;
}
