#pragma once
#include "Component.h"

#define screen_comp_font_size 30

#define screen_comp_height (0.6 * (this->win->GetHeight() - menu_comp_height - 2) - screen_comp_margin * 2)
#define screen_comp_width (screen_comp_height * 16 / 9)

#define screen_comp_margin  10

#define screen_comp_x (setting_comp_margin * 2 + setting_comp_width)
#define screen_comp_y (menu_comp_height + 2 + screen_comp_margin)

class ScreenComponent : public Component {
private:

public:
	ScreenComponent(UI_Window* win);
	~ScreenComponent();

	void Rendering() override;
	void EventProcess(SDL_Event* evt) override;
};

