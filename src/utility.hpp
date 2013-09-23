#pragma once

#include <deque>
#include <string>
#include <sstream>

namespace utility
{
	const float PI = 3.141592f;

	std::deque<std::string> split(const std::string& s, char delim)
	{
		std::deque<std::string> elems;
		std::stringstream ss(s);
		std::string item;
		while(std::getline(ss, item, delim))
		{
			if(item != "")
				elems.push_back(item);
		}
		return elems;
	}
}
