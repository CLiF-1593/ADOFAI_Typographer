#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "UI_Window.h"
#include "Status.h"
#include "Scene.h"
#include "IntroScene.h"
#include "Color.h"
#include "SoftwareStatus.h"
#include <windows.h>
#include <ctime>
#include "Config.h"
using namespace std;

int main(int argc, char *argv[]) {
	SetConsoleOutputCP(65001);
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG);
	TTF_Init();
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
	Color::WindowsSTD::Init();
	UI_Window win;
	SDL_Event evt;
	Scene *scene = new IntroScene(&win);

	int prev_time = clock();
	int limit_prev_time = clock();

	bool event_occured = false;

	while (SoftwareStatus::run) {
		if (Status::IsRenderingOnlyEventOccured()) event_occured = SDL_WaitEvent(&evt) || event_occured;
		else event_occured = SDL_PollEvent(&evt) || event_occured;
		if (evt.type == SDL_QUIT) break;
		if (scene) {
			if (Status::IsGettingEvent()) {
				scene->EventProcess(&evt);
			}

			if (scene->IsFinished()) {
				Scene* next_scene = scene->GetNextScene();
				delete scene;
				scene = next_scene;
			}

			int delta = clock() - limit_prev_time;
			if (delta > 1000 / FPS_LIMIT) {
				int add = 1000 / FPS_LIMIT;
				limit_prev_time += add * (int)((clock() - limit_prev_time) / add);

				delta = clock() - prev_time;
				if (event_occured || delta > 1000 / Status::GetFPS()) {
					add = 1000 / Status::GetFPS();
					prev_time += add * (int)((clock() - prev_time) / add);
					event_occured = false;

					if (Status::IsAlwaysRender() || win.IsFocused()) {
						SDL_RenderClear(win.GetRenderer());
						scene->Rendering();
						SDL_SetRenderDrawColor(win.GetRenderer(), CLR_STD_BACKGROUND);
						SDL_RenderPresent(win.GetRenderer());
					}
				}
			}
		}
	}
	delete scene;
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
	return 0;
}
