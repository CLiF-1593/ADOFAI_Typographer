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

Motion* MotionStatus::CurrentMotion() {
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

	// SAMPLE
	/*Position pos;
	pos.x = 100;
	pos.y = 100;
	pos.begin_frame = 0;
	pos.end_frame = 30;
	pos.transition = TransitionType::LINEAR;
	this->position.push_back(pos);

	Rotation rot;
	rot.degree = 0;
	rot.begin_frame = 20;
	rot.end_frame = 50;
	rot.transition = TransitionType::LINEAR;
	this->rotation.push_back(rot);

	Scale scale;
	scale.x = 100;
	scale.y = 100;
	scale.begin_frame = 40;
	scale.end_frame = 80;
	scale.transition = TransitionType::LINEAR;
	this->scale.push_back(scale);

	Opacity opacity;
	opacity.opacity = 100;
	opacity.begin_frame = 10;
	opacity.end_frame = 40;
	opacity.transition = TransitionType::LINEAR;
	this->opacity.push_back(opacity);*/
}

Motion::~Motion() {
	if (this->texture != nullptr) SDL_DestroyTexture(this->texture);
	this->texture = nullptr;
}

bool Motion::IsAbleToAddPosition(int cur_frame) {
	for (int i = 0; i < this->position.size(); i++) {
		if (this->position[i].begin_frame <= cur_frame && cur_frame < this->position[i].end_frame) 
			return false;
	}
	return true;
}

bool Motion::IsAbleToAddRotation(int cur_frame) {
	for (int i = 0; i < this->rotation.size(); i++) {
		if (this->rotation[i].begin_frame <= cur_frame && cur_frame < this->rotation[i].end_frame)
			return false;
	}
	return true;
}

bool Motion::IsAbleToAddScale(int cur_frame) {
	for (int i = 0; i < this->scale.size(); i++) {
		if (this->scale[i].begin_frame <= cur_frame && cur_frame < this->scale[i].end_frame)
			return false;
	}
	return true;
}

bool Motion::IsAbleToAddOpacity(int cur_frame) {
	for (int i = 0; i < this->opacity.size(); i++) {
		if (this->opacity[i].begin_frame <= cur_frame && cur_frame < this->opacity[i].end_frame)
			return false;
	}
	return true;
}

void Motion::AddPosition(int cur_frame, double frame_size) {
	Position pos;
	pos.x = 0;
	pos.y = 0;
	pos.begin_frame = cur_frame;
	pos.end_frame = cur_frame + 200 / frame_size;
	pos.transition = TransitionType::LINEAR;

	int front_frame, back_frame;
	this->GetFrontBackFrame(cur_frame, front_frame, back_frame, 0);

	if (back_frame < pos.end_frame) {
		pos.begin_frame -= (pos.end_frame - back_frame);
		pos.end_frame = back_frame;
	}
	if (this->IsOverlaped(&pos, 0)) {
		this->GetFrontBackFrame(cur_frame + 1, front_frame, back_frame, 0);
		pos.begin_frame = front_frame;
	}

	this->position.push_back(pos);
}

void Motion::AddRotation(int cur_frame, double frame_size) {
	Rotation rot;
	rot.degree = 0;
	rot.begin_frame = cur_frame;
	rot.end_frame = cur_frame + 200 / frame_size;
	rot.transition = TransitionType::LINEAR;

	int front_frame, back_frame;
	this->GetFrontBackFrame(cur_frame, front_frame, back_frame, 1);

	if (back_frame < rot.end_frame) {
		rot.begin_frame -= (rot.end_frame - back_frame);
		rot.end_frame = back_frame;
	}
	if (this->IsOverlaped(&rot, 1)) {
		this->GetFrontBackFrame(cur_frame + 1, front_frame, back_frame, 1);
		rot.begin_frame = front_frame;
	}

	this->rotation.push_back(rot);
}

void Motion::AddScale(int cur_frame, double frame_size) {
	Scale scale;
	scale.x = 100;
	scale.y = 100;
	scale.begin_frame = cur_frame;
	scale.end_frame = cur_frame + 200 / frame_size;
	scale.transition = TransitionType::LINEAR;

	int front_frame, back_frame;
	this->GetFrontBackFrame(cur_frame, front_frame, back_frame, 2);

	if (back_frame < scale.end_frame) {
		scale.begin_frame -= (scale.end_frame - back_frame);
		scale.end_frame = back_frame;
	}
	if (this->IsOverlaped(&scale, 2)) {
		this->GetFrontBackFrame(cur_frame + 1, front_frame, back_frame, 2);
		scale.begin_frame = front_frame;
	}

	this->scale.push_back(scale);
}

void Motion::AddOpacity(int cur_frame, double frame_size) {
	Opacity opacity;
	opacity.opacity = 100;
	opacity.begin_frame = cur_frame;
	opacity.end_frame = cur_frame + 200 / frame_size;
	opacity.transition = TransitionType::LINEAR;

	int front_frame, back_frame;
	this->GetFrontBackFrame(cur_frame, front_frame, back_frame, 3);

	if (back_frame < opacity.end_frame) {
		opacity.begin_frame -= (opacity.end_frame - back_frame);
		opacity.end_frame = back_frame;
	}
	if (this->IsOverlaped(&opacity, 3)) {
		this->GetFrontBackFrame(cur_frame + 1, front_frame, back_frame, 3);
		opacity.begin_frame = front_frame;
	}

	this->opacity.push_back(opacity);
}

void Motion::GetFrontBackFrame(int cur, int& front, int& back, int level) {
	front = 0;
	back = 2147483647;

	switch (level) {
	case 0:
		for (int i = 0; i < this->position.size(); i++) {
			int begin = this->position[i].begin_frame;
			int end = this->position[i].end_frame;
			if (end < cur && (cur - end) < (cur - front)) {
				front = end;
			}
			if (begin > cur && (begin - cur) < (back - cur)) {
				back = begin;
			}
		}
		break;
	case 1:
		for (int i = 0; i < this->rotation.size(); i++) {
			int begin = this->rotation[i].begin_frame;
			int end = this->rotation[i].end_frame;
			if (end < cur && (cur - end) < (cur - front)) {
				front = end;
			}
			if (begin > cur && (begin - cur) < (back - cur)) {
				back = begin;
			}
		}
		break;
	case 2:
		for (int i = 0; i < this->scale.size(); i++) {
			int begin = this->scale[i].begin_frame;
			int end = this->scale[i].end_frame;
			if (end < cur && (cur - end) < (cur - front)) {
				front = end;
			}
			if (begin > cur && (begin - cur) < (back - cur)) {
				back = begin;
			}
		}
		break;
	case 3:
		for (int i = 0; i < this->opacity.size(); i++) {
			int begin = this->opacity[i].begin_frame;
			int end = this->opacity[i].end_frame;
			if (end < cur && (cur - end) < (cur - front)) {
				front = end;
			}
			if (begin > cur && (begin - cur) < (back - cur)) {
				back = begin;
			}
		}
		break;
	}
}

bool Motion::IsOverlaped(Motion::Unit* motion, int level) {
	switch (level) {
	case 0:
		for (int i = 0; i < this->position.size(); i++) {
			if (motion != &this->position[i]) {
				if (motion->begin_frame >= this->position[i].end_frame || motion->end_frame <= this->position[i].begin_frame)
					continue;
				else return true;
			}
		}
		break;
	case 1:
		for (int i = 0; i < this->rotation.size(); i++) {
			if (motion != &this->rotation[i]) {
				if (motion->begin_frame >= this->rotation[i].end_frame || motion->end_frame <= this->rotation[i].begin_frame)
					continue;
				else return true;
			}
		}
		break;
	case 2:
		for (int i = 0; i < this->scale.size(); i++) {
			if (motion != &this->scale[i]) {
				if (motion->begin_frame >= this->scale[i].end_frame || motion->end_frame <= this->scale[i].begin_frame)
					continue;
				else return true;
			}
		}
		break;
	case 3:
		for (int i = 0; i < this->opacity.size(); i++) {
			if (motion != &this->opacity[i]) {
				if (motion->begin_frame >= this->opacity[i].end_frame || motion->end_frame <= this->opacity[i].begin_frame)
					continue;
				else return true;
			}
		}
		break;
	}
	return false;
}
