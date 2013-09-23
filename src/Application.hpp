#pragma once

#include <SFNUL/TcpListener.hpp>
#include <SFNUL/TcpSocket.hpp>

#include <Thor/Graphics/BigTexture.hpp>
#include <Thor/Graphics/BigSprite.hpp>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>

#include <deque>

class Application
{
public:
	Application();

	void run();

private:
	void process_command(std::pair<std::string, std::string> command);

	void update();
	void draw();

private:
	const std::string VERSION = "0.1";

	sf::RenderWindow m_window;
	sfn::TcpListener::Ptr m_listener;
	std::deque<sfn::TcpSocket::Ptr> m_sockets;

	std::deque<std::pair<std::string, std::string>> m_commands;

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
