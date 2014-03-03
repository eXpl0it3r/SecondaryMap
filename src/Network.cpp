#include "Network.hpp"

#include "Application.hpp"

#include <SFNUL/IpAddress.hpp>
#include <SFNUL/Endpoint.hpp>
#include <SFNUL/NetworkResource.hpp>

#include <iostream>

Network::Network()
{
	// Create our TCP listener socket.
	m_listener = sfn::TcpListener::Create();

	// Listen on 0.0.0.0:80
	m_listener->Listen(sfn::Endpoint{ sfn::IpAddress{ "0.0.0.0" }, 3344 });

	// Start 3 network processing threads.
	sfn::Start(3);
}

std::deque<Command> Network::update()
{
	sfn::TcpSocket::Ptr socket;
	std::deque<Command> commands;

	while(socket = m_listener->GetPendingConnection())
	{
		// Receive request.
		receive_request(socket, commands);
		// Send the HTTP response.
		send_responde(socket);
	}

	return commands;
}

void Network::receive_request(sfn::TcpSocket::Ptr& socket, std::deque<Command>& commands)
{
	std::string data;
	std::string header;
	std::string token;
	std::string number;
	std::string body;

	std::size_t content_length;

	bool received = false;

	sf::Clock clock;

	while(!socket->RemoteHasShutdown() && !received && clock.getElapsedTime() <= sf::milliseconds(TIMEOUT))
	{
		while(socket->BytesToReceive() > 0)
		{
			char bytes[BLOCK_SIZE] = {0};

			std::size_t current_bytes = 0;

			while((current_bytes = socket->Receive(bytes, BLOCK_SIZE)))
			{
				data.append(bytes, current_bytes);
			}

			// Filter header
			if(header.size() == 0)
			{
				std::size_t h_end = data.find("\r\n\r\n");

				if(h_end != std::string::npos)
				{
					header = data.substr(0, h_end + 4);

					// Filter Content-Length
					std::stringstream ss;
					std::string search = "Content-Length: ";

					std::size_t l_begin = header.find(search) + search.size();
					std::size_t l_end = header.find("\n", l_begin);

					ss << header.substr(l_begin, l_end);
					ss >> content_length;

					// Filter token
					std::size_t token_start = header.find("POST ") + 5;
					std::size_t token_end = header.find(" HTTP");

					token = header.substr(token_start, token_end-token_start);
				}
			}
			// Filer body
			if(body.size() == 0)
			{
				if(data.size() >= header.size() + content_length)
				{
					body = data.substr(header.size(), content_length);
					received = true;

					// Add to the command queue
					commands.push_back(Command{token, body});
				}
			}
		}
	}

	std::cout << "--=== Logging ===--" << std::endl;
	std::cout << header << std::endl << std::endl;
	std::cout << body << std::endl << std::endl;
}

void Network::send_responde(sfn::TcpSocket::Ptr& socket)
{
	std::string response{
		"HTTP/1.1 200 OK\r\n"
		"Server: SFNUL HTTP Server\r\n"
		"Content-Type: application/json; charset=UTF-8\r\n"
		"Connection: Close\r\n\r\n"
		"{\"version\":\"" + Application::VERSION + "\"}\r\n\r\n"};

	// Send the HTTP response.
	socket->Send(response.c_str(), response.size());
	// Turn of connection lingering.
	socket->SetLinger(0);
	// Shutdown the socket for sending.
	socket->Shutdown();
}
