#pragma once

#include <SFNUL/TcpListener.hpp>
#include <SFNUL/TcpSocket.hpp>

#include <SFML/Graphics/RenderWindow.hpp>

#include <deque>

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
	sfn::TcpListener::Ptr m_listener;
	std::deque<sfn::TcpSocket::Ptr> m_sockets;
};
