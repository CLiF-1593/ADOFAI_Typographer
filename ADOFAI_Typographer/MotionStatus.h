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
	class Unit {
	public:
		int begin_frame = 0, end_frame = 30;
		TransitionType transition;
	};
	class Position : public Unit {
	public: 
		double x = 0, y = 0;
	};
	class Rotation : public Unit {
	public:
		double degree = 0;
	};
	class Scale : public Unit {
	public: 
		double x = 100, y = 100; 
	};
	class Opacity : public Unit {
	public: 
		double opacity = 100; 
	};

	std::vector<Position> position;
	std::vector<Rotation> rotation;
	std::vector<Scale> scale;
	std::vector<Opacity> opacity;

	std::string name;

	SDL_Texture* GetTexture(UI_Window* win);

	Motion();
	~Motion();

	bool IsAbleToAddPosition(int cur_frame);
	bool IsAbleToAddRotation(int cur_frame);
	bool IsAbleToAddScale(int cur_frame);
	bool IsAbleToAddOpacity(int cur_frame);

	void AddPosition(int cur_frame, double frame_size);
	void AddRotation(int cur_frame, double frame_size);
	void AddScale(int cur_frame, double frame_size);
	void AddOpacity(int cur_frame, double frame_size);

	void GetFrontBackFrame(int cur, int& front, int& back, int level);
	bool IsOverlaped(Motion::Unit* motion, int level);
};	


class MotionStatus {
public:
	static void AddMotion();
	static void DeleteMotion();
	static Motion* CurrentMotion();

	static int GetCurrentMotionIndex();
	static std::vector<Motion*>* GetMotionList();

	static void SetCurrentMotionIndex(int index);


private:
	static std::vector<Motion*> motion_list;
	static int current_motion_index;
};

