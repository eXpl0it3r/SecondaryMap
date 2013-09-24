#pragma once

#include "Network.hpp"
#include "Command.hpp"

#include <Thor/Graphics/BigSprite.hpp>
#include <Thor/Graphics/BigTexture.hpp>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Vector2.hpp>

#include <deque>
#include <string>

class Application
{
public:
	Application();

	void run();

public:
	static const std::string VERSION;

private:
	void process_command(Command command);

	void update();
	void draw();

private:

	sf::RenderWindow m_window;
	Network m_network;

	sf::Sprite m_player;
	sf::Texture m_markers;

	thor::BigSprite m_map;
	thor::BigTexture m_new_eden;

	float m_zoom;
	bool m_move_view;
	sf::Vector2f m_move_current;
	sf::View m_view;

	float m_factor;
};
