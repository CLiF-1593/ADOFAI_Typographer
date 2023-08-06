#include "TimelineComponent.h"
#include "Texture.h"
#include "MenuComponent.h"
#include "SettingComponent.h"
#include "SoftwareStatus.h"
#include "Status.h"
#include "MotionStatus.h"

int TimelineComponent::GetBigFrameUnit() {
	int ret;
	for (ret = 1; ret * this->frame_size < 50; ret *= 10);
	return ret;
}

void TimelineComponent::DrawNumber(int x, int y, int number) {
	SDL_Rect r;
	r.x = x;
	r.y = y;
	vector<int> digits;
	while (number > 0) {
		digits.push_back(number % 10);
		number /= 10;
	}
	if (digits.empty()) digits.push_back(0);
	for (int i = digits.size() - 1; i >= 0; i--) {
		SDL_QueryTexture(this->number_table[digits[i]], NULL, NULL, &r.w, &r.h);
		if(r.x + r.w >= timeline_comp_width + timeline_comp_x) break;
		SDL_RenderCopy(this->win->GetRenderer(), this->number_table[digits[i]], NULL, &r);
		r.x += r.w;
	}
}

void TimelineComponent::GetBarPosition(int& begin, int& end, Motion::Unit& bar, SDL_Rect& time_marking_line) {
	begin = bar.begin_frame * this->frame_size - this->begin_frame * this->frame_size + time_marking_line.x;
	end = bar.end_frame * this->frame_size - this->begin_frame * this->frame_size + time_marking_line.x;
	if (begin > time_marking_line.x + time_marking_line.w || end < time_marking_line.x) return;
	if (begin < time_marking_line.x) begin = time_marking_line.x;
	if (end > time_marking_line.x + time_marking_line.w) end = time_marking_line.x + time_marking_line.w;
}

TimelineComponent::TimelineComponent(UI_Window* win) : Component(win) {
	this->title = Texture::LoadText("Timeline", this->win->GetRenderer(), timeline_comp_font_size, "main", Color::RGB(CLR_SUB_TEXT));
	this->frame_size = 30;
	this->current_frame = 0;
	this->begin_frame = 0;
	this->pseudo_current_frame = 0;
	this->time_marker_dragging = false;
	this->time_table_dragging = false;
	this->prev_x = 0;

	this->position_text = Texture::LoadText("Position", this->win->GetRenderer(), timeline_comp_font_size, "main", Color::RGB(CLR_STD_TEXT));
	this->rotation_text = Texture::LoadText("Rotation", this->win->GetRenderer(), timeline_comp_font_size, "main", Color::RGB(CLR_STD_TEXT));
	this->scale_text = Texture::LoadText("Scale", this->win->GetRenderer(), timeline_comp_font_size, "main", Color::RGB(CLR_STD_TEXT));
	this->opacity_text = Texture::LoadText("Opacity", this->win->GetRenderer(), timeline_comp_font_size, "main", Color::RGB(CLR_STD_TEXT));

	this->position_add_button = new Button("[+]", this->win->GetRenderer(), timeline_comp_font_size, 0, 0);
	this->rotation_add_button = new Button("[+]", this->win->GetRenderer(), timeline_comp_font_size, 0, 0);
	this->scale_add_button = new Button("[+]", this->win->GetRenderer(), timeline_comp_font_size, 0, 0);
	this->opacity_add_button = new Button("[+]", this->win->GetRenderer(), timeline_comp_font_size, 0, 0);

	this->position_add_button->SetEnabled(false);
	this->rotation_add_button->SetEnabled(false);
	this->scale_add_button->SetEnabled(false);
	this->opacity_add_button->SetEnabled(false);

	for (int i = 0; i < 10; i++) {
		this->number_table.push_back(Texture::LoadText(to_string(i).c_str(), this->win->GetRenderer(), timeline_comp_font_size * 0.7, "main", Color::RGB(CLR_SUB_TEXT)));
	}

	this->current_bar = new TimelineBar(this->win, nullptr, 0);
}

TimelineComponent::~TimelineComponent() {
	SDL_DestroyTexture(this->title);
	SDL_DestroyTexture(this->position_text);
	SDL_DestroyTexture(this->rotation_text);
	SDL_DestroyTexture(this->scale_text);
	SDL_DestroyTexture(this->opacity_text);
	delete this->position_add_button, this->rotation_add_button, this->scale_add_button, this->opacity_add_button;
	for (auto& i : this->number_table) {
		SDL_DestroyTexture(i);
	}
	delete this->current_bar;
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
	SDL_QueryTexture(this->title, NULL, NULL, NULL, &rect.h);
	SDL_RenderFillRect(this->win->GetRenderer(), &rect);

	SDL_QueryTexture(this->title, NULL, NULL, &rect.w, NULL);
	rect.x += timeline_comp_margin;
	SDL_RenderCopy(this->win->GetRenderer(), this->title, NULL, &rect);

	// Menu

	rect.x = timeline_comp_x;
	rect.y = timeline_comp_y + rect.h;
	rect.w = timeline_comp_menu_width;
	rect.h = timeline_comp_height - rect.h - 1;
	SDL_SetRenderDrawColor(this->win->GetRenderer(), CLR_STD_BACKGROUND);
	SDL_RenderFillRect(this->win->GetRenderer(), &rect);

	// Timeline

	// Time Axis and Marking Line
	SDL_Rect time_marking_line = rect;
	time_marking_line.x = timeline_comp_x + timeline_comp_menu_width;
	time_marking_line.w = timeline_comp_width - timeline_comp_menu_width;
	int unit_frame = this->GetBigFrameUnit() / 10;
	if (!unit_frame) unit_frame = 1;
	double unit_pix = unit_frame * this->frame_size;
	for (double i = 0, num = 0; i - this->begin_frame * this->frame_size < time_marking_line.w; i += unit_pix, num += unit_frame) {
		int pos = i - this->begin_frame * this->frame_size + time_marking_line.x;
		if (pos > time_marking_line.x) {
			SDL_SetRenderDrawColor(this->win->GetRenderer(), CLR_STD_BACKGROUND);
			SDL_RenderDrawLine(this->win->GetRenderer(), pos, time_marking_line.y, pos, time_marking_line.y + time_marking_line.h);
			SDL_SetRenderDrawColor(this->win->GetRenderer(), CLR_COMP_BACKGROUND_BRIGHT_SUB);
			SDL_RenderDrawLine(this->win->GetRenderer(), pos, time_marking_line.y - 1, pos, time_marking_line.y - 5);
		}
	}
	unit_frame = this->GetBigFrameUnit();
	unit_pix = unit_frame * this->frame_size;
	for (double i = 0, num = 0; i - this->begin_frame * this->frame_size < time_marking_line.w; i += unit_pix, num += unit_frame) {
		int pos = i - this->begin_frame * this->frame_size + time_marking_line.x;
		if (pos >= time_marking_line.x) {
			SDL_SetRenderDrawColor(this->win->GetRenderer(), CLR_DARK_BACKGROUND);
			SDL_RenderDrawLine(this->win->GetRenderer(), pos, time_marking_line.y, pos, time_marking_line.y + time_marking_line.h);
			SDL_SetRenderDrawColor(this->win->GetRenderer(), CLR_COMP_BACKGROUND_BRIGHT);
			SDL_RenderDrawLine(this->win->GetRenderer(), pos, time_marking_line.y - 1, pos, time_marking_line.y - 10);
			this->DrawNumber(pos + 3, time_marking_line.y, num);
		}
	}

	// Timeline Menu
	SDL_Rect timeline_bar;
	timeline_bar.x = rect.x;
	timeline_bar.y = rect.y;
	timeline_bar.w = timeline_comp_width;
	if (SoftwareStatus::software_mode == SoftwareStatus::MOTION_EDITOR) {
		timeline_bar.h = rect.h / 4;
		SDL_SetRenderDrawColor(this->win->GetRenderer(), CLR_STD_BORDER);
		for (int i = 1; i < 4; i++) {
			SDL_RenderDrawLine(this->win->GetRenderer(), timeline_bar.x, timeline_bar.y + timeline_bar.h * i, timeline_bar.x + timeline_bar.w, timeline_bar.y + timeline_bar.h * i);
		}

		SDL_Rect menu_title;
		SDL_QueryTexture(this->position_text, NULL, NULL, &menu_title.w, &menu_title.h);
		menu_title.x = timeline_bar.x + timeline_comp_menu_width * 0.4 - menu_title.w / 2;
		menu_title.y = timeline_bar.y + timeline_bar.h / 2 - menu_title.h / 2;
		SDL_RenderCopy(this->win->GetRenderer(), this->position_text, NULL, &menu_title);

		SDL_QueryTexture(this->rotation_text, NULL, NULL, &menu_title.w, &menu_title.h);
		menu_title.x = timeline_bar.x + timeline_comp_menu_width * 0.4 - menu_title.w / 2;
		menu_title.y = timeline_bar.y + timeline_bar.h * 3 / 2 - menu_title.h / 2;
		SDL_RenderCopy(this->win->GetRenderer(), this->rotation_text, NULL, &menu_title);

		SDL_QueryTexture(this->scale_text, NULL, NULL, &menu_title.w, &menu_title.h);
		menu_title.x = timeline_bar.x + timeline_comp_menu_width * 0.4 - menu_title.w / 2;
		menu_title.y = timeline_bar.y + timeline_bar.h * 5 / 2 - menu_title.h / 2;
		SDL_RenderCopy(this->win->GetRenderer(), this->scale_text, NULL, &menu_title);

		SDL_QueryTexture(this->opacity_text, NULL, NULL, &menu_title.w, &menu_title.h);
		menu_title.x = timeline_bar.x + timeline_comp_menu_width * 0.4 - menu_title.w / 2;
		menu_title.y = timeline_bar.y + timeline_bar.h * 7 / 2 - menu_title.h / 2;
		SDL_RenderCopy(this->win->GetRenderer(), this->opacity_text, NULL, &menu_title);

		// Motion Object
		Motion* cur_motion = MotionStatus::CurrentMotion();
		timeline_bar.x = time_marking_line.x;
		timeline_bar.w = time_marking_line.w;

		if (cur_motion) {
			for (int i = 0; i < cur_motion->position.size(); i++) {
				TimelineBar bar(this->win, &cur_motion->position[i], 0);
				bar.Rendering(this, timeline_bar, time_marking_line, 0, Color::RGB(CLR_POSITION_BAR));
			}
			for (int i = 0; i < cur_motion->rotation.size(); i++) {
				TimelineBar bar(this->win, &cur_motion->rotation[i], 1);
				bar.Rendering(this, timeline_bar, time_marking_line, 0, Color::RGB(CLR_ROTATION_BAR));
			}
			for (int i = 0; i < cur_motion->scale.size(); i++) {
				TimelineBar bar(this->win, &cur_motion->scale[i], 2);
				bar.Rendering(this, timeline_bar, time_marking_line, 0, Color::RGB(CLR_SCALE_BAR));
			}
			for (int i = 0; i < cur_motion->opacity.size(); i++) {
				TimelineBar bar(this->win, &cur_motion->opacity[i], 3);
				bar.Rendering(this, timeline_bar, time_marking_line, 0, Color::RGB(CLR_OPACITY_BAR));
			}
		}

		this->position_add_button->SetPositionCentered(timeline_bar.x - 40, timeline_bar.y + timeline_bar.h * 0.5);
		this->rotation_add_button->SetPositionCentered(timeline_bar.x - 40, timeline_bar.y + timeline_bar.h * 1.5);
		this->scale_add_button->SetPositionCentered(timeline_bar.x - 40, timeline_bar.y + timeline_bar.h * 2.5);
		this->opacity_add_button->SetPositionCentered(timeline_bar.x - 40, timeline_bar.y + timeline_bar.h * 3.5);

		if (MotionStatus::CurrentMotion() && MotionStatus::CurrentMotion()->IsAbleToAddPosition(this->current_frame))
			this->position_add_button->SetEnabled(true);
		else this->position_add_button->SetEnabled(false);
		if (MotionStatus::CurrentMotion() && MotionStatus::CurrentMotion()->IsAbleToAddRotation(this->current_frame))
			this->rotation_add_button->SetEnabled(true);
		else this->rotation_add_button->SetEnabled(false);
		if (MotionStatus::CurrentMotion() && MotionStatus::CurrentMotion()->IsAbleToAddScale(this->current_frame))
			this->scale_add_button->SetEnabled(true);
		else this->scale_add_button->SetEnabled(false);
		if (MotionStatus::CurrentMotion() && MotionStatus::CurrentMotion()->IsAbleToAddOpacity(this->current_frame))
			this->opacity_add_button->SetEnabled(true);
		else this->opacity_add_button->SetEnabled(false);

		this->position_add_button->Render();
		this->rotation_add_button->Render();
		this->scale_add_button->Render();
		this->opacity_add_button->Render();
	}
	else {
		timeline_bar.h = 40;
	}

	// Time Marking Bar
	int pos = this->current_frame * this->frame_size - this->begin_frame * this->frame_size + time_marking_line.x;
	if (pos >= time_marking_line.x && pos <= time_marking_line.x + time_marking_line.w) {
		if (this->time_marker_dragging) {
			Color::RGB clr(CLR_WINDOW_THEME);
			SDL_SetRenderDrawColor(this->win->GetRenderer(), clr.r, clr.g, clr.b, 150);
		}
		else {
			SDL_SetRenderDrawColor(this->win->GetRenderer(), CLR_WINDOW_THEME);
		}
		SDL_RenderDrawLine(this->win->GetRenderer(), pos, time_marking_line.y, pos, time_marking_line.y + time_marking_line.h);
		SDL_Rect rect;
		rect.w = 5;
		rect.h = 10;
		rect.x = pos - rect.w / 2;
		rect.y = time_marking_line.y - rect.h;
		SDL_RenderFillRect(this->win->GetRenderer(), &rect);
	}

	rect.x = timeline_comp_x;
	rect.y = timeline_comp_y;
	rect.w = timeline_comp_width;
	rect.h = timeline_comp_height;

	SDL_SetRenderDrawColor(this->win->GetRenderer(), CLR_STD_BORDER);
	SDL_RenderDrawRect(this->win->GetRenderer(), &rect);
}

void TimelineComponent::EventProcess(SDL_Event* evt) {
	if (evt->type == SDL_MOUSEWHEEL) {
		if (!this->time_marker_dragging && this->current_bar && !this->current_bar->IsBarEventProcessing()) {
			if (SDL_GetModState() & KMOD_ALT) {
				double limit = log2(this->frame_size) + (double)evt->wheel.y / 10.0;
				if (limit < -5)
					this->frame_size = pow(2, -5);
				else if (limit > 8)
					this->frame_size = pow(2, 8);
				else this->frame_size *= pow(2, (double)evt->wheel.y / 10.0);
				if (this->begin_frame < 0) this->begin_frame = 0;
			}
			else if (SDL_GetModState() & KMOD_CTRL) {

			}
			else {
				this->begin_frame -= evt->wheel.y / this->frame_size * 40.0;
				if (this->begin_frame < 0) this->begin_frame = 0;
			}
		}
		return;
	}

	if (this->time_table_dragging) {
		this->begin_frame -= (evt->motion.x - this->prev_x) / this->frame_size;
		if (this->begin_frame < 0) this->begin_frame = 0;
		else this->prev_x = evt->motion.x;
		if(evt->type == SDL_MOUSEBUTTONUP)
			this->time_table_dragging = false;
		return;
	}

	SDL_Rect rect;
	rect.x = timeline_comp_x;
	SDL_QueryTexture(this->title, NULL, NULL, NULL, &rect.h);
	rect.y = timeline_comp_y + rect.h;
	rect.w = timeline_comp_menu_width;
	rect.h = timeline_comp_height - rect.h - 2;
	
	// Time Marking Line
	SDL_Rect time_marking_line = rect;
	time_marking_line.x = timeline_comp_x + timeline_comp_menu_width;
	time_marking_line.w = timeline_comp_width - timeline_comp_menu_width;
	int pos = this->current_frame * this->frame_size - this->begin_frame * this->frame_size + time_marking_line.x;
	if (pos >= time_marking_line.x && pos <= time_marking_line.x + time_marking_line.w) {
		SDL_Rect rect;
		rect.w = 5;
		rect.h = time_marking_line.h + 10;
		rect.x = pos - rect.w / 2;
		rect.y = time_marking_line.y - 10;

		int x = evt->motion.x;
		int y = evt->motion.y;

		if (x >= rect.x && x <= rect.x + rect.w && y >= rect.y && y <= rect.y + rect.h || this->time_marker_dragging) {
			Status::SetCursor(SDL_SYSTEM_CURSOR_SIZEWE);
			if (evt->type == SDL_MOUSEBUTTONDOWN) {
				this->time_marker_dragging = true;
				this->prev_x = evt->button.x;
				return;
			}
			else if (this->time_marker_dragging && evt->type == SDL_MOUSEBUTTONUP) {
				this->time_marker_dragging = false;
				return;
			}
			if (this->time_marker_dragging && evt->type == SDL_MOUSEMOTION) {
				this->pseudo_current_frame += (double)(evt->motion.x - this->prev_x) / this->frame_size;
				this->prev_x = x;
				if (this->pseudo_current_frame < 0) {
					this->pseudo_current_frame = 0;
					this->prev_x = time_marking_line.x;
				}
				this->current_frame = round(pseudo_current_frame);
				int pos = this->current_frame * this->frame_size - this->begin_frame * this->frame_size + time_marking_line.x;
				if (pos < time_marking_line.x)
					this->begin_frame -= (time_marking_line.x - pos + 1) / this->frame_size, this->prev_x = time_marking_line.x + this->frame_size / 2;
				else if(pos > time_marking_line.x + time_marking_line.w)
					this->begin_frame += (pos - (time_marking_line.x + time_marking_line.w) + 2) / this->frame_size, this->prev_x = time_marking_line.x + time_marking_line.w - this->frame_size / 2;
				return;
			}
		}
	}

	SDL_Rect timeline_bar = time_marking_line;
	if (SoftwareStatus::software_mode == SoftwareStatus::MOTION_EDITOR) {
		timeline_bar.h /= 4;
		
		// Add Button
		this->position_add_button->EventProcess(evt);
		this->rotation_add_button->EventProcess(evt);
		this->scale_add_button->EventProcess(evt);
		this->opacity_add_button->EventProcess(evt);

		if (this->position_add_button->IsClicked()) {
			MotionStatus::CurrentMotion()->AddPosition(this->current_frame, this->frame_size);
			this->current_frame = MotionStatus::CurrentMotion()->position.back().end_frame;
			this->pseudo_current_frame = this->current_frame;
			delete this->current_bar;
			this->current_bar = new TimelineBar(this->win, &MotionStatus::CurrentMotion()->position.back(), 0);
			return;
		}
		if (this->rotation_add_button->IsClicked()) {
			MotionStatus::CurrentMotion()->AddRotation(this->current_frame, this->frame_size);
			this->current_frame = MotionStatus::CurrentMotion()->rotation.back().end_frame;
			this->pseudo_current_frame = this->current_frame;
			delete this->current_bar;
			this->current_bar = new TimelineBar(this->win, &MotionStatus::CurrentMotion()->rotation.back(), 0);
			return;
		}
		if (this->scale_add_button->IsClicked()) {
			MotionStatus::CurrentMotion()->AddScale(this->current_frame, this->frame_size);
			this->current_frame = MotionStatus::CurrentMotion()->scale.back().end_frame;
			this->pseudo_current_frame = this->current_frame;
			delete this->current_bar;
			this->current_bar = new TimelineBar(this->win, &MotionStatus::CurrentMotion()->scale.back(), 0);
			return;
		}
		if (this->opacity_add_button->IsClicked()) {
			MotionStatus::CurrentMotion()->AddOpacity(this->current_frame, this->frame_size);
			this->current_frame = MotionStatus::CurrentMotion()->opacity.back().end_frame;
			this->pseudo_current_frame = this->current_frame;
			delete this->current_bar;
			this->current_bar = new TimelineBar(this->win, &MotionStatus::CurrentMotion()->opacity.back(), 0);
			return;
		}

		// Current Selected Motion Unit
		if (this->current_bar->SelectedBarEventProcess(this, evt, timeline_bar, time_marking_line, 0)) {
			return;
		}

		// Motion Bar Selecting
		Motion* cur_motion = MotionStatus::CurrentMotion();
		if (cur_motion) {
			if (cur_motion) {
				for (int i = 0; i < cur_motion->position.size(); i++) {
					TimelineBar bar(this->win, &cur_motion->position[i], 0);
					if (bar.IsClicked(this, evt, timeline_bar, time_marking_line, 0)) {
						delete this->current_bar;
						this->current_bar = new TimelineBar(this->win, &cur_motion->position[i], 0);
						this->current_bar->Press(evt->button.x);
						return;
					}
				}
				for (int i = 0; i < cur_motion->rotation.size(); i++) {
					TimelineBar bar(this->win, &cur_motion->rotation[i], 1);
					if (bar.IsClicked(this, evt, timeline_bar, time_marking_line, 0)) {
						delete this->current_bar;
						this->current_bar = new TimelineBar(this->win, &cur_motion->rotation[i], 1);
						this->current_bar->Press(evt->button.x);
						return;
					}
				}
				for (int i = 0; i < cur_motion->scale.size(); i++) {
					TimelineBar bar(this->win, &cur_motion->scale[i], 2);
					if (bar.IsClicked(this, evt, timeline_bar, time_marking_line, 0)) {
						delete this->current_bar;
						this->current_bar = new TimelineBar(this->win, &cur_motion->scale[i], 2);
						this->current_bar->Press(evt->button.x);
						return;
					}
				}
				for (int i = 0; i < cur_motion->opacity.size(); i++) {
					TimelineBar bar(this->win, &cur_motion->opacity[i], 3);
					if (bar.IsClicked(this, evt, timeline_bar, time_marking_line, 0)) {
						delete this->current_bar;
						this->current_bar = new TimelineBar(this->win, &cur_motion->opacity[i], 3);
						this->current_bar->Press(evt->button.x);
						return;
					}
				}
			}
		}
	}
	else {
		timeline_bar.h = 40;
	}

	if (evt->type == SDL_MOUSEBUTTONDOWN) {
		delete this->current_bar;
		this->current_bar = new TimelineBar(this->win, nullptr, 0);

		if (evt->motion.x > time_marking_line.x && evt->motion.x < time_marking_line.x + time_marking_line.w) {
			if (evt->motion.y > time_marking_line.y && evt->motion.y < time_marking_line.y + time_marking_line.h) {
				this->time_table_dragging = true;
				this->prev_x = evt->button.x;
			}
		}
		time_marking_line.h = time_marking_line.y - timeline_comp_y;
		time_marking_line.y = timeline_comp_y;
		if (evt->motion.x > time_marking_line.x && evt->motion.x < time_marking_line.x + time_marking_line.w) {
			if (evt->motion.y > time_marking_line.y && evt->motion.y < time_marking_line.y + time_marking_line.h) {
				this->pseudo_current_frame = (evt->motion.x - time_marking_line.x) / this->frame_size + this->begin_frame;
				this->current_frame = round(this->pseudo_current_frame);
				this->time_marker_dragging = true;
				this->prev_x = evt->button.x;
			}
		}
	}

	Status::SetCursor(SDL_SYSTEM_CURSOR_ARROW);
}

bool TimelineBar::CalculateBarArea(TimelineComponent* comp, SDL_Rect& bar_area, SDL_Rect& total_area, int offset) {
	if (this->motion) {
		bar_area.y += bar_area.h * this->level - offset;

		if (bar_area.y + bar_area.h < total_area.y || bar_area.y > total_area.y + total_area.h) return false;
		if (bar_area.y < total_area.y) {
			bar_area.h -= total_area.y - bar_area.y;
			bar_area.y = total_area.y;
		}
		if (bar_area.y + bar_area.h > total_area.y + total_area.h) {
			bar_area.h -= bar_area.y + bar_area.h - (total_area.y + total_area.h);
		}

		int begin = this->motion->begin_frame * comp->frame_size - comp->begin_frame * comp->frame_size + bar_area.x;
		int end = this->motion->end_frame * comp->frame_size - comp->begin_frame * comp->frame_size + bar_area.x;
		if (begin > bar_area.x + bar_area.w || end < bar_area.x) return false;
		if (begin < bar_area.x) begin = bar_area.x;
		if (end > bar_area.x + bar_area.w) end = bar_area.x + bar_area.w;

		bar_area.x = begin;
		bar_area.w = end - begin;
		return true;
	}
	return false;
}

TimelineBar::TimelineBar(UI_Window* win, Motion::Unit* motion, int level) {
	this->win = win;
	this->motion = motion;
	this->is_pressed = false;
	this->is_left_resizing = false;
	this->is_right_resizing = false;
	this->level = level;
}

TimelineBar::~TimelineBar() {
	
}

void TimelineBar::Rendering(TimelineComponent* comp, SDL_Rect bar_area, SDL_Rect total_area, int offset, Color::RGB clr) {
	if (this->motion) {
		const int unselected_alpha = 32;
		const int selected_alpha = 64;
		const int border_alpha = 128;

		if (!this->CalculateBarArea(comp, bar_area, total_area, offset)) return;

		if (this->motion == comp->current_bar->motion)
			SDL_SetRenderDrawColor(this->win->GetRenderer(), clr.r, clr.g, clr.b, selected_alpha);
		else SDL_SetRenderDrawColor(this->win->GetRenderer(), clr.r, clr.g, clr.b, unselected_alpha);

		SDL_RenderFillRect(this->win->GetRenderer(), &bar_area);
		SDL_SetRenderDrawColor(this->win->GetRenderer(), clr.r, clr.g, clr.b, border_alpha);
		SDL_RenderDrawRect(this->win->GetRenderer(), &bar_area);
	}
}

bool TimelineBar::IsClicked(TimelineComponent* comp, SDL_Event* evt, SDL_Rect bar_area, SDL_Rect total_area, int offset) {
	if (this->motion) {
		if (!this->CalculateBarArea(comp, bar_area, total_area, offset)) return false;

		int x = evt->button.x;
		int y = evt->button.y;
		if (x >= bar_area.x && x <= bar_area.x + bar_area.w && y >= bar_area.y && y <= bar_area.y + bar_area.h) {
			if (evt->type == SDL_MOUSEBUTTONDOWN) {
				comp->current_bar->motion = this->motion;
				return true;
			}
		}
	}
	return false;
}

bool TimelineBar::SelectedBarEventProcess(TimelineComponent* comp, SDL_Event* evt, SDL_Rect bar_area, SDL_Rect total_area, int offset) {
	if (this->motion) {
		if (!this->CalculateBarArea(comp, bar_area, total_area, offset)) return false;
		int x = evt->button.x;
		int y = evt->button.y;
		if (this->is_pressed) {
			Status::SetCursor(SDL_SYSTEM_CURSOR_HAND);
		}
		else if (this->is_left_resizing || this->is_right_resizing) {
			Status::SetCursor(SDL_SYSTEM_CURSOR_SIZEWE);
		}
		if (evt->type == SDL_MOUSEBUTTONDOWN) {
			SDL_Rect left_area = bar_area;
			left_area.w = 5;
			left_area.x = bar_area.x - left_area.w / 2;
			SDL_Rect right_area = bar_area;
			right_area.w = 5;
			right_area.x = bar_area.x + bar_area.w - right_area.w / 2;

			this->prev_x = x;
			this->begin_frame = this->motion->begin_frame;
			this->end_frame = this->motion->end_frame;

			if (x >= left_area.x && x <= left_area.x + left_area.w && y >= left_area.y && y <= left_area.y + left_area.h) {
				this->is_left_resizing = true;
				Status::SetCursor(SDL_SYSTEM_CURSOR_SIZEWE);
				return true;
			}
			else if (x >= right_area.x && x <= right_area.x + right_area.w && y >= right_area.y && y <= right_area.y + right_area.h) {
				this->is_right_resizing = true;
				Status::SetCursor(SDL_SYSTEM_CURSOR_SIZEWE);
				return true;
			}
			else if (x >= bar_area.x && x <= bar_area.x + bar_area.w && y >= bar_area.y && y <= bar_area.y + bar_area.h) {
				this->is_pressed = true;
				Status::SetCursor(SDL_SYSTEM_CURSOR_HAND);
				this->start_pos = x;
				return true;
			}
		}
		else if (evt->type == SDL_MOUSEBUTTONUP) {
			int front, back;
			if (this->is_pressed) {
				if (SoftwareStatus::software_mode == SoftwareStatus::MOTION_EDITOR) {
					int unit = 1;
					if (this->start_pos < x) {
						unit = -1;
					}
					while (MotionStatus::CurrentMotion()->IsOverlaped(this->motion, this->level)) {
						this->begin_frame += unit;
						this->end_frame += unit;
						this->motion->begin_frame = round(this->begin_frame);
						this->motion->end_frame = round(this->end_frame);
					}
				}
				else {

				}
			}
			else if (this->is_left_resizing) {
				if (SoftwareStatus::software_mode == SoftwareStatus::MOTION_EDITOR) {
					MotionStatus::CurrentMotion()->GetFrontBackFrame(this->motion->end_frame, front, back, this->level);
					if (this->begin_frame < front) this->begin_frame = front;
					this->motion->begin_frame = round(this->begin_frame);
				}
				else {
					
				}
			}
			else if (this->is_right_resizing) {
				if (SoftwareStatus::software_mode == SoftwareStatus::MOTION_EDITOR) {
					MotionStatus::CurrentMotion()->GetFrontBackFrame(this->motion->begin_frame, front, back, this->level);
					if (this->end_frame > back) this->end_frame = back;
					this->motion->end_frame = round(this->end_frame);
				}
				else {

				}
			}
			this->is_pressed = false;
			this->is_left_resizing = false;
			this->is_right_resizing = false;
		}
		else if (evt->type == SDL_MOUSEMOTION) {
			if (this->is_pressed) {
				this->begin_frame += (double)(evt->motion.x - this->prev_x) / comp->frame_size;
				this->end_frame += (double)(evt->motion.x - this->prev_x) / comp->frame_size;
				if (this->begin_frame < 0) {
					this->end_frame = this->end_frame - this->begin_frame;
					this->begin_frame = 0;
				}
				else {
					this->prev_x = evt->motion.x;
				}

				this->motion->begin_frame = round(this->begin_frame);
				this->motion->end_frame = round(this->end_frame);
				return true;
			}
			else if (this->is_left_resizing) {
				this->begin_frame += (double)(evt->motion.x - this->prev_x) / comp->frame_size;
				int prev = this->prev_x;
				if (this->begin_frame < 0) {
					this->end_frame = this->end_frame - this->begin_frame;
					this->begin_frame = 0;
				}
				else if (this->motion->end_frame - 1 <= this->begin_frame) {
					this->begin_frame = this->motion->end_frame - 1;
				}
				else {
					this->prev_x = evt->motion.x;
				}
				
				this->motion->begin_frame = round(this->begin_frame);
				return true;
			}
			else if (this->is_right_resizing) {
				this->end_frame += (double)(evt->motion.x - this->prev_x) / comp->frame_size;
				if (this->motion->begin_frame + 1 >= this->end_frame) {
					this->end_frame = this->motion->begin_frame + 1;
				}
				else {
					this->prev_x = evt->motion.x;
				}

				this->motion->end_frame = round(this->end_frame);
				return true;
			}

			SDL_Rect left_area = bar_area;
			left_area.w = 5;
			left_area.x = bar_area.x - left_area.w / 2;
			SDL_Rect right_area = bar_area;
			right_area.w = 5;
			right_area.x = bar_area.x + bar_area.w - right_area.w / 2;

			this->begin_frame = this->motion->begin_frame;
			this->end_frame = this->motion->end_frame;

			x = evt->motion.x;
			y = evt->motion.y;

			if (x >= left_area.x && x <= left_area.x + left_area.w && y >= left_area.y && y <= left_area.y + left_area.h) {
				Status::SetCursor(SDL_SYSTEM_CURSOR_SIZEWE);
				return true;
			}
			else if (x >= right_area.x && x <= right_area.x + right_area.w && y >= right_area.y && y <= right_area.y + right_area.h) {
				Status::SetCursor(SDL_SYSTEM_CURSOR_SIZEWE);
				return true;
			}
			else if (x >= bar_area.x && x <= bar_area.x + bar_area.w && y >= bar_area.y && y <= bar_area.y + bar_area.h) {
				Status::SetCursor(SDL_SYSTEM_CURSOR_HAND);
				return true;
			}
		}
	}
	return false;
}

void TimelineBar::Press(int x) {
	this->is_pressed = true;
	this->prev_x = x;
	this->begin_frame = this->motion->begin_frame;
	this->end_frame = this->motion->end_frame;
	this->start_pos = x;
}

bool TimelineBar::IsBarEventProcessing() {
	return this->is_left_resizing || this->is_pressed || this->is_right_resizing;
}
