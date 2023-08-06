#include "Button.h"
#include "Texture.h"
#include "Log.h"

Button::Button(string str, SDL_Renderer* ren, int size, int x, int y, bool centered) {
	this->ren = ren;
	this->texture = Texture::LoadText(str.c_str(), ren, size, "main", Color::RGB(CLR_STD_TEXT));
	SDL_QueryTexture(this->texture, NULL, NULL, &this->rect.w, &this->rect.h);
	this->status = BTN_NORMAL;
	this->is_clicked = false;
	this->is_enabled = true;
	this->rect.x = x;
	this->rect.y = y;
	if (centered) {
		this->rect.x = x - this->rect.w / 2;
		this->rect.y = y - this->rect.h / 2;
	}
}

Button::~Button() {
	SDL_DestroyTexture(this->texture);
}

void Button::SetEnabled(bool enabled) {
	this->is_enabled = enabled;
}

bool Button::IsHovered() {
	return this->status == BTN_HOVERED;
}

bool Button::IsClicked() {
	bool ret = this->is_clicked;
	this->is_clicked = false;
	return ret;
}

SDL_Rect Button::GetRect() {
	return this->rect;
}

void Button::SetRect(SDL_Rect rect) {
	this->rect = rect;
}

void Button::SetPositionCentered(int x, int y) {
	this->rect.x = x - this->rect.w / 2;
	this->rect.y = y - this->rect.h / 2;
}

void Button::SetPosition(int x, int y) {
	this->rect.x = x;
	this->rect.y = y;
}

void Button::Render() {
	SDL_Rect text_rect;
	text_rect = this->rect;
	if (this->is_enabled) {
		switch (this->status) {
			case BTN_NORMAL:
				SDL_SetTextureAlphaMod(this->texture, 255);
				SDL_RenderCopy(this->ren, this->texture, NULL, &text_rect);
				break;
			case BTN_HOVERED:
				SDL_SetTextureAlphaMod(this->texture, 200);
				SDL_RenderCopy(this->ren, this->texture, NULL, &text_rect);
				break;
			case BTN_PRESSED:
				SDL_SetTextureAlphaMod(this->texture, 100);
				SDL_RenderCopy(this->ren, this->texture, NULL, &text_rect);
				break;
		}
	}
	else {
		SDL_SetTextureAlphaMod(this->texture, 100);
		SDL_RenderCopy(this->ren, this->texture, NULL, &text_rect);
	}
}

void Button::EventProcess(SDL_Event* evt) {
	if (is_enabled) {
		int mx, my;
		mx = evt->motion.x;
		my = evt->motion.y;

		if (evt->type == SDL_MOUSEBUTTONDOWN) {
			if (mx >= rect.x && mx <= rect.x + rect.w && my >= rect.y && my <= rect.y + rect.h) {
				if (status == BTN_HOVERED) status = BTN_PRESSED;
				//return true;
			}
		}
		else if (evt->type == SDL_MOUSEBUTTONUP) {
			if (mx >= rect.x && mx <= rect.x + rect.w && my >= rect.y && my <= rect.y + rect.h) {
				if(status == BTN_PRESSED) this->is_clicked = true;
				status = BTN_HOVERED;
				//return true;
			}
		}
		else if (evt->type == SDL_MOUSEMOTION) {
			if (mx >= rect.x && mx <= rect.x + rect.w && my >= rect.y && my <= rect.y + rect.h) {
				if (status == BTN_NORMAL) status = BTN_HOVERED;
			}
			else status = BTN_NORMAL;
			//return true;
		}
	}
	//return false;
}
