#pragma once

#include <Common.h>

NAMESPACE_BEGIN;

std::string fmtString(const char* format, ...);

std::string fmtBytes(size_t bytes);

NAMESPACE_END;
