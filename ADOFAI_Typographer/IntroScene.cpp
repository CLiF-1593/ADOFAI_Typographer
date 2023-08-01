#include "IntroScene.h"
#include "Texture.h"
#include "Log.h"
#include "Transition.h"
#include "MainScene.h"
#include "Status.h"

IntroScene::IntroScene(UI_Window* win) : Scene(win) {
	Log::Debug("IntroScene", "Constructor", " : Created");
	this->clif_logo = Texture::LoadImage("software_logo.img", this->win->GetRenderer());
	this->software_title = Texture::LoadText("ADOFAI Typographer", this->win->GetRenderer(), 65, "main", Color::RGB(CLR_STD_TEXT));
	this->developed = Texture::LoadText("Developed by CLiF", this->win->GetRenderer(), 40, "main", Color::RGB(CLR_SUB_TEXT));
	this->begin_time = clock();

	Status::IgnoreEvent();
	Status::RenderRegularly(60);
	Status::AlwaysRender();
}

IntroScene::~IntroScene() {
	Log::Debug("IntroScene", "Destructor", " : Deleted");
	SDL_DestroyTexture(this->clif_logo);
	SDL_DestroyTexture(this->software_title);
	SDL_DestroyTexture(this->developed);
}

void IntroScene::Rendering() {
	double progress = (double)(clock() - this->begin_time) / (double) this->intro_duration;
	progress *= 2;
	progress -= 1;

	if (progress >= 1) {
		this->finish = true;
		progress = 1;
	}
	else if (progress <= -1) {
		progress = -1;
	}

	SDL_Rect src, dst;
	src.x = 0;
	src.y = 0;

	SDL_QueryTexture(this->clif_logo, NULL, NULL, &src.w, &src.h);
	dst = src;
	dst.w /= 2;
	dst.h /= 2;
	dst.x = this->win->GetWidth() / 2 - dst.w / 2;
	dst.y = (this->win->GetHeight() / 2 - dst.h / 2 - 50);
	if (progress < 0) dst.y += 100 * Transition::Calculate(abs(progress), IN_CUBIC);
	SDL_SetTextureAlphaMod(this->clif_logo, 255 - 255 * Transition::Calculate(abs(progress), IN_CUBIC));
	SDL_RenderCopy(this->win->GetRenderer(), this->clif_logo, NULL, &dst);

	SDL_QueryTexture(this->software_title, NULL, NULL, &src.w, &src.h);
	dst = src;
	dst.x = this->win->GetWidth() / 2 - dst.w / 2;
	dst.y = this->win->GetHeight() / 2 - dst.h / 2 + 50;
	if (progress < 0) dst.y += 100 * Transition::Calculate(abs(progress), IN_CUBIC);
	SDL_SetTextureAlphaMod(this->software_title, 255 - 255 * Transition::Calculate(abs(progress), IN_CUBIC));
	SDL_RenderCopy(this->win->GetRenderer(), this->software_title, NULL, &dst);

	SDL_QueryTexture(this->developed, NULL, NULL, &src.w, &src.h);
	dst = src;
	dst.x = this->win->GetWidth() / 2 - dst.w / 2;
	dst.y = this->win->GetHeight() - 60;
	SDL_SetTextureAlphaMod(this->developed, 255 - 255 * Transition::Calculate(abs(progress), IN_CUBIC));
	SDL_RenderCopy(this->win->GetRenderer(), this->developed, NULL, &dst);

	SDL_SetRenderDrawColor(this->win->GetRenderer(), CLR_STD_BACKGROUND);
}

void IntroScene::EventProcess(SDL_Event* evt) {

}

Scene* IntroScene::GetNextScene() {
	return new MainScene(this->win);
}
