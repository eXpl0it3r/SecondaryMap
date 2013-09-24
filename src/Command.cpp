#include "Command.hpp"

#include "utility.hpp"
#include "Application.hpp"

#include <iostream>

Command::Command(const std::string& tokens, const std::string& data) :
	m_failed{false}
{
	init_tokens(tokens);
	init_data(data);

	if(m_failed)
	{
		m_tokens.clear();
		m_data.clear();
	}
}

bool Command::failed()
{
	return m_failed;
}

std::deque<std::string>& Command::tokens()
{
	return m_tokens;
}

Json::Value& Command::data()
{
	return m_data;
}

void Command::init_tokens(const std::string& tokens)
{
	// Split the tokens
	m_tokens = utility::split(tokens, '/');

	// Required: API call & correct version
	if(m_tokens.empty())
	{
		sf::err() << "No tokens given!" << std::endl;
		m_failed = true;
	}
	else if(m_tokens.front() != Application::VERSION)
	{
		sf::err() << "API version miss match!" << std::endl;
		m_failed = true;
	}

	// Remove the API version information
	m_tokens.pop_front();

#ifndef NDEBUG
	for(auto& token : m_tokens)
		std::cout << "Split: " << token << std::endl;
#endif
}

void Command::init_data(const std::string& data)
{
	// Extract JSON data
	Json::Reader reader;
	if(!reader.parse(data, m_data))
	{
		sf::err() << "Failed to parse the data" << std::endl << reader.getFormattedErrorMessages() << std::endl;
		m_failed = true;
	}
#ifndef NDEBUG
	else
	{
		std::cout << "Parsed successfully!" << std::endl;
	}
#endif
}
