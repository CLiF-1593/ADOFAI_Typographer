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

bool IsSurfaceFullyTransparent(SDL_Surface* surface) {
	int width = surface->w;
	int height = surface->h;

	Uint32* pixels = (Uint32*)surface->pixels;
	int pitch = surface->pitch / sizeof(Uint32);

	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			int index = y * pitch + x;
			Uint8 r, g, b, a;
			SDL_GetRGBA(pixels[index], surface->format, &r, &g, &b, &a);
			if (a != 0) {
				return false;
			}
		}
	}

	return true;
}

SDL_Texture * Texture::LoadText(const char* str, SDL_Renderer* renderer, int size, const char* fontfile_name, Color::RGB color, int wrap)
{
	TTF_Font* font;
	SDL_Surface *surface;
	SDL_Texture *texture;
	SDL_Color clr;
	clr.r = color.r;
	clr.g = color.g;
	clr.b = color.b;
	clr.a = 255;
	
	GENERATE_TEXT:
	string adr;
	adr = fontfile_name;
	adr += ".font";
	adr = FileIO::Location(adr);
	font = TTF_OpenFont(adr.c_str(), size);

	surface = TTF_RenderUTF8_Blended_Wrapped(font, str, color.to_SDL(), wrap);

	if (!surface || IsSurfaceFullyTransparent(surface)) {
		if (fontfile_name == "main") fontfile_name = "main_univ";
		else if (fontfile_name == "main_univ") fontfile_name = "main_univ_sub";
		else if (fontfile_name == "main_univ_sub") fontfile_name = "";
		else fontfile_name = "main";

		if (fontfile_name != "") {
			if (!surface) {
				SDL_FreeSurface(surface);
				surface = nullptr;
			}
			goto GENERATE_TEXT;
		}
	}

	texture = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_FreeSurface(surface);

	TTF_CloseFont(font);
	return texture;
}