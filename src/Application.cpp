#include "Application.hpp"

#include "utility.hpp"

#include <json/json.h>

#include <SFNUL/IpAddress.hpp>
#include <SFNUL/Endpoint.hpp>
#include <SFNUL/NetworkResource.hpp>

#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/System/Err.hpp>

#include <sstream>
#include <iostream>
#include <string>

#include <cmath>

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

	m_new_eden.loadFromFile("assets/img/0.7.1710-New-Eden-Minimap.png");

	m_map.setTexture(m_new_eden);
	m_map.setOrigin({4615.f, 4610.f});

	// Create our TCP listener socket.
	m_listener = sfn::TcpListener::Create();

	// Listen on 0.0.0.0:80
	m_listener->Listen(sfn::Endpoint{ sfn::IpAddress{ "0.0.0.0" }, 3344 });

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

	sfn::TcpSocket::Ptr socket;

	// Dequeue any pending connections from the listener.
	while(socket = m_listener->GetPendingConnection())
	{
		std::cout << "BYTES: " << socket->BytesToReceive() << std::endl;

		std::size_t received_bytes = 0;
		std::stringstream ss;
		char bytes[1024] = {0};

		while(socket->BytesToReceive() > received_bytes)
		{
			std::size_t current_bytes = socket->Receive(bytes, 1024);
			received_bytes += current_bytes;

			for(std::size_t it = 0; it < current_bytes; ++it)
				ss << bytes[it];
		}

		std::string post{ss.str()};

		std::cout << "== Got data (" << received_bytes << "):" << post << std::endl << "==" << std::endl;

		if(received_bytes != 0 && post.find("POST ") == 0)
		{
			std::string::size_type url_start = post.find("POST ") + 5;
			std::string::size_type url_end = post.find(" HTTP");

			std::string url = post.substr(url_start, url_end-url_start);

			std::string::size_type data_start = post.find("\r\n\r\n") + 4;

			std::cout << url_start << "-" << url_end << "-" << data_start << std::endl;
			std::string data = post.substr(data_start);

			std::cout << url << "-" << data << std::endl;

			if(url != "" && data != "")
				m_commands.push_back(std::make_pair(url, data));
		}
		else
			std::cout << "No data to process!" << std::endl;

		char response[] =
			"HTTP/1.1 200 OK\r\n"
			"Server: SFNUL HTTP Server\r\n"
			"Content-Type: application/json; charset=UTF-8\r\n"
			"Connection: close\r\n\r\n"
			"{\"status\":\"ok\"}";

		// Turn of connection lingering.
		socket->SetLinger( 0 );

		// Send the HTTP response.
		socket->Send(response, sizeof(response) - 1);

		// Shutdown the socket for sending.
		socket->Shutdown();

		// Store the socket.
		m_sockets.push_back(socket);

		std::cout << "Socket closed!" << std::endl << std::endl;
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

	while(!m_commands.empty())
	{
		process_command(m_commands.front());
		m_commands.pop_front();
	}

	m_view.setCenter(m_player.getPosition());
}

void Application::process_command(std::pair<std::string, std::string> command)
{
	// Retrieve split URL
	std::deque<std::string> split = utility::split(command.first, '/');

	// Required: API call & correct version
	if(split.empty())
	{
		sf::err() << "Require API call!" << std::endl;
		return;
	}
	else if(split.front() != VERSION)
	{
		sf::err() << "API Version miss match!" << std::endl;
		return;
	}

	split.pop_front();

	for(auto& item : split)
		std::cout << "Split: " << item << std::endl;

	// Extract JSON data
	Json::Reader reader;
	Json::Value data;
	if(!reader.parse(command.second, data))
		sf::err() << "Failed to parse the data" << std::endl << reader.getFormattedErrorMessages() << std::endl;
	else
	{
		std::cout << "Parsed!" << std::endl;
		if(split.front() == "player")
		{
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
