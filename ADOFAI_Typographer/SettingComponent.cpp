#include "SettingComponent.h"
#include "Texture.h"
#include "MenuComponent.h"
#include "ScreenComponent.h"
#include "MotionListComponent.h"
#include "MotionStatus.h"

SettingComponent::SettingComponent(UI_Window* win) : Component(win) {
	this->title = Texture::LoadText("Setting", this->win->GetRenderer(), setting_comp_font_size, "main", Color::RGB(CLR_SUB_TEXT));

	this->begin_frame_text = Texture::LoadText("Start Frame", this->win->GetRenderer(), setting_comp_font_size, "main", Color::RGB(CLR_SUB_TEXT));
	this->end_frame_text = Texture::LoadText("End Frame", this->win->GetRenderer(), setting_comp_font_size, "main", Color::RGB(CLR_SUB_TEXT));
	this->imm_run_text = Texture::LoadText("Immediately Run", this->win->GetRenderer(), setting_comp_font_size, "main", Color::RGB(CLR_SUB_TEXT));
	this->imm_frame_text = Texture::LoadText("Frame", this->win->GetRenderer(), setting_comp_font_size, "main", Color::RGB(CLR_SUB_TEXT));
	this->pos_x_text = Texture::LoadText("Position X", this->win->GetRenderer(), setting_comp_font_size, "main", Color::RGB(CLR_SUB_TEXT));
	this->pos_y_text = Texture::LoadText("Position Y", this->win->GetRenderer(), setting_comp_font_size, "main", Color::RGB(CLR_SUB_TEXT));
	this->rot_deg_text = Texture::LoadText("Degree", this->win->GetRenderer(), setting_comp_font_size, "main", Color::RGB(CLR_SUB_TEXT));
	this->scale_x_text = Texture::LoadText("Scale X", this->win->GetRenderer(), setting_comp_font_size, "main", Color::RGB(CLR_SUB_TEXT));
	this->scale_y_text = Texture::LoadText("Scale Y", this->win->GetRenderer(), setting_comp_font_size, "main", Color::RGB(CLR_SUB_TEXT));
	this->opacity_text = Texture::LoadText("Opacity", this->win->GetRenderer(), setting_comp_font_size, "main", Color::RGB(CLR_SUB_TEXT));
	this->transition_text = Texture::LoadText("Transition", this->win->GetRenderer(), setting_comp_font_size, "main", Color::RGB(CLR_SUB_TEXT));

	this->warnning_text = Texture::LoadText("[Setting]\nSetting area width is too short! Please widen the width of this window", this->win->GetRenderer(), setting_comp_font_size, "main", Color::RGB(CLR_SUB_TEXT), setting_comp_minimum_width * 0.75);
	this->prev_width = setting_comp_minimum_width;

	this->frame_input_a = new NumberInput(0, this->win->GetRenderer(), setting_comp_font_size, 0, 0, NumberInput::UNSIGNED_INT);
	this->frame_input_b = new NumberInput(0, this->win->GetRenderer(), setting_comp_font_size, 0, 0, NumberInput::UNSIGNED_INT);
	this->value_input_a = new NumberInput(0, this->win->GetRenderer(), setting_comp_font_size, 0, 0, NumberInput::UNSIGNED_FLOAT);
	this->value_input_b = new NumberInput(0, this->win->GetRenderer(), setting_comp_font_size, 0, 0, NumberInput::UNSIGNED_FLOAT);

	this->imm_switch_yes = new Button("Yes", this->win->GetRenderer(), setting_comp_font_size, 0, 0);
	this->imm_switch_no = new Button("No", this->win->GetRenderer(), setting_comp_font_size, 0, 0);
}

SettingComponent::~SettingComponent() {
	SDL_DestroyTexture(this->title);
	SDL_DestroyTexture(this->begin_frame_text);
	SDL_DestroyTexture(this->end_frame_text);
	SDL_DestroyTexture(this->imm_run_text);
	SDL_DestroyTexture(this->imm_frame_text);
	SDL_DestroyTexture(this->pos_x_text);
	SDL_DestroyTexture(this->pos_y_text);
	SDL_DestroyTexture(this->rot_deg_text);
	SDL_DestroyTexture(this->scale_x_text);
	SDL_DestroyTexture(this->scale_y_text);
	SDL_DestroyTexture(this->opacity_text);
	SDL_DestroyTexture(this->transition_text);
	SDL_DestroyTexture(this->warnning_text);

	delete this->frame_input_a, this->frame_input_b, this->value_input_a, this->value_input_b;
	delete this->imm_switch_yes, this->imm_switch_no;
}

void SettingComponent::Rendering() {
	SDL_Rect rect;
	rect.x = setting_comp_x;
	rect.y = setting_comp_y;
	rect.w = setting_comp_width;
	rect.h = setting_comp_height;

	SDL_SetRenderDrawColor(this->win->GetRenderer(), CLR_COMP_BACKGROUND);
	SDL_RenderFillRect(this->win->GetRenderer(), &rect);
	SDL_SetRenderDrawColor(this->win->GetRenderer(), CLR_STD_BORDER);
	SDL_RenderDrawRect(this->win->GetRenderer(), &rect);


	if (rect.w < setting_comp_minimum_width) {
		if (this->prev_width != rect.w) {
			this->prev_width = rect.w;
			SDL_DestroyTexture(this->warnning_text);
			this->warnning_text = Texture::LoadText("[Setting]\nSetting area width is too short!", this->win->GetRenderer(), setting_comp_font_size, "main", Color::RGB(CLR_SUB_TEXT), rect.w * 0.75);
		}
		SDL_Rect warnning_rect = rect;
		SDL_QueryTexture(this->warnning_text, NULL, NULL, &warnning_rect.w, &warnning_rect.h);
		warnning_rect.x += (rect.w - warnning_rect.w) / 2;
		warnning_rect.y += (rect.h - warnning_rect.h) / 2;
		if (warnning_rect.h < rect.h * 0.75) {
			SDL_RenderCopy(this->win->GetRenderer(), this->warnning_text, NULL, &warnning_rect);
		}
		return;
	}

	SDL_Rect setting_title_rect = rect;

	SDL_QueryTexture(this->title, NULL, NULL, NULL, &rect.h);
	SDL_RenderFillRect(this->win->GetRenderer(), &rect);

	setting_title_rect.y += rect.h + 1;
	setting_title_rect.h -= rect.h + 2;
	setting_title_rect.x += 1;
	setting_title_rect.w -= 2;

	SDL_QueryTexture(this->title, NULL, NULL, &rect.w, NULL);
	rect.x += setting_comp_margin;
	SDL_RenderCopy(this->win->GetRenderer(), this->title, NULL, &rect);

	setting_title_rect.w = setting_comp_minimum_width * 0.75;
	SDL_SetRenderDrawColor(this->win->GetRenderer(), CLR_STD_BACKGROUND);
	SDL_RenderFillRect(this->win->GetRenderer(), &setting_title_rect);

	SDL_Rect setting_title_text_rect = setting_title_rect;
	Motion* motion = MotionStatus::CurrentMotion();
	if (motion) {
		Motion::Unit* unit = motion->current_motion_unit;
		if (unit) {
			setting_title_text_rect.x += setting_comp_margin;
			setting_title_text_rect.y += setting_comp_margin;
			SDL_Rect input_rect;

			// Frame Settings
			if (unit->immediate) {
				SDL_QueryTexture(this->imm_frame_text, NULL, NULL, &setting_title_text_rect.w, &setting_title_text_rect.h);
				SDL_RenderCopy(this->win->GetRenderer(), this->imm_frame_text, NULL, &setting_title_text_rect);
				this->frame_input_a->SetValue(unit->begin_frame);
				input_rect = this->frame_input_a->GetRect();
				input_rect.w = setting_comp_width - setting_title_rect.w - setting_comp_margin * 2;
				input_rect.x = setting_comp_x + setting_title_rect.w + setting_comp_margin;
				input_rect.y = setting_title_text_rect.y;
				this->frame_input_a->SetRect(input_rect);
				this->frame_input_a->Render();
				setting_title_text_rect.y += 2 * (setting_title_text_rect.h + setting_comp_margin);
			}
			else {
				SDL_QueryTexture(this->begin_frame_text, NULL, NULL, &setting_title_text_rect.w, &setting_title_text_rect.h);
				SDL_RenderCopy(this->win->GetRenderer(), this->begin_frame_text, NULL, &setting_title_text_rect);
				this->frame_input_a->SetValue(unit->begin_frame);
				input_rect = this->frame_input_a->GetRect();
				input_rect.w = setting_comp_width - setting_title_rect.w - setting_comp_margin * 2;
				input_rect.x = setting_comp_x + setting_title_rect.w + setting_comp_margin;
				input_rect.y = setting_title_text_rect.y;
				this->frame_input_a->SetRect(input_rect);
				this->frame_input_a->Render();
				setting_title_text_rect.y += setting_title_text_rect.h + setting_comp_margin;

				SDL_QueryTexture(this->end_frame_text, NULL, NULL, &setting_title_text_rect.w, &setting_title_text_rect.h);
				SDL_RenderCopy(this->win->GetRenderer(), this->end_frame_text, NULL, &setting_title_text_rect);
				this->frame_input_b->SetValue(unit->end_frame);
				input_rect = this->frame_input_b->GetRect();
				input_rect.w = setting_comp_width - setting_title_rect.w - setting_comp_margin * 2;
				input_rect.x = setting_comp_x + setting_title_rect.w + setting_comp_margin;
				input_rect.y = setting_title_text_rect.y;
				this->frame_input_b->SetRect(input_rect);
				this->frame_input_b->Render();
				setting_title_text_rect.y += setting_title_text_rect.h + setting_comp_margin;
			}
			SDL_QueryTexture(this->imm_run_text, NULL, NULL, &setting_title_text_rect.w, &setting_title_text_rect.h);
			SDL_RenderCopy(this->win->GetRenderer(), this->imm_run_text, NULL, &setting_title_text_rect);
			if (unit->immediate) {
				this->imm_switch_yes->SetPosition(setting_comp_x + setting_title_rect.w + setting_comp_margin, setting_title_text_rect.y);
				this->imm_switch_yes->Render();
			}
			else {
				this->imm_switch_no->SetPosition(setting_comp_x + setting_title_rect.w + setting_comp_margin, setting_title_text_rect.y);
				this->imm_switch_no->Render();
			}
			setting_title_text_rect.y += setting_title_text_rect.h + setting_comp_margin;


			setting_title_text_rect.y += setting_comp_margin;
			// Details
			switch (unit->Type()) {
			case Motion::Unit::POSITION:
				SDL_QueryTexture(this->pos_x_text, NULL, NULL, &setting_title_text_rect.w, &setting_title_text_rect.h);
				SDL_RenderCopy(this->win->GetRenderer(), this->pos_x_text, NULL, &setting_title_text_rect);
				this->value_input_a->SetValue(((Motion::Position*)unit)->x);
				input_rect = this->value_input_a->GetRect();
				input_rect.w = setting_comp_width - setting_title_rect.w - setting_comp_margin * 2;
				input_rect.x = setting_comp_x + setting_title_rect.w + setting_comp_margin;
				input_rect.y = setting_title_text_rect.y;
				this->value_input_a->SetRect(input_rect);
				this->value_input_a->Render();
				setting_title_text_rect.y += setting_title_text_rect.h + setting_comp_margin;
				
				SDL_QueryTexture(this->pos_y_text, NULL, NULL, &setting_title_text_rect.w, &setting_title_text_rect.h);
				SDL_RenderCopy(this->win->GetRenderer(), this->pos_y_text, NULL, &setting_title_text_rect);
				this->value_input_b->SetValue(((Motion::Position*)unit)->y);
				input_rect = this->value_input_b->GetRect();
				input_rect.w = setting_comp_width - setting_title_rect.w - setting_comp_margin * 2;
				input_rect.x = setting_comp_x + setting_title_rect.w + setting_comp_margin;
				input_rect.y = setting_title_text_rect.y;
				this->value_input_b->SetRect(input_rect);
				this->value_input_b->Render();
				setting_title_text_rect.y += setting_title_text_rect.h + setting_comp_margin;
				break;
			case Motion::Unit::ROTATION:
				SDL_QueryTexture(this->rot_deg_text, NULL, NULL, &setting_title_text_rect.w, &setting_title_text_rect.h);
				SDL_RenderCopy(this->win->GetRenderer(), this->rot_deg_text, NULL, &setting_title_text_rect);
				this->value_input_a->SetValue(((Motion::Rotation*)unit)->degree);
				input_rect = this->value_input_a->GetRect();
				input_rect.w = setting_comp_width - setting_title_rect.w - setting_comp_margin * 2;
				input_rect.x = setting_comp_x + setting_title_rect.w + setting_comp_margin;
				input_rect.y = setting_title_text_rect.y;
				this->value_input_a->SetRect(input_rect);
				this->value_input_a->Render();
				setting_title_text_rect.y += setting_title_text_rect.h + setting_comp_margin;
				break;
			case Motion::Unit::SCALE:
				SDL_QueryTexture(this->scale_x_text, NULL, NULL, &setting_title_text_rect.w, &setting_title_text_rect.h);
				SDL_RenderCopy(this->win->GetRenderer(), this->scale_x_text, NULL, &setting_title_text_rect);
				this->value_input_a->SetValue(((Motion::Scale*)unit)->x);
				input_rect = this->value_input_a->GetRect();
				input_rect.w = setting_comp_width - setting_title_rect.w - setting_comp_margin * 2;
				input_rect.x = setting_comp_x + setting_title_rect.w + setting_comp_margin;
				input_rect.y = setting_title_text_rect.y;
				this->value_input_a->SetRect(input_rect);
				this->value_input_a->Render();
				setting_title_text_rect.y += setting_title_text_rect.h + setting_comp_margin;

				SDL_QueryTexture(this->scale_y_text, NULL, NULL, &setting_title_text_rect.w, &setting_title_text_rect.h);
				SDL_RenderCopy(this->win->GetRenderer(), this->scale_y_text, NULL, &setting_title_text_rect);
				this->value_input_b->SetValue(((Motion::Scale*)unit)->y);
				input_rect = this->value_input_b->GetRect();
				input_rect.w = setting_comp_width - setting_title_rect.w - setting_comp_margin * 2;
				input_rect.x = setting_comp_x + setting_title_rect.w + setting_comp_margin;
				input_rect.y = setting_title_text_rect.y;
				this->value_input_b->SetRect(input_rect);
				this->value_input_b->Render();
				setting_title_text_rect.y += setting_title_text_rect.h + setting_comp_margin;
				break;
			case Motion::Unit::OPACITY:
				SDL_QueryTexture(this->opacity_text, NULL, NULL, &setting_title_text_rect.w, &setting_title_text_rect.h);
				SDL_RenderCopy(this->win->GetRenderer(), this->opacity_text, NULL, &setting_title_text_rect);
				this->value_input_a->SetValue(((Motion::Opacity*)unit)->opacity);
				input_rect = this->value_input_a->GetRect();
				input_rect.w = setting_comp_width - setting_title_rect.w - setting_comp_margin * 2;
				input_rect.x = setting_comp_x + setting_title_rect.w + setting_comp_margin;
				input_rect.y = setting_title_text_rect.y;
				this->value_input_a->SetRect(input_rect);
				this->value_input_a->Render();
				setting_title_text_rect.y += setting_title_text_rect.h + setting_comp_margin;
				break;
			}
			SDL_QueryTexture(this->transition_text, NULL, NULL, &setting_title_text_rect.w, &setting_title_text_rect.h);
			SDL_RenderCopy(this->win->GetRenderer(), this->transition_text, NULL, &setting_title_text_rect);
			setting_title_text_rect.y += setting_title_text_rect.h + setting_comp_margin;
		}
	}
}

void SettingComponent::EventProcess(SDL_Event* evt) {
	Motion* motion = MotionStatus::CurrentMotion();
	if (motion) {
		Motion::Unit* unit = motion->current_motion_unit;
		if (unit) {
			// Frame Settings
			if (unit->immediate) {
				this->frame_input_a->EventProcess(evt);
			}
			else {
				this->frame_input_a->EventProcess(evt);
				this->frame_input_b->EventProcess(evt);
			}
			if (unit->immediate) {
				this->imm_switch_yes->EventProcess(evt);
				if (this->imm_switch_yes->IsClicked()) unit->SetImmediate(false);
			}
			else {
				this->imm_switch_no->EventProcess(evt);
				if (this->imm_switch_no->IsClicked()) unit->SetImmediate(true);
			}

			// Details
			switch (unit->Type()) {
			case Motion::Unit::POSITION:
				this->value_input_a->EventProcess(evt);
				this->value_input_b->EventProcess(evt);
				break;
			case Motion::Unit::ROTATION:
				this->value_input_a->EventProcess(evt);
				break;
			case Motion::Unit::SCALE:
				this->value_input_a->EventProcess(evt);
				this->value_input_b->EventProcess(evt);
				break;
			case Motion::Unit::OPACITY:
				this->value_input_a->EventProcess(evt);
				break;
			}
		}
	}
}
