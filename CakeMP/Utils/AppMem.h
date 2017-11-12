#pragma once

#include <Common.h>

NAMESPACE_BEGIN;

uint8_t* memGetBasePointer();

uint8_t* memFindPattern(uint8_t* buffer, size_t size, const char* pattern);
uint8_t* memFindPattern(const char* pattern);

NAMESPACE_END;
