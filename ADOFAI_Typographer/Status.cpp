#include "Status.h"
#include "Log.h"
#include <Windows.h>

bool Status::event_rendering = false;
bool Status::get_event = false;
bool Status::always_render = true;
int Status::fps = 60;

SDL_SystemCursor Status::current_cursor_shape = SDL_SYSTEM_CURSOR_ARROW;

void Status::Init() {
	
}

void Status::RenderOnlyEventOccured() {
	Status::event_rendering = true;
}

void Status::RenderRegularly(int fps) {
	Status::event_rendering = false;
	Status::fps = fps;
}

void Status::GetEvent() {
	Status::get_event = true;
}

void Status::IgnoreEvent() {
	Status::get_event = false;
}

bool Status::IsRenderingOnlyEventOccured() {
	return Status::event_rendering;
}

bool Status::IsRenderingRegularly() {
	return !Status::event_rendering;
}

bool Status::IsGettingEvent() {
	return Status::get_event;
}

bool Status::IsIgnoringEvent() {
	return !Status::get_event;
}

void Status::AlwaysRender() {
	Status::always_render = true;
}

void Status::EconomyRender() {
	Status::always_render = false;
}

bool Status::IsAlwaysRender() {
	return Status::always_render;
}

bool Status::IsEconomyRender() {
	return !Status::always_render;
}

int Status::GetFPS() {
	return Status::fps;
}

void Status::SetCursor(SDL_SystemCursor id) {
	if (id != Status::current_cursor_shape) {
		SDL_Cursor* prev_cur = SDL_GetCursor();
		SDL_Cursor* cursor = SDL_CreateSystemCursor(id);
		SDL_SetCursor(cursor);
		SDL_FreeCursor(prev_cur);

		Status::current_cursor_shape = id;
	}
}
