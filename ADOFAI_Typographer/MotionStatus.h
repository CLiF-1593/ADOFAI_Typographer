#pragma once
#include <vector>
#include <string>
#include "Transition.h"
#include "Button.h"
#include "UI_Window.h"

class Motion {
	std::string prev_name;
	SDL_Texture* texture;

public:
	class Position { 
	public: 
		double x = 0, y = 0;
		int begin_frame = 0, end_frame = 30; 
		TransitionType transition; 
	};
	class Rotation {
	public:
		double degree = 0;
		int begin_frame = 0, end_frame = 30; 
		TransitionType transition;
	};
	class Scale {
	public: 
		double x = 100, y = 100; 
		int begin_frame = 0, end_frame = 30; 
		TransitionType transition;
	};
	class Opacity { 
	public: 
		double opacity = 100; 
		int begin_frame = 0, end_frame = 30; 
		TransitionType transition; 
	};

	std::vector<Position> position;
	std::vector<Rotation> rotation;
	std::vector<Scale> scale;
	std::vector<Opacity> opacity;

	std::string name;

	SDL_Texture* GetTexture(UI_Window* win);

	Motion();
	~Motion();
};

class MotionStatus {
public:
	static void AddMotion();
	static void DeleteMotion();
	static Motion* GetMotion();

	static int GetCurrentMotionIndex();
	static std::vector<Motion*>* GetMotionList();

	static void SetCurrentMotionIndex(int index);

private:
	static std::vector<Motion*> motion_list;
	static int current_motion_index;
};

