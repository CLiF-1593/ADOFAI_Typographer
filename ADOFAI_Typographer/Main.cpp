#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "UI_Window.h"
#include "Status.h"
#include "Scene.h"
#include "IntroScene.h"
#include "Color.h"
#include "SoftwareStatus.h"
using namespace std;

int main(int argc, char *argv[]) {
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG);
	TTF_Init();
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
	Color::WindowsSTD::Init();
	UI_Window win;
	SDL_Event evt;
	Scene *scene = new IntroScene(&win);
	while (SoftwareStatus::run) {
		if (Status::IsRenderingOnlyEventOccured()) SDL_WaitEvent(&evt);
		else SDL_PollEvent(&evt);
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

			SDL_RenderClear(win.GetRenderer());
			scene->Rendering();
			SDL_SetRenderDrawColor(win.GetRenderer(), CLR_STD_BACKGROUND);
			SDL_RenderPresent(win.GetRenderer());
		}
	}
	delete scene;
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
	return 0;
}