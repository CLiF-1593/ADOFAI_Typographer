#include "MenuComponent.h"
#include "Texture.h"
#include "TypoStatus.h"
#include "Log.h"

const int menu_font_size = 30;
const int menu_height = 40;

const int margin = 10;

const int menu_selector_width = 200;

MenuComponent::MenuComponent(UI_Window* win) : Component(win) {
	this->activated_menu = NONE;

	this->logo = Texture::LoadImage("software_logo.img", this->win->GetRenderer());
	this->motion_editor_title = Texture::LoadText(":: Motion Editor", this->win->GetRenderer(), menu_font_size, "main", Color::RGB(CLR_SUB_TEXT));
	this->typo_editor_title = Texture::LoadText(":: Typography Editor", this->win->GetRenderer(), menu_font_size, "main", Color::RGB(CLR_SUB_TEXT));

	int x = (menu_height - menu_font_size) / 2;
	x += menu_font_size;
	int y = (menu_height - menu_font_size) / 2;
	x += margin;

	this->btn_file = new Button("File", this->win->GetRenderer(), menu_font_size, x, y);
	x += this->btn_file->GetRect().w + margin;
	this->btn_edit = new Button("Edit", this->win->GetRenderer(), menu_font_size, x, y);
	x += this->btn_edit->GetRect().w + margin;
	this->btn_mode = new Button("Mode", this->win->GetRenderer(), menu_font_size, x, y);
	x += this->btn_mode->GetRect().w + margin;
	this->btn_object = new Button("Object", this->win->GetRenderer(), menu_font_size, x, y);
	this->btn_motion = new Button("Motion", this->win->GetRenderer(), menu_font_size, x, y);
	x += max(this->btn_object->GetRect().w, this->btn_motion->GetRect().w) + margin;
	this->btn_render = new Button("Render", this->win->GetRenderer(), menu_font_size, x, y);
	x += this->btn_render->GetRect().w + margin;
	this->btn_preview = new Button("Preview", this->win->GetRenderer(), menu_font_size, x, y);
	x += this->btn_preview->GetRect().w + margin;
	this->btn_info = new Button("Info", this->win->GetRenderer(), menu_font_size, x, y);

	y = menu_height + margin;
	this->selection_file_new = new Button("New", this->win->GetRenderer(), menu_font_size, this->btn_file->GetRect().x + margin, y);
	y += this->selection_file_new->GetRect().h;
	this->selection_file_open = new Button("Open", this->win->GetRenderer(), menu_font_size, this->btn_file->GetRect().x + margin, y);
	y += this->selection_file_new->GetRect().h;
	this->selection_file_save = new Button("Save", this->win->GetRenderer(), menu_font_size, this->btn_file->GetRect().x + margin, y);
	y += this->selection_file_new->GetRect().h;
	this->selection_file_save_as = new Button("Save As", this->win->GetRenderer(), menu_font_size, this->btn_file->GetRect().x + margin, y);
	y += this->selection_file_new->GetRect().h;
	this->selection_file_exit = new Button("Exit", this->win->GetRenderer(), menu_font_size, this->btn_file->GetRect().x + margin, y);

	y = menu_height + margin;
	this->selection_edit_undo = new Button("Undo", this->win->GetRenderer(), menu_font_size, this->btn_edit->GetRect().x + margin, y);
	y += this->selection_file_new->GetRect().h;
	this->selection_edit_redo = new Button("Redo", this->win->GetRenderer(), menu_font_size, this->btn_edit->GetRect().x + margin, y);
	y += this->selection_file_new->GetRect().h;
	this->selection_edit_cut = new Button("Cut", this->win->GetRenderer(), menu_font_size, this->btn_edit->GetRect().x + margin, y);
	y += this->selection_file_new->GetRect().h;
	this->selection_edit_copy = new Button("Copy", this->win->GetRenderer(), menu_font_size, this->btn_edit->GetRect().x + margin, y);
	y += this->selection_file_new->GetRect().h;
	this->selection_edit_paste = new Button("Paste", this->win->GetRenderer(), menu_font_size, this->btn_edit->GetRect().x + margin, y);
	y += this->selection_file_new->GetRect().h;
	this->selection_edit_delete = new Button("Delete", this->win->GetRenderer(), menu_font_size, this->btn_edit->GetRect().x + margin, y);

	y = menu_height + margin;
	this->selection_mode_typo_editor = new Button("Typo Editor", this->win->GetRenderer(), menu_font_size, this->btn_mode->GetRect().x + margin, y);
	y += this->selection_file_new->GetRect().h;
	this->selection_mode_motion_editor = new Button("Motion Editor", this->win->GetRenderer(), menu_font_size, this->btn_mode->GetRect().x + margin, y);

	y = menu_height + margin;
	this->selection_object_add_text = new Button("Add Text", this->win->GetRenderer(), menu_font_size, this->btn_object->GetRect().x + margin, y);
	y += this->selection_file_new->GetRect().h;
	this->selection_object_add_image = new Button("Add Image", this->win->GetRenderer(), menu_font_size, this->btn_object->GetRect().x + margin, y);
	y += this->selection_file_new->GetRect().h;
	this->selection_object_add_box = new Button("Add Box", this->win->GetRenderer(), menu_font_size, this->btn_object->GetRect().x + margin, y);
	y += this->selection_file_new->GetRect().h;

	y = menu_height + margin;
	this->selection_motion_new_motion = new Button("New Motion", this->win->GetRenderer(), menu_font_size, this->btn_motion->GetRect().x + margin, y);
	y += this->selection_file_new->GetRect().h;
	this->selection_motion_export_motion = new Button("Export Motion", this->win->GetRenderer(), menu_font_size, this->btn_motion->GetRect().x + margin, y);
	y += this->selection_file_new->GetRect().h;
	this->selection_motion_import_motion = new Button("Import Motion", this->win->GetRenderer(), menu_font_size, this->btn_motion->GetRect().x + margin, y);
	y += this->selection_file_new->GetRect().h;
	this->selection_motion_change_motion_name = new Button("Change Motion Name", this->win->GetRenderer(), menu_font_size, this->btn_motion->GetRect().x + margin, y);

	y = menu_height + margin;
	this->selection_info_credit = new Button("Credit", this->win->GetRenderer(), menu_font_size, this->btn_info->GetRect().x + margin, y);
	y += this->selection_file_new->GetRect().h;
	this->selection_info_how_to_use = new Button("How to Use", this->win->GetRenderer(), menu_font_size, this->btn_info->GetRect().x + margin, y);
	y += this->selection_file_new->GetRect().h;
	this->selection_info_github = new Button("Github", this->win->GetRenderer(), menu_font_size, this->btn_info->GetRect().x + margin, y);
}

MenuComponent::~MenuComponent() {
	SDL_DestroyTexture(this->motion_editor_title);
	SDL_DestroyTexture(this->typo_editor_title);
	SDL_DestroyTexture(this->logo);
	delete this->btn_file, this->btn_edit, this->btn_mode, this->btn_object, this->btn_motion, this->btn_render, this->btn_preview, this->btn_info;
	delete this->selection_file_new, this->selection_file_open, this->selection_file_save, this->selection_file_save_as, this->selection_file_exit;
	delete this->selection_edit_undo, this->selection_edit_redo, this->selection_edit_cut, this->selection_edit_copy, this->selection_edit_paste, this->selection_edit_delete;
	delete this->selection_mode_typo_editor, this->selection_mode_motion_editor;
	delete this->selection_object_add_text, this->selection_object_add_image, this->selection_object_add_box;
	delete this->selection_motion_new_motion, this->selection_motion_export_motion, this->selection_motion_import_motion, this->selection_motion_change_motion_name;
	delete this->selection_info_credit, this->selection_info_how_to_use, this->selection_info_github;
}

void MenuComponent::Rendering() {
	SDL_Rect rect;
	rect.x = 0;
	rect.y = 0;
	rect.w = this->win->GetWidth();
	rect.h = menu_height;
	SDL_SetRenderDrawColor(this->win->GetRenderer(), CLR_MENU_BACKGROUND);
	SDL_RenderFillRect(this->win->GetRenderer(), &rect);
	rect.h = 1;
	SDL_SetRenderDrawColor(this->win->GetRenderer(), CLR_STD_BORDER);
	SDL_RenderFillRect(this->win->GetRenderer(), &rect);
	rect.y = menu_height;
	rect.h = 2;
	SDL_SetRenderDrawColor(this->win->GetRenderer(), CLR_WINDOW_THEME);
	SDL_RenderFillRect(this->win->GetRenderer(), &rect);
	
	rect.x = (menu_height - menu_font_size) / 2;
	rect.y = (menu_height - menu_font_size) / 2;
	rect.w = menu_font_size;
	rect.h = menu_font_size;
	SDL_RenderCopy(this->win->GetRenderer(), this->logo, NULL, &rect);
	rect.x = this->win->GetWidth() - (menu_height - menu_font_size) / 2;
	SDL_Texture* title;
	if(TypoStatus::software_mode == TypoStatus::MOTION_EDITOR) title = this->motion_editor_title;
	else title = this->typo_editor_title;
	SDL_QueryTexture(title, NULL, NULL, &rect.w, &rect.h);
	rect.x -= rect.w;
	SDL_RenderCopy(this->win->GetRenderer(), title, NULL, &rect);
	/*
	rect.x += rect.w + margin;
	rect.w = margin;
	SDL_RenderFillRect(this->win->GetRenderer(), &rect);
	*/

	this->btn_file->Render();
	this->btn_edit->Render();
	this->btn_mode->Render();
	if (TypoStatus::software_mode == TypoStatus::MOTION_EDITOR) 
		this->btn_motion->Render();
	else this->btn_object->Render();
	this->btn_render->Render();
	this->btn_preview->Render();
	this->btn_info->Render();

	if (this->activated_menu) {
		SDL_Rect rect;
		switch (this->activated_menu) {
			case FILE: rect.x = this->btn_file->GetRect().x; rect.h = this->selection_file_cnt; break;
			case EDIT: rect.x = this->btn_edit->GetRect().x; rect.h = this->selection_edit_cnt; break;
			case MODE: rect.x = this->btn_mode->GetRect().x; rect.h = this->selection_mode_cnt; break;
			case OBJECT: rect.x = this->btn_object->GetRect().x; rect.h = this->selection_object_cnt; break;
			case MOTION: rect.x = this->btn_motion->GetRect().x; rect.h = this->selection_motion_cnt; break;
			case INFO : rect.x = this->btn_info->GetRect().x; rect.h = this->selection_info_cnt; break;
			default: rect.x = 0; break;
		}
		if (rect.x) {
			rect.w = menu_selector_width;
			rect.y = menu_height;
			rect.h *= this->selection_file_new->GetRect().h;
			rect.h += margin * 2;

			SDL_SetRenderDrawColor(this->win->GetRenderer(), CLR_MENU_BACKGROUND);
			SDL_RenderFillRect(this->win->GetRenderer(), &rect);
			SDL_SetRenderDrawColor(this->win->GetRenderer(), CLR_STD_BORDER);
			SDL_RenderDrawRect(this->win->GetRenderer(), &rect);
		}

		switch (this->activated_menu) {
		case FILE:
			this->selection_file_new->Render();
			this->selection_file_open->Render();
			this->selection_file_save->Render();
			this->selection_file_save_as->Render();
			this->selection_file_exit->Render();
			break;
		case EDIT:
			this->selection_edit_undo->Render();
			this->selection_edit_redo->Render();
			this->selection_edit_cut->Render();
			this->selection_edit_copy->Render();
			this->selection_edit_paste->Render();
			this->selection_edit_delete->Render();
			break;
		case MODE:
			this->selection_mode_typo_editor->Render();
			this->selection_mode_motion_editor->Render();
			break;
		case OBJECT:
			this->selection_object_add_text->Render();
			this->selection_object_add_image->Render();
			this->selection_object_add_box->Render();
			break;
		case MOTION:
			this->selection_motion_new_motion->Render();
			this->selection_motion_export_motion->Render();
			this->selection_motion_import_motion->Render();
			this->selection_motion_change_motion_name->Render();
			break;
		case INFO:
			this->selection_info_credit->Render();
			this->selection_info_how_to_use->Render();
			this->selection_info_github->Render();
			break;
		}
	}
}

void MenuComponent::EventProcess(SDL_Event* evt) {
	// Event Process
	this->btn_file->EventProcess(evt);
	this->btn_edit->EventProcess(evt);
	this->btn_mode->EventProcess(evt);
	if (TypoStatus::software_mode == TypoStatus::MOTION_EDITOR)
		this->btn_motion->EventProcess(evt);
	else this->btn_object->EventProcess(evt);
	this->btn_render->EventProcess(evt);
	this->btn_preview->EventProcess(evt);
	this->btn_info->EventProcess(evt);

	switch (this->activated_menu) {
	case FILE:
		this->selection_file_new->EventProcess(evt);
		this->selection_file_open->EventProcess(evt);
		this->selection_file_save->EventProcess(evt);
		this->selection_file_save_as->EventProcess(evt);
		this->selection_file_exit->EventProcess(evt);
		break;
	case EDIT:
		this->selection_edit_undo->EventProcess(evt);
		this->selection_edit_redo->EventProcess(evt);
		this->selection_edit_cut->EventProcess(evt);
		this->selection_edit_copy->EventProcess(evt);
		this->selection_edit_paste->EventProcess(evt);
		this->selection_edit_delete->EventProcess(evt);
		break;
	case MODE:
		this->selection_mode_typo_editor->EventProcess(evt);
		this->selection_mode_motion_editor->EventProcess(evt);
		break;
	case OBJECT:
		this->selection_object_add_text->EventProcess(evt);
		this->selection_object_add_image->EventProcess(evt);
		this->selection_object_add_box->EventProcess(evt);
		break;
	case MOTION:
		this->selection_motion_new_motion->EventProcess(evt);
		this->selection_motion_export_motion->EventProcess(evt);
		this->selection_motion_import_motion->EventProcess(evt);
		this->selection_motion_change_motion_name->EventProcess(evt);
		break;
	case INFO:
		this->selection_info_credit->EventProcess(evt);
		this->selection_info_how_to_use->EventProcess(evt);
		this->selection_info_github->EventProcess(evt);
		break;
	}

	// Click Event Check

	// Menu Activation
	if (this->btn_file->IsClicked() || (this->activated_menu && this->btn_file->IsHovered())) {
		this->activated_menu = FILE;
	}
	else if (this->btn_edit->IsClicked() || (this->activated_menu && this->btn_edit->IsHovered())) {
		this->activated_menu = EDIT;
	}
	else if (this->btn_mode->IsClicked() || (this->activated_menu && this->btn_mode->IsHovered())) {
		this->activated_menu = MODE;
	}
	else if (this->btn_object->IsClicked() || (this->activated_menu && this->btn_object->IsHovered())) {
		this->activated_menu = OBJECT;
	}
	else if (this->btn_motion->IsClicked() || (this->activated_menu && this->btn_motion->IsHovered())) {
		this->activated_menu = MOTION;
	}
	else if (this->btn_render->IsClicked() || (this->activated_menu && this->btn_render->IsHovered())) {
		this->activated_menu = RENDER;
	}
	else if (this->btn_preview->IsClicked() || (this->activated_menu && this->btn_preview->IsHovered())) {
		this->activated_menu = PREVIEW;
	}
	else if (this->btn_info->IsClicked() || (this->activated_menu && this->btn_info->IsHovered())) {
		this->activated_menu = INFO;
	}
	else if (this->activated_menu) {
		if (evt->type == SDL_MOUSEBUTTONUP) {
			SDL_Rect rect;
			switch (this->activated_menu) {
			case FILE: rect.x = this->btn_file->GetRect().x; rect.h = this->selection_file_cnt; break;
			case EDIT: rect.x = this->btn_edit->GetRect().x; rect.h = this->selection_edit_cnt; break;
			case MODE: rect.x = this->btn_mode->GetRect().x; rect.h = this->selection_mode_cnt; break;
			case OBJECT: rect.x = this->btn_object->GetRect().x; rect.h = this->selection_object_cnt; break;
			case MOTION: rect.x = this->btn_motion->GetRect().x; rect.h = this->selection_motion_cnt; break;
			case INFO: rect.x = this->btn_info->GetRect().x; rect.h = this->selection_info_cnt; break;
			default: rect.x = 0; break;
			}
			if (rect.x) {
				rect.w = menu_selector_width;
				rect.y = menu_height;
				rect.h *= this->selection_file_new->GetRect().h;
				rect.h += margin * 2;

				if (evt->button.x < rect.x || evt->button.x > rect.x + rect.w || evt->button.y < rect.y || evt->button.y > rect.y + rect.h) {
					this->activated_menu = NONE;
				}
			}
		}
	}

	// Menu Selection Process
	switch (this->activated_menu) {
	case FILE:
		this->selection_file_new->IsClicked();
		this->selection_file_open->IsClicked();
		this->selection_file_save->IsClicked();
		this->selection_file_save_as->IsClicked();
		this->selection_file_exit->IsClicked();
		break;
	case EDIT:
		this->selection_edit_undo->IsClicked();
		this->selection_edit_redo->IsClicked();
		this->selection_edit_cut->IsClicked();
		this->selection_edit_copy->IsClicked();
		this->selection_edit_paste->IsClicked();
		this->selection_edit_delete->IsClicked();
		break;
	case MODE:
		if(this->selection_mode_typo_editor->IsClicked()) TypoStatus::software_mode = TypoStatus::TYPO_EDITOR;
		if(this->selection_mode_motion_editor->IsClicked()) TypoStatus::software_mode = TypoStatus::MOTION_EDITOR;
		break;
	case OBJECT:
		this->selection_object_add_text->IsClicked();
		this->selection_object_add_image->IsClicked();
		this->selection_object_add_box->IsClicked();
		break;
	case MOTION:
		this->selection_motion_new_motion->IsClicked();
		this->selection_motion_export_motion->IsClicked();
		this->selection_motion_import_motion->IsClicked();
		this->selection_motion_change_motion_name->IsClicked();
		break;
	case INFO:
		this->selection_info_credit->IsClicked();
		this->selection_info_how_to_use->IsClicked();
		this->selection_info_github->IsClicked();
		break;
	}
}
