#include "TextInputScene.h"
#include "Texture.h"
#include "Log.h"
#include "Status.h"
#include <codecvt>

#define TEXTINPUT_MARGIN 30

#define INPUT_TEXT_SIZE 50

TextInputScene::TextInputScene(UI_Window* win, string title, string explanation, string* text) : Scene(win) {
	Log::Debug("TextInputScene", "Constructor", " : Created");
	
	this->title = Texture::LoadText(title.c_str(), this->win->GetRenderer(), 50, "main", Color::RGB(CLR_STD_TEXT));
	this->explanation = Texture::LoadText(explanation.c_str(), this->win->GetRenderer(), 30, "main", Color::RGB(CLR_SUB_TEXT));
	
	this->btn_ok = new Button("OK", this->win->GetRenderer(), 30, 0, 0);
	this->btn_cancel = new Button("Cancel", this->win->GetRenderer(), 30, 0, 0);

	SDL_Rect rect;

	rect.w = 960;
	rect.h = 540;
	rect.x = (this->win->GetWidth() - rect.w) / 2;
	rect.y = (this->win->GetHeight() - rect.h) / 2;

	this->btn_ok->SetPosition(rect.x + rect.w - this->btn_ok->GetRect().w - this->btn_cancel->GetRect().h - TEXTINPUT_MARGIN * 2, rect.y + rect.h - this->btn_ok->GetRect().h - TEXTINPUT_MARGIN / 2);
	this->btn_cancel->SetPosition(rect.x + rect.w - this->btn_cancel->GetRect().w - TEXTINPUT_MARGIN, rect.y + rect.h - this->btn_cancel->GetRect().h - TEXTINPUT_MARGIN / 2);

	this->text = text;
	this->input = *text;

	editing_texture = nullptr;

	wstring_convert<codecvt_utf8<wchar_t>> myconv;
	wstring wstr = myconv.from_bytes(this->input);

	for (int i = 0; i < wstr.size(); i++) {
		this->input_texture.push_back(Texture::LoadText(myconv.to_bytes(wstr[i]).c_str(), this->win->GetRenderer(), INPUT_TEXT_SIZE, "main", Color::RGB(CLR_STD_TEXT)));
	}
	
	this->begin_time = clock();

	this->cursor_pos = this->input.size();

	Status::RenderRegularly(2);
}

TextInputScene::~TextInputScene() {
	Log::Debug("TextInputScene", "Destructor", " : Deleted");

	for (int i = 0; i < this->input_texture.size(); i++) {
		SDL_DestroyTexture(this->input_texture[i]);
	}
	this->input_texture.clear();
	SDL_DestroyTexture(this->title);
	SDL_DestroyTexture(this->explanation);

	Status::RenderOnlyEventOccured();
}

void TextInputScene::Rendering() {
	if (this->input.empty()) this->btn_ok->SetEnabled(false);
	else this->btn_ok->SetEnabled(true);

	SDL_Rect rect;
	rect.x = 0;
	rect.y = 0;
	rect.w = this->win->GetWidth();
	rect.h = this->win->GetHeight();

	Color::RGB rgb(CLR_STD_BACKGROUND);

	SDL_SetRenderDrawColor(this->win->GetRenderer(), rgb.r, rgb.g, rgb.b, 150);
	SDL_RenderFillRect(this->win->GetRenderer(), &rect);

	rect.w = 640;
	rect.h = 360;
	rect.x = (this->win->GetWidth() - rect.w) / 2;
	rect.y = (this->win->GetHeight() - rect.h) / 2;

	SDL_SetRenderDrawColor(this->win->GetRenderer(), CLR_STD_BACKGROUND);
	SDL_RenderFillRect(this->win->GetRenderer(), &rect);
	SDL_SetRenderDrawColor(this->win->GetRenderer(), CLR_STD_BORDER);
	SDL_RenderDrawRect(this->win->GetRenderer(), &rect);

	this->btn_ok->SetPosition(rect.x + rect.w - this->btn_ok->GetRect().w - this->btn_cancel->GetRect().h - TEXTINPUT_MARGIN * 2, rect.y + rect.h - this->btn_ok->GetRect().h - TEXTINPUT_MARGIN / 2);
	this->btn_cancel->SetPosition(rect.x + rect.w - this->btn_cancel->GetRect().w - TEXTINPUT_MARGIN, rect.y + rect.h - this->btn_cancel->GetRect().h - TEXTINPUT_MARGIN / 2);

	this->btn_ok->Render();
	this->btn_cancel->Render();

	int area_y = rect.y;
	SDL_QueryTexture(this->title, nullptr, nullptr, &rect.w, &rect.h);
	rect.x = (this->win->GetWidth() - rect.w) / 2;
	rect.y = (area_y + 360 * 0.3 - rect.h / 2);
	SDL_RenderCopy(this->win->GetRenderer(), this->title, nullptr, &rect);

	SDL_QueryTexture(this->explanation, nullptr, nullptr, &rect.w, &rect.h);
	rect.x = (this->win->GetWidth() - rect.w) / 2;
	rect.y = (area_y + 360 * 0.4 - rect.h / 2);
	SDL_RenderCopy(this->win->GetRenderer(), this->explanation, nullptr, &rect);

	int texture_length = 0;
	for (int i = 0; i < this->input_texture.size(); i++) {
		int w;
		SDL_QueryTexture(this->input_texture[i], nullptr, nullptr, &w, nullptr);
		texture_length += w;
	}
	if (this->editing_texture) {
		int w;
		SDL_QueryTexture(this->editing_texture, nullptr, nullptr, &w, nullptr);
		texture_length += w;
	}

	int indent = 0;
	for (int i = 0; i < this->input_texture.size(); i++) {
		if (i == this->cursor_pos && this->editing_texture) {
			SDL_QueryTexture(this->editing_texture, nullptr, nullptr, &rect.w, &rect.h);
			rect.x = (this->win->GetWidth() - texture_length) / 2 + indent;
			rect.y = (area_y + 360 * 0.6 - rect.h / 2);
			indent += rect.w;
			SDL_RenderCopy(this->win->GetRenderer(), this->editing_texture, nullptr, &rect);
		}
		SDL_QueryTexture(this->input_texture[i], nullptr, nullptr, &rect.w, &rect.h);
		rect.x = (this->win->GetWidth() - texture_length) / 2 + indent;
		rect.y = (area_y + 360 * 0.6 - rect.h / 2);
		indent += rect.w;
		SDL_RenderCopy(this->win->GetRenderer(), this->input_texture[i], nullptr, &rect);
	}
	if (this->input_texture.size() == this->cursor_pos && this->editing_texture) {
		SDL_QueryTexture(this->editing_texture, nullptr, nullptr, &rect.w, &rect.h);
		rect.x = (this->win->GetWidth() - texture_length) / 2 + indent;
		rect.y = (area_y + 360 * 0.6 - rect.h / 2);
		indent += rect.w;
		SDL_RenderCopy(this->win->GetRenderer(), this->editing_texture, nullptr, &rect);
	}

	if (((clock() - this->begin_time) / 500) % 2 == 0) {
		rect.w = 3;
		rect.h = INPUT_TEXT_SIZE * 0.8;
		rect.x = 0;
		rect.y = (area_y + 360 * 0.6 - rect.h / 2);
		for (int i = 0; i < this->cursor_pos; i++) {
			int x;
			SDL_QueryTexture(this->input_texture[i], nullptr, nullptr, &x, nullptr);
			rect.x += x;
		}
		if (this->editing_texture) {
			int x;
			SDL_QueryTexture(this->editing_texture, nullptr, nullptr, &x, nullptr);
			rect.x += x;
		}
		rect.x = (this->win->GetWidth() - texture_length) / 2 + rect.x;
		SDL_SetRenderDrawColor(this->win->GetRenderer(), CLR_STD_TEXT);
		SDL_RenderFillRect(this->win->GetRenderer(), &rect);
	}
}

void pop_utf8(std::string& utf8, int pos) {
	if (utf8.empty())
		return;
	wstring_convert<codecvt_utf8<wchar_t>> myconv;
	wstring wstr = myconv.from_bytes(utf8);
	wstr.erase(wstr.begin() + pos);
	utf8 = myconv.to_bytes(wstr);
}

void insert_utf8(std::string& utf8, std::string text, int pos) {
	wstring_convert<codecvt_utf8<wchar_t>> myconv;
	wstring wstr = myconv.from_bytes(utf8);
	wstring wins = myconv.from_bytes(text);
	wstr.insert(wstr.begin() + pos, wins.begin(), wins.end());
	utf8 = myconv.to_bytes(wstr);
}

void TextInputScene::EventProcess(SDL_Event* evt) {
	this->btn_ok->EventProcess(evt);
	this->btn_cancel->EventProcess(evt);

	if (this->btn_ok->IsClicked() || (evt->type == SDL_KEYDOWN && evt->key.keysym.sym == SDLK_RETURN)) {
		this->text->assign(this->input);
		this->finish = true;
	}
	else if (this->btn_cancel->IsClicked() || (evt->type == SDL_KEYDOWN && evt->key.keysym.sym == SDLK_ESCAPE)) {
		this->finish = true;
	}

	if (evt->type == SDL_KEYDOWN) {
		this->begin_time = clock();
		if (evt->key.keysym.sym == SDLK_BACKSPACE && this->input.size() > 0) {
			if (!this->input.empty() && this->cursor_pos) {
				this->cursor_pos--;
				pop_utf8(this->input, this->cursor_pos);
				SDL_DestroyTexture(this->input_texture[this->cursor_pos]);
				this->input_texture.erase(this->input_texture.begin() + this->cursor_pos);
			}
		}
		else if (evt->key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL) {
			SDL_SetClipboardText(this->input.c_str());
		}
		else if (evt->key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL) {
			string clip = SDL_GetClipboardText();
			this->input.insert(this->input.begin() + this->cursor_pos, clip.begin(), clip.end());
			for (int i = 0; i < clip.size(); i++) {
				SDL_Texture* t = Texture::LoadText(clip[i] + "", this->win->GetRenderer(), INPUT_TEXT_SIZE, "main", Color::RGB(CLR_STD_TEXT));
				this->input_texture.insert(this->input_texture.begin() + this->cursor_pos + i, t);
			}
			this->cursor_pos += clip.size();
		}
		else if (evt->key.keysym.sym == SDLK_LEFT && this->cursor_pos > 0) {
			this->cursor_pos--;
		}
		else if (evt->key.keysym.sym == SDLK_RIGHT && this->cursor_pos < this->input_texture.size()) {
			this->cursor_pos++;
		}
	}
	else if (evt->type == SDL_TEXTINPUT) {
		this->begin_time = clock();
		if (!(SDL_GetModState() & KMOD_CTRL && (evt->text.text[0] == 'c' || evt->text.text[0] == 'C' || evt->text.text[0] == 'v' || evt->text.text[0] == 'V'))) {
			string s = evt->text.text;
			insert_utf8(this->input, s, this->cursor_pos);
			SDL_Texture* t = Texture::LoadText(s.c_str(), this->win->GetRenderer(), INPUT_TEXT_SIZE, "main", Color::RGB(CLR_STD_TEXT));
			this->input_texture.insert(this->input_texture.begin() + this->cursor_pos, t);
			this->cursor_pos++;
		}
	}
	else if (evt->type == SDL_TEXTEDITING) {
		this->begin_time = clock();
		if (!(SDL_GetModState() & KMOD_CTRL && (evt->text.text[0] == 'c' || evt->text.text[0] == 'C' || evt->text.text[0] == 'v' || evt->text.text[0] == 'V'))) {
			string s = evt->text.text;
			if (this->editing_texture) {
				SDL_DestroyTexture(this->editing_texture);
				this->editing_texture = nullptr;
			}
			if(!s.empty()) this->editing_texture = Texture::LoadText(s.c_str(), this->win->GetRenderer(), INPUT_TEXT_SIZE, "main", Color::RGB(CLR_STD_TEXT));
		}
	}
}

Scene* TextInputScene::GetNextScene() {
	return nullptr;
}
