#include "UI_Window.h"
#include <SDL_syswm.h>
#include <dwmapi.h>
#include <Windows.h>

#pragma comment(lib, "dwmapi.lib")

UI_Window::UI_Window() {
	this->win = SDL_CreateWindow("ADOFAI Typographer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_HIDDEN | SDL_WINDOW_RESIZABLE);
	this->ren = SDL_CreateRenderer(this->win, -1, SDL_RENDERER_ACCELERATED);
	//SDL_SetWindowBordered(this->win, SDL_FALSE);
	SDL_SetWindowMinimumSize(this->win, 960, 540);
	SDL_SysWMinfo wmInfo;
	SDL_VERSION(&wmInfo.version);
	SDL_GetWindowWMInfo(this->win, &wmInfo);
	HWND hwnd = wmInfo.info.win.window;

	COLORREF DARK_COLOR = 0x00202020;
	//COLORREF BORDER_COLOR = 0x00ff9e6e; //BLUE
	COLORREF BORDER_COLOR = 0x00505050; //BLACK

	DwmSetWindowAttribute(hwnd, DWMWINDOWATTRIBUTE::DWMWA_CAPTION_COLOR, &DARK_COLOR, sizeof(DARK_COLOR));
	DwmSetWindowAttribute(hwnd, DWMWINDOWATTRIBUTE::DWMWA_BORDER_COLOR, &BORDER_COLOR, sizeof(DARK_COLOR));
	
	SDL_SetRenderDrawBlendMode(this->ren, SDL_BLENDMODE_BLEND);

	SDL_ShowWindow(this->win);
}

UI_Window::~UI_Window() {
	SDL_DestroyRenderer(this->ren);
	SDL_DestroyWindow(this->win);
}

SDL_Renderer* UI_Window::GetRenderer() {
	return this->ren;
}

int UI_Window::GetWidth() {
	int w;
	SDL_GetWindowSize(this->win, &w, NULL);
	return w;
}

int UI_Window::GetHeight() {
	int h;
	SDL_GetWindowSize(this->win, NULL, &h);
	return h;
}
