#pragma once
#include <SDL.h>
#include <string>
using namespace std;
class Button {
private:
	enum ButtonStatus {
		BTN_NORMAL,
		BTN_HOVERED,
		BTN_PRESSED
	};
	SDL_Texture* texture;
	SDL_Rect rect;
	ButtonStatus status;
	bool is_enabled;
	bool is_clicked;
	SDL_Renderer* ren;

public:
	Button(string str, SDL_Renderer* ren, int size, int x, int y, bool centered = false);
	~Button();

	void SetEnabled(bool enabled);

	bool IsHovered();
	bool IsClicked();
	SDL_Rect GetRect();

	void SetRect(SDL_Rect rect);

	void SetPositionCentered(int x, int y);
	void SetPosition(int x, int y);

	void Render();
	void EventProcess(SDL_Event* evt);
};

