#pragma once

#include <thread>
#include <array>
#include <iostream>

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

	void startServer(int clientCount, class Random& random);
	void startClient(const std::string& serverAddress);

	bool isConnected() const
	{
		return connected;
	}

private:
	bool networkThreadStarted = false;

	bool run = true;

	bool connected = true;

	std::thread networkThread;

	friend class NetworkServer;
	friend class NetworkClient;
};