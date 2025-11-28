#include "Network.h"

#include "System.h"
#include "Random.h"

#include <SFML/Network.hpp>

#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

class NetworkServer
{
public:
	NetworkServer(Network& network, int clientCount, Random& random)
		: network(network)
	{
		clients.resize(clientCount);
		playerOrder.resize(clientCount + 1);
		playerOrder[0] = 0;
		for (int i = 0; i < clientCount; ++i)
		{
			clients[i].socket = std::make_unique<sf::TcpSocket>();
			playerOrder[i + 1] = i + 1; // Should index at i + 1, right?
		}
		random.shuffle(playerOrder);
	}

	void operator()();

private:
	Network& network;

	struct Client
	{
		std::unique_ptr<sf::TcpSocket> socket;
		bool connected = false;
	};

	std::vector<Client> clients;
	std::vector<int> playerOrder;
	int activePlayerIndex = 0;
};

class NetworkClient
{
public:
	NetworkClient(Network& network, const std::string& serverAddress)
		: network(network)
		, serverAddress(serverAddress)
	{
		serverSocket = std::make_unique<sf::TcpSocket>();
	}

	void operator()();

private:
	Network& network;

	std::string serverAddress;

	std::unique_ptr<sf::TcpSocket> serverSocket;
};

void NetworkServer::operator()()
{
	try
	{
		while (network.run)
		{
			if (!network.connected)
			{
				sf::TcpListener listener;

				// bind the listener to a port
				if (listener.listen(53000) != sf::Socket::Status::Done)
				{
					THROW;
				}

				// accept a new connection
				for (int i = 0; i < clients.size(); ++i)
				{
					Client& client = clients[i];

					while (!client.connected)
					{
						std::cout << "Connecting client " << i << std::endl;

						sf::Socket::Status status = listener.accept(*client.socket);

						std::cout << "Client " << i << " connected with status ";

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

						client.connected = status == sf::Socket::Done;
					}
				}

				network.connected = true;
			}

			while (network.connected)
			{
				Sleep(5000);

				for (int i = 0; i < clients.size() && network.connected; ++i)
				{
					Client& client = clients[i];

					char data[10];
					size_t received = 0;
					sf::Socket::Status status = client.socket->receive(data, 10, received);

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

					client.connected = status == sf::Socket::Done;

					if (!client.connected)
					{
						network.connected = false;
					}
				}
			}
		}
	}
	catch (std::exception& e)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WORD(FOREGROUND_RED | FOREGROUND_INTENSITY));
		std::cerr << std::endl << e.what() << std::endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WORD(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY));
	}
}

void NetworkClient::operator()()
{
	try
	{
		while (network.run)
		{
			while (!network.connected)
			{
				std::cout << "Connecting to server" << std::endl;

				sf::Socket::Status status = serverSocket->connect(sf::IpAddress(serverAddress), 53000);

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

			while (network.connected)
			{
				Sleep(2000);

				const char data[] = { 'g' };
				size_t sent = 0;
				sf::Socket::Status status = serverSocket->send(data, 1, sent);
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

				network.connected = status == sf::Socket::Done;
			}
		}
	}
	catch (std::exception& e)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WORD(FOREGROUND_RED | FOREGROUND_INTENSITY));
		std::cerr << std::endl << e.what() << std::endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WORD(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY));
	}
}

void Network::startServer(int clientCount, Random& random)
{
	connected = false;
	networkThread = std::thread(NetworkServer(*this, clientCount, random));
	networkThreadStarted = true;
}

void Network::startClient(const std::string& serverAddress)
{
	connected = false;
	networkThread = std::thread(NetworkClient(*this, serverAddress));
	networkThreadStarted = true;
}
