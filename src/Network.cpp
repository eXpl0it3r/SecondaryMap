#include "Network.hpp"

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

	// Dequeue any pending connections from the listener.
	while(socket = m_listener->GetPendingConnection())
	{
		std::string request = receive_request(socket);

#ifndef NDEBUG
		std::cout << "== Received Request:" << request << std::endl << "==" << std::endl;
#endif

		// No empty requests - Has to start with POST header
		if(!request.empty() && request.find("POST ") == 0)
		{
			// Retrieve tokens
			std::string::size_type tokens_start = request.find("POST ") + 5;
			std::string::size_type tokens_end = request.find(" HTTP");

			std::string tokens = request.substr(tokens_start, tokens_end-tokens_start);

			// Retrieve data
			std::string::size_type data_start = request.find("\r\n\r\n") + 4;

			std::string data = request.substr(data_start);

#ifndef NDEBUG
			std::cout << tokens << "-" << data << std::endl;
#endif

			// Add to the command queue
			if(request != "" && data != "")
				commands.emplace_back(Command{tokens, data});
		}
#ifndef NDEBUG
		else
			std::cout << "Request is either empty or not a POST request." << std::endl;
#endif

		send_responde(socket);

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

	return commands;
}

std::string Network::receive_request(sfn::TcpSocket::Ptr& socket)
{
	std::size_t received_bytes = 0;
	std::stringstream ss;
	char bytes[BLOCK_SIZE] = {0};

	while(socket->BytesToReceive() > received_bytes)
	{
		std::size_t current_bytes = socket->Receive(bytes, BLOCK_SIZE);
		received_bytes += current_bytes;

		for(std::size_t it = 0; it < current_bytes; ++it)
			ss << bytes[it];
	}

	return ss.str();
}

void Network::send_responde(sfn::TcpSocket::Ptr& socket)
{
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
}
