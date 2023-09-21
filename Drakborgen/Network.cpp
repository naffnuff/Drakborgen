#include "Network.h"

#include "System.h"

#include <SFML/Network.hpp>

#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

class NetworkRunner
{
public:
	NetworkRunner(Network& network, NetRole netRole)
		: network(network)
		, netRole(netRole)
	{ }

	void operator()();

private:
	void runServer();
	void runClient();

private:
	Network& network;
	NetRole netRole;

	std::vector<std::unique_ptr<sf::TcpSocket>> sockets;
};

void NetworkRunner::operator()()
{
	try
	{
		if (netRole == NetRole::Server)
		{
			runServer();
		} 
		else if (netRole == NetRole::Client)
		{
			runClient();
		}
	}
	catch (std::exception& e)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WORD(FOREGROUND_RED | FOREGROUND_INTENSITY));
		std::cerr << std::endl << e.what() << std::endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WORD(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY));
	}
}

void NetworkRunner::runServer()
{
	while (network.run)
	{
		while (!network.connected)
		{
			sf::TcpListener listener;

			// bind the listener to a port
			if (listener.listen(53000) != sf::Socket::Status::Done)
			{
				THROW;
			}

			// accept a new connection
			for (int i = 0; i < network.clientCount; ++i)
			{
				sockets.push_back(std::make_unique<sf::TcpSocket>());
				sf::Socket::Status status = listener.accept(*sockets[i]);

				std::cout << "Server accepted with status ";

				switch (status)
				{
				case sf::Socket::Done:
					std::cout << "Done";
					break;
				case sf::Socket::NotReady:
					std::cout << "NotReady";
					break;
				case sf::Socket::Partial:
					std::cout << "Partial";
					break;
				case sf::Socket::Disconnected:
					std::cout << "Disconnected";
					break;
				case sf::Socket::Error:
					std::cout << "Error";
					break;
				default:
					break;
				}

				std::cout << std::endl;

				network.connected = status == sf::Socket::Done;
			}
		}

		while (network.connected)
		{
			Sleep(1000);

			for (int i = 0; i < network.clientCount; ++i)
			{
				char data[10];
				size_t received = 0;
				sf::Socket::Status status = sockets[i]->receive(data, 10, received);

				std::cout << "received " << received << " bytes from client " << i << " with status ";

				switch (status)
				{
				case sf::Socket::Done:
					std::cout << "Done, " << data[0] << " was the message";
					break;
				case sf::Socket::NotReady:
					std::cout << "NotReady";
					break;
				case sf::Socket::Partial:
					std::cout << "Partial";
					break;
				case sf::Socket::Disconnected:
					std::cout << "Disconnected";
					break;
				case sf::Socket::Error:
					std::cout << "Error";
					break;
				default:
					break;
				}

				std::cout << std::endl;
			}
		}
	}
}

void NetworkRunner::runClient()
{
	while (network.run)
	{
		while (!network.connected)
		{
			sockets.push_back(std::make_unique<sf::TcpSocket>());
			sf::Socket::Status status = sockets[0]->connect(sf::IpAddress(network.serverIpAddress[0], network.serverIpAddress[1], network.serverIpAddress[2], network.serverIpAddress[3]), 53000);

			std::cout << "Client connected with status ";

			switch (status)
			{
			case sf::Socket::Done:
				std::cout << "Done";
				break;
			case sf::Socket::NotReady:
				std::cout << "NotReady";
				break;
			case sf::Socket::Partial:
				std::cout << "Partial";
				break;
			case sf::Socket::Disconnected:
				std::cout << "Disconnected";
				break;
			case sf::Socket::Error:
				std::cout << "Error";
				break;
			default:
				break;
			}

			std::cout << std::endl;

			network.connected = status == sf::Socket::Done;
		}

		while (network.connected)
		{
			Sleep(1000);

			const char data[] = { 'g' };
			size_t sent = 0;
			sf::Socket::Status status = sockets[0]->send(data, 1, sent);
			std::cout << "sent " << sent << " bytes with status ";

			switch (status)
			{
			case sf::Socket::Done:
				std::cout << "Done";
				break;
			case sf::Socket::NotReady:
				std::cout << "NotReady";
				break;
			case sf::Socket::Partial:
				std::cout << "Partial";
				break;
			case sf::Socket::Disconnected:
				std::cout << "Disconnected";
				break;
			case sf::Socket::Error:
				std::cout << "Error";
				break;
			default:
				break;
			}

			std::cout << std::endl;
		}
	}
}

void Network::start(NetRole netRole)
{
	networkThread = std::thread(NetworkRunner(*this, netRole));
	networkThreadStarted = true;
}
