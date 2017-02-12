#pragma once

// This needs to move someplace else eventually (we don't want this in pch)
#define PROJECT_NAME "Strawberry Pie"
#define PROJECT_NAME_SHORT "Strawberry"
#define PROJECT_VERSION "0.1.000.000"

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
#include <queue>

// Custom includes
#include <Memory.h>
#include <Utils.h>

// Libraries
#include <glm/glm.hpp>
#include <glm/vec3.hpp>

// Custom assertions
#ifdef assert
#undef assert
#endif
#define assert(x) { if (!(x)) { NAMESPACE_NAME::logAssertFailed(#x, __FILE__, __LINE__); } }
