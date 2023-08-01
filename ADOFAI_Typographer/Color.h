#pragma once
#include <SDL.h>

#define CLR_STD_BACKGROUND  24, 24, 24, 255
#define CLR_DARK_BACKGROUND 8, 8, 8, 255
#define CLR_STD_TEXT		250, 250, 250, 255
#define CLR_SUB_TEXT		170, 170, 170, 255
#define CLR_COMP_BACKGROUND 32, 32, 32, 255
#define CLR_COMP_BACKGROUND_BRIGHT 96, 96, 96, 255
#define CLR_SCREEN_BACKGROUND 18, 18, 18, 255
#define CLR_STD_BORDER		48, 48, 48, 255
#define CLR_WINDOW_THEME	Color::WindowsSTD::GetThemeColor().r, Color::WindowsSTD::GetThemeColor().g, Color::WindowsSTD::GetThemeColor().b, 255

namespace Color {
	
	class RGB;
	class HSV;

	class RGB {
	public:
		RGB(int r, int g, int b, int a = 0);
		RGB(const RGB &rgb);
		RGB(HSV &hsv);
		RGB(SDL_Color &sdl);
		SDL_Color to_SDL();
		HSV to_HSV();

		int r;
		int g;
		int b;
	};

	class HSV {
	public:
		HSV(int h, int s, int v);
		HSV(const HSV& hsv);
		HSV(RGB& rgb);
		HSV(SDL_Color& sdl);
		SDL_Color to_SDL();
		RGB to_RGB();

		int h;
		int s;
		int v;
	};

	class WindowsSTD {
	private:
		static RGB theme_color;

	public:
		static void Init();
		static RGB GetThemeColor();
	};
}

