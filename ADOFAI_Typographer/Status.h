#pragma once
class Status {
private:
	static bool event_rendering;
	static bool get_event;

public:
	static void RenderOnlyEventOccured();
	static void RenderRegularly();

	static void GetEvent();
	static void IgnoreEvent();

	static bool IsRenderingOnlyEventOccured();
	static bool IsRenderingRegularly();

	static bool IsGettingEvent();
	static bool IsIgnoringEvent();
};

