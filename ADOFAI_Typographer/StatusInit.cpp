#include "SoftwareStatus.h"
#include "TypoStatus.h"
#include "MotionStatus.h"

// Software Status Initialization
SoftwareStatus::SoftwareMode SoftwareStatus::software_mode = SoftwareStatus::TYPO_EDITOR;
bool SoftwareStatus::run = true;
std::string SoftwareStatus::current_file_path = "";

// Typo Status Initialization


// Motion Status Initialization
