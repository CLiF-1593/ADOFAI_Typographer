#pragma once
#include <string>

class SoftwareStatus {
public:
	enum SoftwareMode { TYPO_EDITOR, MOTION_EDITOR };
	static SoftwareMode software_mode;

	static bool run;

	static std::string current_file_path;
	static int selected_motion_index;
};

