#pragma once
#include <SDL.h>

class UI_Window {
private:
	SDL_Window* win;
	SDL_Renderer* ren;

public:
	UI_Window();
	~UI_Window();

	SDL_Renderer* GetRenderer();
	int GetWidth();
	int GetHeight();
};

