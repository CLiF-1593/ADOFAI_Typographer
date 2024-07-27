#pragma once
#include <vector>
#include <string>
#include "Transition.h"
#include "UI_Element.h"
#include "UI_Window.h"

class Motion {
	std::string prev_name;
	SDL_Texture* texture;

public:
	class Unit {
	public:
		enum MotionType { NONE, POSITION, ROTATION, SCALE, OPACITY };

		int begin_frame = 0, end_frame = 30;
		bool immediate = false;
		TransitionType transition = LINEAR;

		Unit(const MotionType& type) : motion_type(type) {};
		inline MotionType Type() { return motion_type; }

		void SetImmediate(bool immediate);

	private:
		MotionType motion_type;
	};
	class Position : public Unit {
	public:
		double x = 0, y = 0;
		Position() : Unit(POSITION) {};
	};
	class Rotation : public Unit {
	public:
		double degree = 0;
		Rotation() : Unit(ROTATION) {};
	};
	class Scale : public Unit {
	public:
		double x = 100, y = 100;
		Scale() : Unit(SCALE) {};
	};
	class Opacity : public Unit {
	public:
		double opacity = 100;
		Opacity() : Unit(OPACITY) {};
	};

	std::vector<Position> position;
	std::vector<Rotation> rotation;
	std::vector<Scale> scale;
	std::vector<Opacity> opacity;

	std::string name;

	Motion::Unit* current_motion_unit;

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

