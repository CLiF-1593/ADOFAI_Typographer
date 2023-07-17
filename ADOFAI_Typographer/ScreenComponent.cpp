#include "ScreenComponent.h"
#include "Texture.h"
#include "MenuComponent.h"
#include "SettingComponent.h"
#include "MotionListComponent.h"

ScreenComponent::ScreenComponent(UI_Window* win) : Component(win) {
}

ScreenComponent::~ScreenComponent() {
}

void ScreenComponent::Rendering() {
	SDL_Rect rect;
	rect.x = screen_comp_x;
	rect.y = screen_comp_y;
	rect.w = screen_comp_width;
	rect.h = screen_comp_height;

	SDL_SetRenderDrawColor(this->win->GetRenderer(), CLR_SCREEN_BACKGROUND);
	SDL_RenderFillRect(this->win->GetRenderer(), &rect);
	SDL_SetRenderDrawColor(this->win->GetRenderer(), CLR_STD_BORDER);
	SDL_RenderDrawRect(this->win->GetRenderer(), &rect);
}

void ScreenComponent::EventProcess(SDL_Event* evt) {
}
