#pragma once

// Build Mode Values
#define Typo_Debug 0
#define Typo_Release 1

// ===== Config Setting Zone =====

// Build Mode : Flow_Debug, Flow_Release
#define TypoBuildMode Typo_Debug

// Version Information
#define TypoVersion "v 0.0.0"

// ===============================

// ====== Available Values =======
// DEBUG or RELEASE

// DEBUG or RELEASE value
#if !TypoBuildMode
#define DEBUG_BUILD true
#define RELEASE_BUILD false
#else
#define DEBUG_BUILD false
#define RELEASE_BUILD true
#endif

using Time = time_t;