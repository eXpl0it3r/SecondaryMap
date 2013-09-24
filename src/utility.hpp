#pragma once

#include <deque>
#include <string>
#include <sstream>

namespace utility
{
	const float PI = 3.141592f;

	std::deque<std::string> split(const std::string& s, char delim);
}
