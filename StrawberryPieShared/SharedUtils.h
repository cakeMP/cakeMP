#pragma once

#include <cstdint>

#include <string>

// From: https://en.wikipedia.org/wiki/Jenkins_hash_function#one-at-a-time
inline uint32_t hashGet(const char* str)
{
	size_t len = strlen(str);
	size_t i = 0;
	uint32_t hash = 0;
	while (i != len) {
		hash += (char)tolower(str[i++]);
		hash += hash << 10;
		hash ^= hash >> 6;
	}
	hash += hash << 3;
	hash ^= hash >> 11;
	hash += hash << 15;
	return hash;
}

inline uint32_t hashGet(const std::string &str)
{
	return hashGet(str.c_str());
}
