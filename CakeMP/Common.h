#pragma once

// Global defines
#define NAMESPACE_NAME sbp
#define NAMESPACE_BEGIN namespace NAMESPACE_NAME {
#define NAMESPACE_END }

// C includes
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <cstdarg>
#include <ctime>

// STL includes
#include <thread>
#include <mutex>
#include <string>
#include <sstream>
#include <queue>
#include <unordered_map>
#include <chrono>
#include <functional>

// Custom includes
#include <Memory.h>
#include <Utils/Logging.h>
#include <Utils/Parsing.h>
#include <SharedUtils.h>

// Libraries
#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtx/quaternion.hpp>

// Scratch2
#include <Libs/scratch2/s2ref.h>

// Custom assertions
#ifdef assert
#undef assert
#endif
#define assert(x) { if (!(x)) { NAMESPACE_NAME::logAssertFailed(#x, __FILE__, __LINE__); } }

// Custom typedefs
typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::time_point<std::chrono::steady_clock> ClockTime;
#define MilliDuration std::chrono::duration_cast<std::chrono::milliseconds>
#define MicroDuration std::chrono::duration_cast<std::chrono::microseconds>
