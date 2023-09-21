#pragma once

#include <thread>
#include <array>
#include <iostream>

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
		std::cout << "Network destructor start" << std::endl;

		run = false;
		if (networkThreadStarted)
		{
			networkThread.join();
		}

		std::cout << "Network destructor end" << std::endl;
	}

	void start(NetRole netRole);

	void setClientCount(int count)
	{
		clientCount = count;
	}

public:
	std::array<int, 4> serverIpAddress = { 192, 168, 1, 121 };

private:
	bool networkThreadStarted = false;

	bool run = true;

	bool connected = false;

	int clientCount = 0;

	std::thread networkThread;

	friend class NetworkRunner;
};