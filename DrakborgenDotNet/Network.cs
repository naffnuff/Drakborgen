using Drakborgen;
using System.Net;
using System;
using SFML.Audio;
using static System.Runtime.InteropServices.JavaScript.JSType;
using static Drakborgen.Network;

namespace Drakborgen
{
    internal class Network : IDisposable
    {
        private bool _run = true;
        private Thread? _networkThread;
        private INetworkHost _networkHost;
        private bool _disposed = false;

        public bool IsConnected { get; private set; }

        public Network()
        {
            // Start the network thread
            /*_networkThread = new Thread(NetworkThreadProc)
            {
                IsBackground = true
            };
            _networkThread.Start();
            Console.WriteLine("Network thread started.");*/
        }

        ~Network()
        {
            Dispose(false); // Finalizer calls Dispose(false) for unmanaged cleanup
        }

        // IDisposable implementation
        void IDisposable.Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        protected virtual void Dispose(bool disposing)
        {
            if (!_disposed)
            {
                Console.WriteLine("Network Dispose start");

                if (disposing)
                {
                    // Signal the thread to stop
                    _run = false;

                    // If the thread is running, wait for it to finish
                    if (_networkThread != null && _networkThread.IsAlive)
                    {
                        _networkThread.Join();
                        _networkThread = null;
                    }
                }

                Console.WriteLine("Network Dispose end");
                _disposed = true;
            }
        }

        private class TcpSocket
        {
            internal enum Status
            {
                Done
            }

            internal Status Receive(char[] data, int v, int received)
            {
                throw new NotImplementedException();
            }

            internal Status Connect(IpAddress ipAddress, int v)
            {
                throw new NotImplementedException();
            }

            internal Status Send(char[] data, int v, int sent)
            {
                throw new NotImplementedException();
            }
        }
        private class TcpListener
        {
            internal TcpSocket.Status Listen(int v)
            {
                throw new NotImplementedException();
            }

            internal TcpSocket.Status Accept(TcpSocket socket)
            {
                throw new NotImplementedException();
            }
        }
        private class IpAddress
        {
            private string _serverAddress;

            public IpAddress(string serverAddress)
            {
                _serverAddress = serverAddress;
            }
        }

        internal interface INetworkHost
        {
            void Run();
        }

        private class NetworkServer : INetworkHost
        {
            private record Client(TcpSocket Socket, bool IsConnected = false)
            {
                internal bool IsConnected { get; set; } = IsConnected;
            }

            Network _network;

            Client[] _clients;
            int[] _playerOrder;
            int _activePlayerIndex = 0;

            internal NetworkServer(Network network, int clientCount, Random random)
            {
                _network = network;
                _clients = new Client[clientCount];
                _playerOrder = new int[clientCount + 1];
                _playerOrder[0] = 0;
                for (int i = 0; i < clientCount; ++i)
                {
                    _clients[i] = new Client(new TcpSocket());
                    _playerOrder[i + 1] = i + 1; // Should index at i + 1, right?
                }
                random.Shuffle<int>(_playerOrder);
            }

            public void Run()
            {
                try
                {
                    while (_network._run)
                    {
                        if (!_network.IsConnected)
                        {
                            TcpListener listener = new TcpListener();

                            // bind the listener to a port
                            if (listener.Listen(53000) != TcpSocket.Status.Done)
                            {
                                throw new Exception();
                            }

                            // accept a new connection
                            for (int i = 0; i < _clients.Length; ++i)
                            {
                                Client client = _clients[i];

                                while (!client.IsConnected)
                                {
                                    Console.WriteLine("Connecting client " + i);

                                    TcpSocket.Status status = listener.Accept(client.Socket);

                                    Console.WriteLine("Client " + i + " connected with status " + Enum.GetName(status));

                                    client.IsConnected = status == TcpSocket.Status.Done;
                                }
                            }

                            _network.IsConnected = true;
                        }

                        while (_network.IsConnected)
                        {
                            Thread.Sleep(5000);

                            for (int i = 0; i < _clients.Length && _network.IsConnected; ++i)
                            {
                                Client client = _clients[i];

                                char[] data = new char[10];
                                int received = 0;
                                TcpSocket.Status status = client.Socket.Receive(data, 10, received);

                                Console.WriteLine("received " + received + " bytes from client " + i + " with status " + Enum.GetName(status) + ", " + data + " was the message");

                                client.IsConnected = status == TcpSocket.Status.Done;

                                if (!client.IsConnected)
                                {
                                    _network.IsConnected = false;
                                }
                            }
                        }
                    }
                }
                catch (Exception e)

                {
                    Console.Error.WriteLine(e.ToString());
                }
            }
        }

        private class NetworkClient : INetworkHost
        {
            private Network _network;
            private string _serverAddress;
            private TcpSocket _serverSocket;

            internal NetworkClient(Network network, string serverAddress)
            {
                _network = network;
                _serverAddress = serverAddress;
                _serverSocket = new TcpSocket();
            }

            public void Run()
            {
                try
                {
                    while (_network._run)
                    {
                        while (!_network.IsConnected)
                        {
                            Console.WriteLine("Connecting to server");

                            TcpSocket.Status status = _serverSocket.Connect(new IpAddress(_serverAddress), 53000);

                            Console.WriteLine("Server accepted with status " + Enum.GetName(status));

                            _network.IsConnected = status == TcpSocket.Status.Done;
                        }

                        while (_network.IsConnected)
                        {
                            Thread.Sleep(2000);

                            char[] data = ['g'];
                            int sent = 0;
                            TcpSocket.Status status = _serverSocket.Send(data, 1, sent);
                            Console.WriteLine("sent " + sent + " bytes with status " + Enum.GetName(status));

                            _network.IsConnected = status == TcpSocket.Status.Done;
                        }
                    }
                }
                catch (Exception e)

                {
                    Console.Error.WriteLine(e.ToString());
                }

            }
        }

        internal void StartServer(int clientCount, Random random)
        {
            IsConnected = false;
            _networkHost = new NetworkServer(this, clientCount, random);
            _networkThread = new Thread(() => _networkHost.Run());
        }

        internal void StartClient(string serverAddress)
        {
            IsConnected = false;
            _networkHost = new NetworkClient(this, serverAddress);
            _networkThread = new Thread(() => _networkHost.Run());
        }
    }
}