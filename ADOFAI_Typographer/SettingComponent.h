#pragma once
#include "Component.h"

#define setting_comp_font_size 30

#define setting_comp_height (0.6 * (this->win->GetHeight() - menu_comp_height - 2) - setting_comp_margin * 2)
#define setting_comp_width (this->win->GetWidth() - motion_list_comp_width - motion_list_comp_margin * 2 - screen_comp_width - setting_comp_margin * 2)

#define setting_comp_margin 10

#define setting_comp_x (setting_comp_margin)
#define setting_comp_y (menu_comp_height + 2 + setting_comp_margin)

class SettingComponent : public Component {
private:
	SDL_Texture* title;

public:
	SettingComponent(UI_Window* win);
	~SettingComponent();

	void Rendering() override;
	void EventProcess(SDL_Event* evt) override;

};

