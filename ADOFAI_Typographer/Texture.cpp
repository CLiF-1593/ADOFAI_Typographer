#include "Texture.h"
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <iostream>
#include "Log.h"
#include "FileIO.h"

SDL_Texture *circle;

void Texture::InitLoadTextureLibrary(SDL_Renderer *renderer) {
	circle = LoadImage(FileIO::Location("circle.img").c_str(), renderer);
	SDL_SetTextureBlendMode(circle, SDL_BLENDMODE_BLEND);
}

void Texture::QuitLoadTextureLibrary() {
	SDL_DestroyTexture(circle);
}

void Texture::DrawPoint(SDL_Point point, SDL_Renderer * renderer, int circle_size, int r, int g, int b, int a) {
	SDL_SetTextureColorMod(circle, r, g, b);
	SDL_Rect dst;
	dst.w = circle_size;
	dst.h = circle_size;
	dst.x = point.x - circle_size / 2;
	dst.y = point.y - circle_size / 2;
	SDL_SetTextureBlendMode(circle, SDL_BLENDMODE_ADD);
	SDL_SetTextureAlphaMod(circle, a / 2);
	SDL_RenderCopy(renderer, circle, NULL, &dst);
	SDL_SetTextureBlendMode(circle, SDL_BLENDMODE_BLEND);
	SDL_SetTextureAlphaMod(circle, a);
	SDL_RenderCopy(renderer, circle, NULL, &dst);
}

void Texture::DrawLine(SDL_Point begin, SDL_Point end, SDL_Renderer * renderer, int circle_size, int r, int g, int b, int a) {
	SDL_SetTextureColorMod(circle, r, g, b);
	int addX, addY;
	int counter = 0;
	int dx = end.x - begin.x;
	int dy = end.y - begin.y;
	if (dx < 0) {
		addX = -1;
		dx = -dx;
	}
	else {
		addX = 1;
	}
	if (dy < 0) {
		addY = -1;
		dy = -dy;
	}
	else {
		addY = 1;
	}

	int x = begin.x;
	int y = begin.y;

	double n = sqrt(circle_size) / 2 + 1;
	addX *= n;
	addY *= n;

	if (dx >= dy) {
		for (int i = 0; i < dx; i += n) {
			x += addX;
			counter += dy;
			if (counter >= dx) {
				y += addY;
				counter -= dx;
			}
			SDL_Rect dst;
			dst.w = circle_size;
			dst.h = circle_size;
			dst.x = x - circle_size / 2;
			dst.y = y - circle_size / 2;
			SDL_SetTextureBlendMode(circle, SDL_BLENDMODE_ADD);
			SDL_SetTextureAlphaMod(circle, a / 2);
			SDL_RenderCopy(renderer, circle, NULL, &dst);
			SDL_SetTextureBlendMode(circle, SDL_BLENDMODE_BLEND);
			SDL_SetTextureAlphaMod(circle, a);
			SDL_RenderCopy(renderer, circle, NULL, &dst);
		}
	}
	else {
		for (int i = 0; i < dy; i += n) {
			y += addY;
			counter += dx;
			if (counter >= dy) {
				x += addX;
				counter -= dy;
			}
			SDL_Rect dst;
			dst.w = circle_size;
			dst.h = circle_size;
			dst.x = x - circle_size / 2;
			dst.y = y - circle_size / 2;
			SDL_SetTextureBlendMode(circle, SDL_BLENDMODE_ADD);
			SDL_SetTextureAlphaMod(circle, a / 2);
			SDL_RenderCopy(renderer, circle, NULL, &dst);
			SDL_SetTextureBlendMode(circle, SDL_BLENDMODE_BLEND);
			SDL_SetTextureAlphaMod(circle, a);
			SDL_RenderCopy(renderer, circle, NULL, &dst);
		}
	}
}

SDL_Texture * Texture::LoadImage(const char * filename, SDL_Renderer * renderer) {
	SDL_Surface *surface;
	SDL_Texture *texture;
	surface = IMG_Load(FileIO::Location(filename).c_str());
	Log::Debug("Texture", "LoadImage", IMG_GetError());
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	return texture;
}

SDL_Texture * Texture::LoadText(const char* str, SDL_Renderer* renderer, int size, const char* fontfile_name, Color::RGB color, int wrap)
{
	TTF_Font* font;
	SDL_Surface *surface;
	SDL_Texture *texture;
	string adr;
	adr = fontfile_name;
	adr += ".font";
	adr = FileIO::Location(adr);
	font = TTF_OpenFont(adr.c_str(), size);

	SDL_Color clr;
	clr.r = color.r;
	clr.g = color.g;
	clr.b = color.b;
	clr.a = 255;

	surface = TTF_RenderUTF8_Blended_Wrapped(font, str, color.to_SDL(), wrap);

	texture = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_FreeSurface(surface);

	//delete font;
	TTF_CloseFont(font);
	return texture;
}