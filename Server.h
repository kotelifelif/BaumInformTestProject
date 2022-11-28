#pragma once
#include <netinet/in.h>

class Server {
public:
    Server(int domain, int type, int port);

    Server(const Server& server) = delete;
    Server& operator=(const Server& server) = delete;

    Server(Server&& server) = default;
    Server& operator=(Server&& server) = default;

    ~Server();

    void SendMessage(const char* data);

    int CreateListener();
    int GetListener() const;

    bool BindAddress();
    bool BindAddress(const sockaddr_in& address);
    sockaddr_in GetAddress() const;

    int AcceptSocket();
    int GetSocket() const;

    void Close() const;
    int Receive(int socket, char* data, const int size) const;

    int GetDomain() const;
    int GetType() const;
    int GetPort() const;
private:
    sockaddr_in address_;
    int listener_;
    int server_socket_;
    int domain_;
    int type_;
    int port_;
};
