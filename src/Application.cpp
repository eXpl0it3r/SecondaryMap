#include "Application.hpp"

#include <SFNUL/IpAddress.hpp>
#include <SFNUL/Endpoint.hpp>
#include <SFNUL/NetworkResource.hpp>

#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>

Application::Application() :
	m_window(sf::VideoMode(1024, 768), "Secondary Map")
{
	// Create our TCP listener socket.
	m_listener = sfn::TcpListener::Create();

	// Listen on 0.0.0.0:80
	m_listener->Listen(sfn::Endpoint{ sfn::IpAddress{ "127.0.0.1" }, 3344 });

	// Start 3 network processing threads.
	sfn::Start(3);
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

	sfn::TcpSocket::Ptr socket;

	// Dequeue any pending connections from the listener.
	while(socket = m_listener->GetPendingConnection())
	{
		char response[] =
			"HTTP/1.1 200 OK\r\n"
			"Server: SFNUL HTTP Server\r\n"
			"Content-Type: text/html; charset=UTF-8\r\n"
			"Connection: close\r\n\r\n"
			"<html><head><title>SFNUL HTTP Server Page</title></head>"
			"<body>SFNUL HTTP Server Document</body></html>\r\n\r\n";

		// Turn of connection lingering.
		socket->SetLinger( 0 );

		// Send the HTTP response.
		socket->Send(response, sizeof(response) - 1);

		// Shutdown the socket for sending.
		socket->Shutdown();

		// Store the socket.
		m_sockets.push_back(socket);
	}

	for(auto socket_iter = m_sockets.begin(); socket_iter != m_sockets.end(); )
	{
		// Remove and thereby close all active sockets that have been
		// remotely shut down and don't have data left to send.
		if((*socket_iter)->RemoteHasShutdown() && !(*socket_iter)->BytesToSend())
		{
			socket_iter = m_sockets.erase(socket_iter);
		}
		else
		{
			++socket_iter;
		}
	}
}

void Application::draw()
{
	m_window.clear();
	m_window.display();
}
