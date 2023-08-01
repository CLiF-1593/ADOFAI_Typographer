#pragma once
#include <SDL.h>

enum CursorType {
	CUR_NORMAL = 32512,
	CUR_WAIT = 32514,
	CUR_TEXT = 32513,
	CUR_RESIZE_UPRIGHT = 32642,
	CUR_RESIZE_UPLEFT = 32643,
	CUR_RESIZE_HORIZONTAL =	32644,
	CUR_RESIZE_VERTICAL = 32645
};

class Status {
private:
	static bool event_rendering;
	static bool get_event;
	static int fps;
	static bool always_render;

	static SDL_SystemCursor current_cursor_shape;

public:
	static void Init();

	static void RenderOnlyEventOccured();
	static void RenderRegularly(int fps);

	static void GetEvent();
	static void IgnoreEvent();

	static bool IsRenderingOnlyEventOccured();
	static bool IsRenderingRegularly();

	static bool IsGettingEvent();
	static bool IsIgnoringEvent();

	static void AlwaysRender();
	static void EconomyRender();

	static bool IsAlwaysRender();
	static bool IsEconomyRender();

	static int GetFPS();

	static void SetCursor(SDL_SystemCursor id);
};