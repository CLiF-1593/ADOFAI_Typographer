#include "TimelineComponent.h"
#include "Texture.h"
#include "MenuComponent.h"
#include "SettingComponent.h"

TimelineComponent::TimelineComponent(UI_Window* win) : Component(win) {
	this->title = Texture::LoadText("Timeline", this->win->GetRenderer(), timeline_comp_font_size, "main", Color::RGB(CLR_SUB_TEXT));
}

TimelineComponent::~TimelineComponent() {
	SDL_DestroyTexture(this->title);
}

void TimelineComponent::Rendering() {
	SDL_Rect rect;
	rect.x = timeline_comp_x;
	rect.y = timeline_comp_y;
	rect.w = timeline_comp_width;
	rect.h = timeline_comp_height;

	SDL_SetRenderDrawColor(this->win->GetRenderer(), CLR_COMP_BACKGROUND);
	SDL_RenderFillRect(this->win->GetRenderer(), &rect);
	SDL_SetRenderDrawColor(this->win->GetRenderer(), CLR_STD_BORDER);
	SDL_RenderDrawRect(this->win->GetRenderer(), &rect);

	SDL_QueryTexture(this->title, NULL, NULL, NULL, &rect.h);
	SDL_RenderFillRect(this->win->GetRenderer(), &rect);

	SDL_QueryTexture(this->title, NULL, NULL, &rect.w, NULL);
	rect.x += timeline_comp_margin;
	SDL_RenderCopy(this->win->GetRenderer(), this->title, NULL, &rect);
}

void TimelineComponent::EventProcess(SDL_Event* evt) {
}
