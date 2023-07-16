#include "Status.h"

bool Status::event_rendering = false;
bool Status::get_event = false;

void Status::RenderOnlyEventOccured() {
	Status::event_rendering = true;
}

void Status::RenderRegularly() {
	Status::event_rendering = false;
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
