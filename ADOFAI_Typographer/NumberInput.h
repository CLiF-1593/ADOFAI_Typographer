#pragma once
#include <SDL.h>
#include <string>
#include <vector>x

class NumberInput {
public:
	enum InputType {
		SIGNED_INT,
		UNSIGNED_INT,
		SIGNED_FLOAT,
		UNSIGNED_FLOAT
	};

private:
	enum Status {
		Normal,
		Editing
	};
	bool is_hovered;
	bool is_pressed;

	SDL_Rect rect;
	Status status;
	std::string input;
	double min, max;
	bool is_enabled;
	SDL_Renderer* ren;
	InputType type;
	int size;

	std::vector<SDL_Texture*> input_texture;

	int cursor_pos;

	bool StringCheck(std::string str);
	std::string FormatedInput();

public:
	NumberInput(double init_value, SDL_Renderer* ren, int size, int x, int y, InputType type);
	~NumberInput();

	NumberInput& SetMin(double min);
	NumberInput& SetMax(double max);

	void SetEnabled(bool enabled);

	SDL_Rect GetRect();

	void SetRect(SDL_Rect rect);

	void SetPosition(int x, int y);

	void SetType(InputType type);

	void Render();
	void EventProcess(SDL_Event* evt);

	void SetValue(double value);
	void SetValue(int value);
	void SetValue(std::string value);

	double GetValue();
};

