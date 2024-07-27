#include "MotionListComponent.h"
#include "Texture.h"
#include "MenuComponent.h"
#include "SettingComponent.h"
#include "ScreenComponent.h"
#include "MotionStatus.h"
#include "SoftwareStatus.h"
#include "TextInputScene.h"

MotionListComponent::MotionListComponent(UI_Window* win) :Component(win) {
	this->title = Texture::LoadText("Motion List", this->win->GetRenderer(), motion_list_comp_font_size, "main", Color::RGB(CLR_SUB_TEXT));
	this->offset = 0;

	int y = 0;
	SDL_QueryTexture(this->title, NULL, NULL, NULL, &y);
	y += motion_list_comp_font_size + motion_list_comp_y;

	this->btn_add = new Button("Add", this->win->GetRenderer(), motion_list_comp_font_size, 0, 0);
	this->btn_delete = new Button("Delete", this->win->GetRenderer(), motion_list_comp_font_size, 0, 0);
	this->btn_rename = new Button("Rename", this->win->GetRenderer(), motion_list_comp_font_size, 0, 0);

	this->btn_append = new Button("Append", this->win->GetRenderer(), motion_list_comp_font_size, 0, 0);

	SDL_Rect rect;
	rect.x = motion_list_comp_x;
	rect.y = motion_list_comp_y;
	rect.w = motion_list_comp_width;
	rect.h = 0;
	SDL_QueryTexture(this->title, NULL, NULL, &rect.w, &rect.h);
	rect.x += motion_list_comp_margin;

	this->btn_add->SetPosition(rect.x, rect.y += rect.h);
	this->btn_delete->SetPosition(rect.x += this->btn_add->GetRect().w + motion_list_comp_margin, rect.y);
	this->btn_rename->SetPosition(rect.x += this->btn_delete->GetRect().w + motion_list_comp_margin, rect.y);

	this->btn_append->SetPosition(rect.x, rect.y += rect.h);
}

MotionListComponent::~MotionListComponent() {
	SDL_DestroyTexture(this->title); 
	delete this->btn_add, this->btn_delete, this->btn_rename;
}

void MotionListComponent::Rendering() {
	if (MotionStatus::CurrentMotion()) {
		this->btn_delete->SetEnabled(true);
		this->btn_rename->SetEnabled(true);
		this->btn_append->SetEnabled(true);
	}
	else {
		this->btn_delete->SetEnabled(false);
		this->btn_rename->SetEnabled(false);
		this->btn_append->SetEnabled(false);
	}

	SDL_Rect rect;
	rect.x = motion_list_comp_x;
	rect.y = motion_list_comp_y;
	rect.w = motion_list_comp_width;
	rect.h = motion_list_comp_height;

	SDL_SetRenderDrawColor(this->win->GetRenderer(), CLR_COMP_BACKGROUND);
	SDL_RenderFillRect(this->win->GetRenderer(), &rect);
	SDL_SetRenderDrawColor(this->win->GetRenderer(), CLR_STD_BORDER);
	SDL_RenderDrawRect(this->win->GetRenderer(), &rect);

	SDL_QueryTexture(this->title, NULL, NULL, NULL, &rect.h);
	rect.h *= 2;
	SDL_RenderFillRect(this->win->GetRenderer(), &rect);

	rect.h /= 2;
	SDL_QueryTexture(this->title, NULL, NULL, &rect.w, NULL);
	rect.x += motion_list_comp_margin;
	SDL_RenderCopy(this->win->GetRenderer(), this->title, NULL, &rect);

	if (SoftwareStatus::software_mode == SoftwareStatus::TYPO_EDITOR) {
		this->btn_append->SetPosition(rect.x, rect.y += rect.h);
		this->btn_append->Render();
	}
	else {
		this->btn_add->SetPosition(rect.x, rect.y += rect.h);
		this->btn_delete->SetPosition(rect.x += this->btn_add->GetRect().w + motion_list_comp_margin, rect.y);
		this->btn_rename->SetPosition(rect.x += this->btn_delete->GetRect().w + motion_list_comp_margin, rect.y);
		this->btn_add->Render();
		this->btn_delete->Render();
		this->btn_rename->Render();
	}

	SDL_Rect scroll_rect;
	scroll_rect.x = motion_list_comp_x + motion_list_comp_width - 5;
	scroll_rect.y = rect.y + rect.h;
	scroll_rect.w = 5;
	scroll_rect.h = motion_list_comp_height - (scroll_rect.y - motion_list_comp_y);

	rect.x = motion_list_comp_x + motion_list_comp_margin;
	rect.y += motion_list_comp_margin;

	for (int i = this->offset; i < MotionStatus::GetMotionList()->size(); i++) {
		Motion* motion = MotionStatus::GetMotionList()->at(i);
		rect.y += rect.h;
		if (i == MotionStatus::GetCurrentMotionIndex()) {
			rect.w = motion_list_comp_width;
			rect.x -= motion_list_comp_margin;
			SDL_QueryTexture(motion->GetTexture(this->win), NULL, NULL, NULL, &rect.h);
			SDL_SetRenderDrawColor(this->win->GetRenderer(), 255, 255, 255, 10);
			SDL_RenderFillRect(this->win->GetRenderer(), &rect);
			SDL_SetRenderDrawColor(this->win->GetRenderer(), CLR_STD_BORDER);
			SDL_RenderDrawRect(this->win->GetRenderer(), &rect);
			rect.x += motion_list_comp_margin;
		}
		if (rect.y + motion_list_comp_margin > motion_list_comp_y + motion_list_comp_height) break;
		SDL_QueryTexture(motion->GetTexture(this->win), NULL, NULL, &rect.w, &rect.h);
		SDL_RenderCopy(this->win->GetRenderer(), motion->GetTexture(this->win), NULL, &rect);
	}

	int container_capacity = MotionStatus::GetMotionList()->size();
	if (container_capacity) {
		SDL_SetRenderDrawColor(this->win->GetRenderer(), CLR_STD_BORDER);
		SDL_RenderDrawRect(this->win->GetRenderer(), &scroll_rect);

		double height = ((double)scroll_rect.h - 16.0) / (double)container_capacity;
		scroll_rect.h = height + 16.0;
		scroll_rect.y += (height * this->offset);

		SDL_SetRenderDrawColor(this->win->GetRenderer(), CLR_COMP_BACKGROUND_BRIGHT);
		SDL_RenderFillRect(this->win->GetRenderer(), &scroll_rect);
	}
}

void MotionListComponent::EventProcess(SDL_Event* evt) {
	int mouse_x, mouse_y;
	SDL_GetMouseState(&mouse_x, &mouse_y);
	if (mouse_x >= motion_list_comp_x && mouse_x <= motion_list_comp_x + motion_list_comp_width) {
		if (mouse_y >= motion_list_comp_y && mouse_y <= motion_list_comp_y + motion_list_comp_height) {
			if (evt->type == SDL_MOUSEWHEEL) {
				this->offset -= evt->wheel.y;
				if (this->offset >= (int)(MotionStatus::GetMotionList()->size())) this->offset = (int)(MotionStatus::GetMotionList()->size()) - 1;
				if (this->offset < 0) this->offset = 0;
			}
		}
	}

	if (SoftwareStatus::software_mode == SoftwareStatus::TYPO_EDITOR) {
		this->btn_append->EventProcess(evt);
	}
	else {
		this->btn_add->EventProcess(evt);
		this->btn_delete->EventProcess(evt);
		this->btn_rename->EventProcess(evt);

		if (this->btn_add->IsClicked()) {
			MotionStatus::AddMotion();
			this->SetChildScene(new TextInputScene(this->win, "Add Motion", "Motion Name", &(MotionStatus::CurrentMotion()->name)));
		}
		else if (this->btn_delete->IsClicked()) {
			MotionStatus::DeleteMotion();
		}
		else if (this->btn_rename->IsClicked()) {
			this->SetChildScene(new TextInputScene(this->win, "Rename Motion", "Motion Name", &(MotionStatus::CurrentMotion()->name)));
		}
	}

	SDL_Rect rect;
	rect.x = motion_list_comp_x;
	rect.y = motion_list_comp_y + motion_list_comp_margin;
	rect.w = motion_list_comp_width;
	SDL_QueryTexture(this->title, NULL, NULL, NULL, &rect.h);
	rect.y += rect.h;
	
	for (int i = this->offset; i < MotionStatus::GetMotionList()->size(); i++) {
		Motion* motion = MotionStatus::GetMotionList()->at(i);
		rect.y += rect.h;
		if (rect.y + motion_list_comp_margin > motion_list_comp_y + motion_list_comp_height) break;
		SDL_QueryTexture(motion->GetTexture(this->win), NULL, NULL, NULL, &rect.h);

		if (evt->type == SDL_MOUSEBUTTONUP && evt->button.button == SDL_BUTTON_LEFT) {
			if (evt->button.x >= rect.x && evt->button.x <= rect.x + rect.w) {
				if (evt->button.y >= rect.y && evt->button.y <= rect.y + rect.h) {
					MotionStatus::SetCurrentMotionIndex(i);
					break;
				}
			}
		}
	}
}
