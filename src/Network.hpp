#pragma once

#include "Command.hpp"

#include <SFNUL/TcpListener.hpp>
#include <SFNUL/TcpSocket.hpp>

#include <string>
#include <deque>

class Network
{
public:
	Network();

	std::deque<Command> update();

private:
	std::string receive_request(sfn::TcpSocket::Ptr& socket);
	void send_responde(sfn::TcpSocket::Ptr& socket);
	void process_request(const std::string& request, std::deque<Command>& commands);

private:
	static const unsigned int BLOCK_SIZE = 1024;

	sfn::TcpListener::Ptr m_listener;
	std::deque<sfn::TcpSocket::Ptr> m_sockets;
};
