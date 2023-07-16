#pragma once
#include <vector>
#include <string>
#include <SDL.h>
#include "Color.h"
using namespace std;

class Texture {
public:
	static void DrawPoint(SDL_Point point, SDL_Renderer* renderer, int circle_size = 15, int r = 255, int g = 255, int b = 255, int a = 255);
	static void DrawLine(SDL_Point begin, SDL_Point end, SDL_Renderer* renderer, int circle_size = 15, int r = 255, int g = 255, int b = 255, int a = 255);
	static SDL_Texture* LoadImage(const char* filename, SDL_Renderer* renderer);
	static SDL_Texture* LoadText(const char* str, SDL_Renderer* renderer, int size, const char* fontfile_name, Color::RGB color, int wrap = 0);

	static void InitLoadTextureLibrary(SDL_Renderer* renderer);
	static void QuitLoadTextureLibrary();
};