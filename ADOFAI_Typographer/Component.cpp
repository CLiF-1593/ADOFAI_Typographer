#include "Component.h"

void Component::SetChildScene(Scene* scene) {
	if (this->scene_ptr && *(this->scene_ptr) == nullptr) {
		*(this->scene_ptr) = scene;
	}
	else if (scene) {
		delete scene;
	}
}

Component::Component(UI_Window* win) {
	this->win = win;
}

Component::~Component() {
}

void Component::AllocateChildScene(Scene** scene_ptr) {
	this->scene_ptr = scene_ptr;
}
