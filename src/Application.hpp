#pragma once

#include <SFML/Graphics/RenderWindow.hpp>

class Application
{
public:
	Application();

	void run();

private:
	void update();
	void draw();

private:
	sf::RenderWindow m_window;
};
