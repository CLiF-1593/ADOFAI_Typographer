#include "Scene.h"

Scene::Scene(UI_Window* win) {
	this->win = win;
	this->finish = false;
}

bool Scene::IsFinished() {
	return this->finish;
}
