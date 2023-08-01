#include "TimelineComponent.h"
#include "Texture.h"
#include "MenuComponent.h"
#include "SettingComponent.h"
#include "SoftwareStatus.h"
#include "Status.h"

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

TimelineComponent::TimelineComponent(UI_Window* win) : Component(win) {
	this->title = Texture::LoadText("Timeline", this->win->GetRenderer(), timeline_comp_font_size, "main", Color::RGB(CLR_SUB_TEXT));
	this->frame_size = 30;
	this->current_frame = 0;
	this->begin_frame = 0;
	this->time_marker_dragging = false;
	this->prev_x = 0;

	this->position_text = Texture::LoadText("Position", this->win->GetRenderer(), timeline_comp_font_size, "main", Color::RGB(CLR_STD_TEXT));
	this->rotation_text = Texture::LoadText("Rotation", this->win->GetRenderer(), timeline_comp_font_size, "main", Color::RGB(CLR_STD_TEXT));
	this->scale_text = Texture::LoadText("Scale", this->win->GetRenderer(), timeline_comp_font_size, "main", Color::RGB(CLR_STD_TEXT));
	this->opacity_text = Texture::LoadText("Opacity", this->win->GetRenderer(), timeline_comp_font_size, "main", Color::RGB(CLR_STD_TEXT));

	this->position_add_button = new Button("[+ ADD]", this->win->GetRenderer(), timeline_comp_font_size, 0, 0);
	this->rotation_add_button = new Button("[+ ADD]", this->win->GetRenderer(), timeline_comp_font_size, 0, 0);
	this->scale_add_button = new Button("[+ ADD]", this->win->GetRenderer(), timeline_comp_font_size, 0, 0);
	this->opacity_add_button = new Button("[+ ADD]", this->win->GetRenderer(), timeline_comp_font_size, 0, 0);

	for (int i = 0; i < 10; i++) {
		this->number_table.push_back(Texture::LoadText(to_string(i).c_str(), this->win->GetRenderer(), timeline_comp_font_size * 0.7, "main", Color::RGB(CLR_SUB_TEXT)));
	}
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
	SDL_SetRenderDrawColor(this->win->GetRenderer(), CLR_STD_BACKGROUND);
	int unit_frame = this->GetBigFrameUnit() / 10;
	if (!unit_frame) unit_frame = 1;
	double unit_pix = unit_frame * this->frame_size;
	for (double i = 0, num = 0; i - this->begin_frame * this->frame_size < time_marking_line.w; i += unit_pix, num += unit_frame) {
		int pos = i - this->begin_frame * this->frame_size + time_marking_line.x;
		if (pos > time_marking_line.x) {
			SDL_RenderDrawLine(this->win->GetRenderer(), pos, time_marking_line.y, pos, time_marking_line.y + time_marking_line.h);
		}
	}
	unit_frame = this->GetBigFrameUnit();
	unit_pix = unit_frame * this->frame_size;
	SDL_SetRenderDrawColor(this->win->GetRenderer(), CLR_DARK_BACKGROUND);
	for (double i = 0, num = 0; i - this->begin_frame * this->frame_size < time_marking_line.w; i += unit_pix, num += unit_frame) {
		int pos = i - this->begin_frame * this->frame_size + time_marking_line.x;
		if (pos >= time_marking_line.x) {
			SDL_RenderDrawLine(this->win->GetRenderer(), pos, time_marking_line.y, pos, time_marking_line.y + time_marking_line.h);
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
		menu_title.x = timeline_bar.x + timeline_comp_menu_width / 2 - menu_title.w / 2;
		menu_title.y = timeline_bar.y + timeline_bar.h / 2 - menu_title.h / 2;
		SDL_RenderCopy(this->win->GetRenderer(), this->position_text, NULL, &menu_title);

		SDL_QueryTexture(this->rotation_text, NULL, NULL, &menu_title.w, &menu_title.h);
		menu_title.x = timeline_bar.x + timeline_comp_menu_width / 2 - menu_title.w / 2;
		menu_title.y = timeline_bar.y + timeline_bar.h * 3 / 2 - menu_title.h / 2;
		SDL_RenderCopy(this->win->GetRenderer(), this->rotation_text, NULL, &menu_title);

		SDL_QueryTexture(this->scale_text, NULL, NULL, &menu_title.w, &menu_title.h);
		menu_title.x = timeline_bar.x + timeline_comp_menu_width / 2 - menu_title.w / 2;
		menu_title.y = timeline_bar.y + timeline_bar.h * 5 / 2 - menu_title.h / 2;
		SDL_RenderCopy(this->win->GetRenderer(), this->scale_text, NULL, &menu_title);

		SDL_QueryTexture(this->opacity_text, NULL, NULL, &menu_title.w, &menu_title.h);
		menu_title.x = timeline_bar.x + timeline_comp_menu_width / 2 - menu_title.w / 2;
		menu_title.y = timeline_bar.y + timeline_bar.h * 7 / 2 - menu_title.h / 2;
		SDL_RenderCopy(this->win->GetRenderer(), this->opacity_text, NULL, &menu_title);
	}
	else {
		timeline_bar.h = 40;
	}

	int pos = round(this->current_frame) * this->frame_size - this->begin_frame * this->frame_size + time_marking_line.x;
	if (pos >= time_marking_line.x && pos <= time_marking_line.x + time_marking_line.w) {
		if (this->time_marker_dragging) {
			Color::RGB clr(CLR_WINDOW_THEME);
			SDL_SetRenderDrawColor(this->win->GetRenderer(), clr.r, clr.g, clr.b, 100);
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
		if (SDL_GetModState() & KMOD_ALT) {
			double limit = log2(this->frame_size) + (double)evt->wheel.y / 10.0;
			if(limit < -5)
				this->frame_size = pow(2, -5);
			else if (limit > 8)
				this->frame_size = pow(2, 8);
			else this->frame_size *= pow(2, (double)evt->wheel.y / 10.0);
		}
		else if (SDL_GetModState() & KMOD_CTRL) {

		}
		else {
			this->begin_frame -= (double)evt->wheel.y / this->frame_size * 20.0;
			if (this->begin_frame < 0) this->begin_frame = 0;
		}
		return;
	}

	SDL_Rect rect;
	rect.x = timeline_comp_x;
	SDL_QueryTexture(this->title, NULL, NULL, NULL, &rect.h);
	rect.y = timeline_comp_y + rect.h;
	rect.w = timeline_comp_menu_width;
	rect.h = timeline_comp_height - rect.h - 1;
	
	// Time Marking Line
	SDL_Rect time_marking_line = rect;
	time_marking_line.x = timeline_comp_x + timeline_comp_menu_width;
	time_marking_line.w = timeline_comp_width - timeline_comp_menu_width;
	int pos = round(this->current_frame) * this->frame_size - this->begin_frame * this->frame_size + time_marking_line.x;
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
			}
			else if (evt->type == SDL_MOUSEBUTTONUP) {
				this->time_marker_dragging = false;
			}
			if (this->time_marker_dragging) {
				this->current_frame += (double)(evt->motion.x - this->prev_x) / this->frame_size;
				this->prev_x = x;
				if (this->current_frame < 0) {
					this->current_frame = 0;
					this->prev_x = time_marking_line.x;
				}
				int pos = round(this->current_frame) * this->frame_size - this->begin_frame * this->frame_size + time_marking_line.x;
				if (pos < time_marking_line.x)
					this->begin_frame -= (time_marking_line.x - pos + 1) / this->frame_size, this->prev_x = time_marking_line.x + this->frame_size / 2;
				else if(pos > time_marking_line.x + time_marking_line.w)
					this->begin_frame += (pos - (time_marking_line.x + time_marking_line.w) + 2) / this->frame_size, this->prev_x = time_marking_line.x + time_marking_line.w - this->frame_size / 2;
			}
			return;
		}
		else {
			Status::SetCursor(SDL_SYSTEM_CURSOR_ARROW);
		}
	}

	SDL_Rect timeline_bar;
	timeline_bar.x = rect.x;
	timeline_bar.y = rect.y;
	timeline_bar.w = timeline_comp_width;
	if (SoftwareStatus::software_mode == SoftwareStatus::MOTION_EDITOR) {
		timeline_bar.h = rect.h / 4;
	}
	else {
		timeline_bar.h = 40;
	}
}
