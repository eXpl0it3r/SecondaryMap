#include "Application.hpp"

#include "utility.hpp"

#include <json/json.h>

#include <SFML/System/Err.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/VideoMode.hpp>

#include <deque>
#include <iostream>
#include <sstream>

#include <cmath>

const std::string Application::VERSION = "0.1";

Application::Application() :
	m_window(sf::VideoMode(1024, 768), "Secondary Map"),
	m_zoom(0.f),
	m_move_view(false),
	m_factor{2.f}
{
	m_window.setFramerateLimit(60);
	m_view = m_window.getDefaultView();

	m_markers.loadFromFile("assets/img/map_markers.png");

	m_player.setTexture(m_markers);
	m_player.setTextureRect({32, 32, 32, 32});
	m_player.setOrigin({16.f, 16.f});
	m_player.setColor({0xFF, 0xC0, 0x40});

	m_new_eden.loadFromFile("assets/img/dummy.jpg");//0.7.1710-New-Eden-Minimap.png");

	m_map.setTexture(m_new_eden);
	m_map.setOrigin({4615.f, 4610.f});
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
		else if(event.type == sf::Event::Resized)
		{
			sf::Vector2f size = static_cast<sf::Vector2f>(m_window.getSize());

			// Minimum size
			if(size.x < 800)
				size.x = 800;
			if(size.y < 600)
				size.y = 600;

			// Apply possible size changes
			m_window.setSize(static_cast<sf::Vector2u>(size));

			// Reset grid view
			m_view.setCenter(size/2.f);
			m_view.setSize(size);
		}
		else if(event.type == sf::Event::MouseWheelMoved)
			m_view.zoom(1.f+(0.05f*event.mouseWheel.delta));
		else if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Add)
			m_factor += 0.1f;
		else if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Subtract)
			m_factor -= 0.1f;

	}

	std::deque<Command> commands = m_network.update();

	while(!commands.empty())
	{
		process_command(commands.front());
		commands.pop_front();
	}

	m_view.setCenter(m_player.getPosition());
}

void Application::process_command(Command command)
{
	if(!command.failed())
	{
		if(command.tokens().front() == "player")
		{
			Json::Value data = command.data();

			std::cout << "x: " << data["x"].asFloat() << " / y: " << data["y"].asFloat() << std::endl;
			m_player.setPosition({data["x"].asFloat()*m_factor, -data["y"].asFloat()*m_factor});
			m_player.setRotation(data["rotation"].asFloat()*180/utility::PI);
		}
	}
}

void Application::draw()
{
	m_window.clear();
	m_window.setView(m_view);
	m_window.draw(m_map);
	m_window.draw(m_player);
	m_window.display();
}
