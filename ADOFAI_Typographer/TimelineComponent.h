#pragma once
#include "Component.h"
#include "MotionStatus.h"
#include "UI_Element.h"
#include "Color.h"
#include <vector>

#define timeline_comp_font_size 30

#define timeline_comp_height (0.4 * (this->win->GetHeight() - menu_comp_height - 2) - timeline_comp_margin)
#define timeline_comp_width (this->win->GetWidth() - 2 * timeline_comp_margin)

#define timeline_comp_margin 10

#define timeline_comp_x (timeline_comp_margin)
#define timeline_comp_y (menu_comp_height + 2 + setting_comp_height + setting_comp_margin * 2)

#define timeline_comp_menu_width 200

class TimelineBar;

class TimelineComponent : public Component {
private:
	SDL_Texture* title;

	double pseudo_current_frame;
	double begin_frame;
	double frame_size;

	int current_frame;

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
	bool time_table_dragging;

	int prev_x;

	TimelineBar* current_bar;

	void GetBarPosition(int& begin, int& end, Motion::Unit& bar, SDL_Rect& time_marking_line);

	friend class TimelineBar;

public:
	TimelineComponent(UI_Window* win);
	~TimelineComponent();

	void Rendering() override;
	void EventProcess(SDL_Event* evt) override;
};

class TimelineBar {
private:
	UI_Window* win;
	Motion::Unit* motion;

	bool CalculateBarArea(TimelineComponent* comp, SDL_Rect& bar_area, SDL_Rect& total_area, int offset);

	bool is_pressed;
	bool is_left_resizing;
	bool is_right_resizing;

	int level;

	int prev_x;
	double begin_frame;
	double end_frame;

	int start_pos;

public:

	TimelineBar(UI_Window* win, Motion::Unit* motion, int level);
	~TimelineBar();

	void Rendering(TimelineComponent* comp, SDL_Rect bar_area, SDL_Rect total_area, int offset, Color::RGB clr);
	bool IsClicked(TimelineComponent* comp, SDL_Event* evt, SDL_Rect bar_area, SDL_Rect total_area, int offset);
	bool SelectedBarEventProcess(TimelineComponent* comp, SDL_Event* evt, SDL_Rect bar_area, SDL_Rect total_area, int offset);

	void Press(int x);

	bool IsBarEventProcessing();
};