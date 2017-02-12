#pragma once

// C includes
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <ctime>
#include <cassert>

#ifndef _MSC_VER
#include <unistd.h>
#endif

// STL includes
#include <thread>
#include <mutex>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>
#include <chrono>

// glm includes
#include <glm/glm.hpp>
#include <glm/vec3.hpp>

// Our types and definitions
typedef std::chrono::high_resolution_clock Clock;
typedef Clock::time_point ClockTime;
#define ClockDuration std::chrono::duration_cast<std::chrono::milliseconds>
