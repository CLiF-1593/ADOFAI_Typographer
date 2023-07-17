#pragma once
#include "Component.h"

#define motion_list_comp_font_size 30

#define motion_list_comp_height (0.6 * (this->win->GetHeight() - menu_comp_height - 2) - motion_list_comp_margin * 2)
#define motion_list_comp_width (300 - motion_list_comp_margin * 2)

#define motion_list_comp_margin 10

#define motion_list_comp_x (this->win->GetWidth() - motion_list_comp_margin - motion_list_comp_width)
#define motion_list_comp_y (menu_comp_height + 2 + motion_list_comp_margin)

class MotionListComponent : public Component {
private:
	SDL_Texture* title;

public:
	MotionListComponent(UI_Window* win);
	~MotionListComponent();

	void Rendering() override;
	void EventProcess(SDL_Event* evt) override;
};

