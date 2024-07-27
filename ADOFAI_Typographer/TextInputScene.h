#pragma once
#include "Scene.h"
#include <string>
#include <vector>
#include "UI_Element.h"
using namespace std;

class TextInputScene : public Scene {
private:
	SDL_Texture* title;
	SDL_Texture* explanation;

	vector<SDL_Texture*> input_texture;
	SDL_Texture* editing_texture;

	Button *btn_ok, *btn_cancel;

	string* text;
	string input;

	int begin_time;

	int cursor_pos;

public:
	TextInputScene(UI_Window* win, string title, string explanation, string* text);
	~TextInputScene();

	void Rendering() override;
	void EventProcess(SDL_Event* evt) override;

	Scene* GetNextScene() override;
};

