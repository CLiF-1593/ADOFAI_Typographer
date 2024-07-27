#pragma once
#include "Component.h"
#include "UI_Element.h"

#define setting_comp_font_size 30

#define setting_comp_height (0.6 * (this->win->GetHeight() - menu_comp_height - 2) - setting_comp_margin * 2)
#define setting_comp_width (this->win->GetWidth() - motion_list_comp_width - motion_list_comp_margin * 2 - screen_comp_width - setting_comp_margin * 2)

#define setting_comp_margin 10

#define setting_comp_x (setting_comp_margin)
#define setting_comp_y (menu_comp_height + 2 + setting_comp_margin)

#define setting_comp_minimum_width 200

class SettingComponent : public Component {
private:
	SDL_Texture* title;

	SDL_Texture* begin_frame_text;
	SDL_Texture* end_frame_text;
	SDL_Texture* imm_run_text;
	SDL_Texture* imm_frame_text;
	SDL_Texture* pos_x_text;
	SDL_Texture* pos_y_text;
	SDL_Texture* rot_deg_text;
	SDL_Texture* scale_x_text;
	SDL_Texture* scale_y_text;
	SDL_Texture* opacity_text;
	SDL_Texture* transition_text;

	SDL_Texture* warnning_text;

	int prev_width;

	NumberInput* frame_input_a, *frame_input_b;
	NumberInput* value_input_a, *value_input_b;
	Button* imm_switch_yes, *imm_switch_no;

public:
	SettingComponent(UI_Window* win);
	~SettingComponent();

	void Rendering() override;
	void EventProcess(SDL_Event* evt) override;
};

