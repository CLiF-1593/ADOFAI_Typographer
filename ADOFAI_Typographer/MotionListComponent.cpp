#include "MotionListComponent.h"
#include "Texture.h"
#include "MenuComponent.h"
#include "SettingComponent.h"
#include "ScreenComponent.h"

MotionListComponent::MotionListComponent(UI_Window* win) :Component(win) {
	this->title = Texture::LoadText("Motion List", this->win->GetRenderer(), motion_list_comp_font_size, "main", Color::RGB(CLR_SUB_TEXT));
}

MotionListComponent::~MotionListComponent() {
	SDL_DestroyTexture(this->title);
}

void MotionListComponent::Rendering() {
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
	SDL_RenderFillRect(this->win->GetRenderer(), &rect);

	SDL_QueryTexture(this->title, NULL, NULL, &rect.w, NULL);
	rect.x += motion_list_comp_margin;
	SDL_RenderCopy(this->win->GetRenderer(), this->title, NULL, &rect);
}

void MotionListComponent::EventProcess(SDL_Event* evt) {
}
