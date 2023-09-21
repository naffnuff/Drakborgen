#pragma once

#include <thread>
#include <array>

enum class NetRole
{
	Server,
	Client
};

class Network
{
public:
	Network() {}

	~Network()
	{
		run = false;
		if (networkThreadStarted)
		{
			networkThread.join();
		}
	}

	void start(NetRole netRole);

	void setClientCount(int count)
	{
		clientCount = count;
	}

private:
	bool networkThreadStarted = false;

	bool run = true;

	bool connected = false;

	int clientCount = 0;

	std::array<sf::Uint8, 4> serverIpAddress = { 192, 168, 1, 121 };

	std::thread networkThread;

	friend class NetworkRunner;
};