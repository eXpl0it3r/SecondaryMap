#pragma once

#include <json/json.h>

#include <string>
#include <deque>

class Command
{
public:
	Command(const std::string& tokens, const std::string& data);

	bool failed();
	std::deque<std::string>& tokens();
	Json::Value& data();

private:
	void init_tokens(const std::string& tokens);
	void init_data(const std::string& data);

private:
	bool m_failed;
	std::deque<std::string> m_tokens;
	Json::Value m_data;
};
