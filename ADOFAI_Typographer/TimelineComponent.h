#pragma once
#include "Component.h"

#define timeline_comp_font_size 30

#define timeline_comp_height (0.4 * (this->win->GetHeight() - menu_comp_height - 2) - timeline_comp_margin)
#define timeline_comp_width (this->win->GetWidth() - 2 * timeline_comp_margin)

#define timeline_comp_margin 10

#define timeline_comp_x (timeline_comp_margin)
#define timeline_comp_y (menu_comp_height + 2 + setting_comp_height + setting_comp_margin * 2)

class TimelineComponent : public Component {
private:
	SDL_Texture* title;

public:
	TimelineComponent(UI_Window* win);
	~TimelineComponent();

	void Rendering() override;
	void EventProcess(SDL_Event* evt) override;
};

