#include "Application.hpp"

#include <SFML/System/Err.hpp>

#include <sstream>
#include <fstream>

int main()
{
	// Redirect sf::err() for user-friendly error handling
	std::stringstream errors;
	sf::err().rdbuf(errors.rdbuf());

	try
	{
		Application{}.run();
	}
	catch (std::exception& e)
	{
		sf::err() << "EXCEPTION: " << e.what() << std::endl;

		std::ofstream log("SecondaryMap.log");
		log << errors.str() << std::endl;

		throw std::runtime_error("SecondaryMap has experienced an runtime error. Check the SecondaryMap.log and/or contact the developer.");
	}
}
