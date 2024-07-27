#include "NumberInput.h"
#include "Texture.h"
#include <codecvt>
using namespace std;

bool NumberInput::StringCheck(string str) {
	if (str.size() == 0) return false;
	if (str[0] == '-' && this->type != SIGNED_INT && this->type != SIGNED_FLOAT) {
		return false;
	}
	bool dot = false;
	for (int i = 0; i < str.size(); i++) {
		if (this->type == SIGNED_FLOAT || this->type == UNSIGNED_FLOAT) {
			if (str[i] == '.') {
				if (dot) return false;
				dot = true;
				continue;
			}
		}
		if (str[i] < '0' || str[i] > '9') return false;
	}
	return true;
}

std::string NumberInput::FormatedInput() {
	std::string str = this->input;
	if (str.empty()) {
		str = "0";
	}
	if (str.front() == '.') {
		str = "0" + str;
	}
	if (str.back() == '.') {
		str.pop_back();
	}
	return str;
}

NumberInput::NumberInput(double init_value, SDL_Renderer* ren, int size, int x, int y, InputType type) {
	this->is_hovered = false;
	this->is_pressed = false;

	this->status = Normal;
	this->input = std::to_string(init_value);
	this->min = 0;
	this->max = 10000;
	this->is_enabled = true;
	this->ren = ren;
	this->rect.x = x;
	this->rect.y = y;
	this->rect.w = 0;
	this->type = type;

	wstring_convert<codecvt_utf8<wchar_t>> myconv;
	wstring wstr = myconv.from_bytes(this->input);

	for (int i = 0; i < wstr.size(); i++) {
		this->input_texture.push_back(Texture::LoadText(myconv.to_bytes(wstr[i]).c_str(), ren, size, "main", Color::RGB(CLR_STD_TEXT)));
		int w;
		SDL_QueryTexture(this->input_texture.back(), NULL, NULL, &w, &this->rect.h);
		this->rect.w += w;
	}

	this->cursor_pos = this->input.size();

	this->size = size;
}

NumberInput::~NumberInput() {
	for (int i = 0; i < this->input_texture.size(); i++) {
		SDL_DestroyTexture(this->input_texture[i]);
	}
	this->input_texture.clear();
}

NumberInput& NumberInput::SetMin(double min) {
	this->min = min;
	return *this;
}

NumberInput& NumberInput::SetMax(double max) {
	this->max = max;
	return *this;
}

void NumberInput::SetEnabled(bool enabled) {
	this->is_enabled = enabled;
}

SDL_Rect NumberInput::GetRect() {
	return this->rect;
}

void NumberInput::SetRect(SDL_Rect rect) {
	this->rect = rect;
}

void NumberInput::SetPosition(int x, int y) {
	this->rect.x = x;
	this->rect.y = y;
}

void NumberInput::SetType(InputType type) {
	this->type = type;
}

void NumberInput::Render() {
	int texture_length = 0;
	for (int i = 0; i < this->input_texture.size(); i++) {
		int w;
		SDL_QueryTexture(this->input_texture[i], nullptr, nullptr, &w, nullptr);
		texture_length += w;
	}

	int indent = 0;
	SDL_Rect rect;
	for (int i = 0; i < this->input_texture.size(); i++) {
		SDL_QueryTexture(this->input_texture[i], nullptr, nullptr, &rect.w, &rect.h);
		rect.x = this->rect.x + indent;
		rect.y = this->rect.y;
		indent += rect.w;
		if (!this->is_enabled) {
			SDL_SetTextureAlphaMod(this->input_texture[i], 100);
		}
		else if (this->is_pressed) {
			SDL_SetTextureAlphaMod(this->input_texture[i], 100);
		}
		else if (this->is_hovered) {
			SDL_SetTextureAlphaMod(this->input_texture[i], 200);
		}
		else {
			SDL_SetTextureAlphaMod(this->input_texture[i], 255);
		}
		SDL_RenderCopy(this->ren, this->input_texture[i], nullptr, &rect);
	}

	if (this->status == Editing) {
		rect = this->rect;
		rect.w = 3;
		rect.h = this->size * 0.8;
		for (int i = 0; i < this->cursor_pos; i++) {
			int x;
			SDL_QueryTexture(this->input_texture[i], nullptr, nullptr, &x, nullptr);
			rect.x += x;
		}
		SDL_SetRenderDrawColor(this->ren, CLR_STD_TEXT);
		SDL_RenderFillRect(this->ren, &rect);
	}
}

void num_pop_utf8(std::string& utf8, int pos) {
	if (utf8.empty())
		return;
	wstring_convert<codecvt_utf8<wchar_t>> myconv;
	wstring wstr = myconv.from_bytes(utf8);
	wstr.erase(wstr.begin() + pos);
	utf8 = myconv.to_bytes(wstr);
}

void num_insert_utf8(std::string& utf8, std::string text, int pos) {
	wstring_convert<codecvt_utf8<wchar_t>> myconv;
	wstring wstr = myconv.from_bytes(utf8);
	wstring wins = myconv.from_bytes(text);
	wstr.insert(wstr.begin() + pos, wins.begin(), wins.end());
	utf8 = myconv.to_bytes(wstr);
}

void NumberInput::EventProcess(SDL_Event* evt) {
	if (this->is_enabled) {
		if (evt->type == SDL_MOUSEMOTION) {
			if (evt->motion.x >= this->rect.x && evt->motion.x <= this->rect.x + this->rect.w &&
				evt->motion.y >= this->rect.y && evt->motion.y <= this->rect.y + this->rect.h) {
				this->is_hovered = true;
			}
			else {
				this->is_hovered = false;
			}
		}
		else if (evt->type == SDL_MOUSEBUTTONDOWN) {
			if (evt->button.button == SDL_BUTTON_LEFT) {
				if (evt->button.x >= this->rect.x && evt->button.x <= this->rect.x + this->rect.w &&
					evt->button.y >= this->rect.y && evt->button.y <= this->rect.y + this->rect.h) {
					this->is_pressed = true;
				}
				else {
					this->is_pressed = false;
					this->status = Normal;
					SetValue(this->FormatedInput());
				}
			}
		}
		else if (evt->type == SDL_MOUSEBUTTONUP) {
			if (evt->button.button == SDL_BUTTON_LEFT) {
				if (this->is_pressed && evt->button.x >= this->rect.x && evt->button.x <= this->rect.x + this->rect.w &&
					evt->button.y >= this->rect.y && evt->button.y <= this->rect.y + this->rect.h) {
					this->is_pressed = false;
					this->status = Editing;
				}
				else {
					this->is_pressed = false;
					this->status = Normal;
					SetValue(this->FormatedInput());
				}
			}
			else {
				this->is_pressed = false;
				this->status = Normal;
				SetValue(this->FormatedInput());
			}
		}

		if (this->status == Editing) {
			if (evt->type == SDL_KEYDOWN) {
				if (evt->key.keysym.sym == SDLK_BACKSPACE && this->input.size() > 0) {
					if (!this->input.empty() && this->cursor_pos) {
						this->cursor_pos--;
						num_pop_utf8(this->input, this->cursor_pos);
						SDL_DestroyTexture(this->input_texture[this->cursor_pos]);
						this->input_texture.erase(this->input_texture.begin() + this->cursor_pos);
					}
				}
				else if (evt->key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL) {
					SDL_SetClipboardText(this->input.c_str());
				}
				else if (evt->key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL) {
					string clip = SDL_GetClipboardText();
					string prev = this->input;
					this->input.insert(this->input.begin() + this->cursor_pos, clip.begin(), clip.end());
					if (this->StringCheck(this->input)) {
						for (int i = 0; i < clip.size(); i++) {
							SDL_Texture* t = Texture::LoadText(clip[i] + "", this->ren, this->size, "main", Color::RGB(CLR_STD_TEXT));
							this->input_texture.insert(this->input_texture.begin() + this->cursor_pos + i, t);
						}
						this->cursor_pos += clip.size();
					}
					else {
						this->input = prev;
					}
				}
				else if (evt->key.keysym.sym == SDLK_LEFT && this->cursor_pos > 0) {
					this->cursor_pos--;
				}
				else if (evt->key.keysym.sym == SDLK_RIGHT && this->cursor_pos < this->input_texture.size()) {
					this->cursor_pos++;
				}
			}
			else if (evt->type == SDL_TEXTINPUT) {
				if (!(SDL_GetModState() & KMOD_CTRL && (evt->text.text[0] == 'c' || evt->text.text[0] == 'C' || evt->text.text[0] == 'v' || evt->text.text[0] == 'V'))) {
					string s = evt->text.text;
					string prev = this->input;
					num_insert_utf8(this->input, s, this->cursor_pos);
					if (this->StringCheck(this->input)) {
						SDL_Texture* t = Texture::LoadText(s.c_str(), this->ren, this->size, "main", Color::RGB(CLR_STD_TEXT));
						this->input_texture.insert(this->input_texture.begin() + this->cursor_pos, t);
						this->cursor_pos++;
					}
					else {
						this->input = prev;
					}
				}
			}
		}
	}
}

void NumberInput::SetValue(double value) {
	this->SetValue(to_string(value));
}

void NumberInput::SetValue(int value) {
	this->SetValue(to_string(value));
}

void NumberInput::SetValue(std::string value) {
	if (this->StringCheck(value) && this->input != value) {
		this->input = value;
		if(this->cursor_pos >= this->input.size()) this->cursor_pos = this->input.size();
		for (int i = 0; i < this->input_texture.size(); i++) {
			SDL_DestroyTexture(this->input_texture[i]);
		}
		this->input_texture.clear();

		wstring_convert<codecvt_utf8<wchar_t>> myconv;
		wstring wstr = myconv.from_bytes(this->input);

		for (int i = 0; i < wstr.size(); i++) {
			this->input_texture.push_back(Texture::LoadText(myconv.to_bytes(wstr[i]).c_str(), ren, size, "main", Color::RGB(CLR_STD_TEXT)));
		}
	}
}

double NumberInput::GetValue() {
	return stod(this->FormatedInput());
}
