#include "Application.hpp"

#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>

Application::Application() :
	m_window(sf::VideoMode(1024, 768), "Secondary Map")
{

}

void Application::run()
{
	while(m_window.isOpen())
	{
		update();
		draw();
	}
}

void Application::update()
{
	sf::Event event;

	while(m_window.pollEvent(event))
	{
		if(event.type == sf::Event::Closed)
			m_window.close();
	}
}

void Application::draw()
{
	m_window.clear();
	m_window.display();
}
