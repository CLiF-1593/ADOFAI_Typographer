#include "MotionStatus.h"
#include "MotionListComponent.h"
#include "MenuComponent.h"
#include "Log.h"
#include "Texture.h"
#include "UI_Window.h"
using namespace std;

vector<Motion*> MotionStatus::motion_list;
int MotionStatus::current_motion_index = -1;

void MotionStatus::AddMotion() {
	Motion* motion = new Motion();
	motion_list.push_back(motion);
	current_motion_index = motion_list.size() - 1;
}

void MotionStatus::DeleteMotion() {
	if (motion_list.size() == 0 || current_motion_index == -1) return;
	delete motion_list[current_motion_index];
	motion_list[current_motion_index] = nullptr;
	motion_list.erase(motion_list.begin() + current_motion_index);
	current_motion_index--;
}

Motion* MotionStatus::GetMotion() {
	if (motion_list.size() == 0 || current_motion_index == -1) return nullptr;
	return motion_list[current_motion_index];
}

int MotionStatus::GetCurrentMotionIndex() {
	return current_motion_index;
}

std::vector<Motion*>* MotionStatus::GetMotionList() {
	return &motion_list;
}

void MotionStatus::SetCurrentMotionIndex(int index) {
	current_motion_index = index;
}

SDL_Texture* Motion::GetTexture(UI_Window* win) {
	if (this->name != this->prev_name) {
		if (this->texture != nullptr) SDL_DestroyTexture(this->texture);
		this->texture = Texture::LoadText(this->name.c_str(), win->GetRenderer(), motion_list_comp_font_size, "main", Color::RGB(CLR_SUB_TEXT));
		this->prev_name = this->name;
	}
	return this->texture;
}

Motion::Motion() {
	this->name = "New Motion";
	this->prev_name = "";
	this->texture = nullptr;
}

Motion::~Motion() {
	if (this->texture != nullptr) SDL_DestroyTexture(this->texture);
	this->texture = nullptr;
}
