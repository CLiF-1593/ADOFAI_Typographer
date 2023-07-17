#pragma once
class SoftwareStatus {
public:
	enum SoftwareMode { TYPO_EDITOR, MOTION_EDITOR };
	static SoftwareMode software_mode;

	static bool run;
};

