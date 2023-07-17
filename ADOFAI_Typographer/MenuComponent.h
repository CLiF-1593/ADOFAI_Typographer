#pragma once
#include "Component.h"
#include "Button.h"

/*
	Menu Bar Button List

	Typo : file, edit, mode, object, render, preview, info
	Motion : file, edit, mode, motion, render, preview, info
		- file : new, open, save, save as, exit
		- edit : undo, redo, cut, copy, paste, delete
		- mode : typo editor, motion editor
		- object : add text, add box, add image
		- motion : new motion, export motion, import motion, change motion name
		- info : credit, how to use, github
*/

#define menu_comp_font_size 30

#define menu_comp_width (this->win->GetWidth())
#define menu_comp_height 40

#define menu_comp_margin 10

#define menu_comp_selector_width 200

class MenuComponent : public Component {
private:
	SDL_Texture* logo;
	SDL_Texture* motion_editor_title;
	SDL_Texture* typo_editor_title;
	
	enum ActivatedMenu { NONE, FILE, EDIT, MODE, OBJECT, MOTION, RENDER, PREVIEW, INFO };

	ActivatedMenu activated_menu;

	Button* btn_file;
	Button* btn_edit;
	Button* btn_mode;
	Button* btn_object;
	Button* btn_motion;
	Button* btn_render;
	Button* btn_preview;
	Button* btn_info;

	const int selection_file_cnt = 5;
	Button* selection_file_new;
	Button* selection_file_open;
	Button* selection_file_save;
	Button* selection_file_save_as;
	Button* selection_file_exit;

	const int selection_edit_cnt = 6;
	Button* selection_edit_undo;
	Button* selection_edit_redo;
	Button* selection_edit_cut;
	Button* selection_edit_copy;
	Button* selection_edit_paste;
	Button* selection_edit_delete;

	const int selection_mode_cnt = 2;
	Button* selection_mode_typo_editor;
	Button* selection_mode_motion_editor;

	const int selection_object_cnt = 3;
	Button* selection_object_add_text;
	Button* selection_object_add_box;
	Button* selection_object_add_image;

	const int selection_motion_cnt = 4;
	Button* selection_motion_new_motion;
	Button* selection_motion_export_motion;
	Button* selection_motion_import_motion;
	Button* selection_motion_change_motion_name;

	const int selection_render_cnt = 2;
	Button* selection_render_render;
	Button* selection_render_preview;

	const int selection_info_cnt = 3;
	Button* selection_info_credit;
	Button* selection_info_how_to_use;
	Button* selection_info_github;

public:
	MenuComponent(UI_Window* win);
	~MenuComponent();

	void Rendering() override;
	void EventProcess(SDL_Event* evt) override;
};

