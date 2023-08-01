#pragma once
#include "Component.h"
#include "Button.h"
#include <vector>

#define timeline_comp_font_size 30

#define timeline_comp_height (0.4 * (this->win->GetHeight() - menu_comp_height - 2) - timeline_comp_margin)
#define timeline_comp_width (this->win->GetWidth() - 2 * timeline_comp_margin)

#define timeline_comp_margin 10

#define timeline_comp_x (timeline_comp_margin)
#define timeline_comp_y (menu_comp_height + 2 + setting_comp_height + setting_comp_margin * 2)

#define timeline_comp_menu_width 200

class TimelineComponent : public Component {
private:
	SDL_Texture* title;

	double current_frame;
	double begin_frame;
	double frame_size;

	SDL_Texture* position_text;
	SDL_Texture* rotation_text;
	SDL_Texture* scale_text;
	SDL_Texture* opacity_text;

	Button* position_add_button;
	Button* rotation_add_button;
	Button* scale_add_button;
	Button* opacity_add_button;

	int GetBigFrameUnit();

	vector<SDL_Texture*> number_table;
	void DrawNumber(int x, int y, int number);

	bool time_marker_dragging;

	int prev_x;

public:
	TimelineComponent(UI_Window* win);
	~TimelineComponent();

	void Rendering() override;
	void EventProcess(SDL_Event* evt) override;
};

