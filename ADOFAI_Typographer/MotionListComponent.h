#pragma once
#include "Component.h"
#include "UI_Element.h"

#define motion_list_comp_font_size 30

#define motion_list_comp_height (0.6 * (win->GetHeight() - menu_comp_height - 2) - motion_list_comp_margin * 2)
#define motion_list_comp_width (300 - motion_list_comp_margin * 2)

#define motion_list_comp_margin 10

#define motion_list_comp_x (win->GetWidth() - motion_list_comp_margin - motion_list_comp_width)
#define motion_list_comp_y (menu_comp_height + 2 + motion_list_comp_margin)

class MotionListComponent : public Component {
private:
	SDL_Texture* title;
	int offset;

	Button* btn_add;
	Button* btn_delete;
	Button* btn_rename;

	Button* btn_append;


public:
	MotionListComponent(UI_Window* win);
	~MotionListComponent();

	void Rendering() override;
	void EventProcess(SDL_Event* evt) override;
};

