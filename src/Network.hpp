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
	void receive_request(sfn::TcpSocket::Ptr& socket, std::deque<Command>& commands);
	void send_responde(sfn::TcpSocket::Ptr& socket);

private:
	static const unsigned int BLOCK_SIZE = 8192;
	static const unsigned int TIMEOUT = 500;

	sfn::TcpListener::Ptr m_listener;
	std::deque<sfn::TcpSocket::Ptr> m_sockets;
};
