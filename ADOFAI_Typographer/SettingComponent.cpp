#include "SettingComponent.h"
#include "Texture.h"
#include "MenuComponent.h"
#include "ScreenComponent.h"
#include "MotionListComponent.h"

SettingComponent::SettingComponent(UI_Window* win) : Component(win){
	this->title = Texture::LoadText("Setting", this->win->GetRenderer(), setting_comp_font_size, "main", Color::RGB(CLR_SUB_TEXT));
}

SettingComponent::~SettingComponent() {
	SDL_DestroyTexture(this->title);
}

void SettingComponent::Rendering() {
	SDL_Rect rect;
	rect.x = setting_comp_x;
	rect.y = setting_comp_y;
	rect.w = setting_comp_width;
	rect.h = setting_comp_height;

	SDL_SetRenderDrawColor(this->win->GetRenderer(), CLR_COMP_BACKGROUND);
	SDL_RenderFillRect(this->win->GetRenderer(), &rect);
	SDL_SetRenderDrawColor(this->win->GetRenderer(), CLR_STD_BORDER);
	SDL_RenderDrawRect(this->win->GetRenderer(), &rect);

	SDL_QueryTexture(this->title, NULL, NULL, NULL, &rect.h);
	SDL_RenderFillRect(this->win->GetRenderer(), &rect);

	SDL_QueryTexture(this->title, NULL, NULL, &rect.w, NULL);
	rect.x += setting_comp_margin;
	SDL_RenderCopy(this->win->GetRenderer(), this->title, NULL, &rect);
}

void SettingComponent::EventProcess(SDL_Event* evt) {

}
