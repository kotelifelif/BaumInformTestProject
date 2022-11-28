#include "Server.h"
#include "Client.h"

#include <string>
#include <thread>


using namespace std;


void ProcessClient(int socket, Client& client, Server& server) {
	while (true) {
		char command[1024];
		string command_str;
		int bytes_read = server.Receive(socket, command, 1024);
		if (bytes_read > 0) {
			command_str = command;
			client.ProcessCommand(command_str);
			if (command_str == "export seq") {
				server.SendMessage(client.GetMessage().c_str());
				server.Close();
				break;
			}
		}
	}
}

// Google style guide
int main() {
	constexpr int kPort = 8081;
	Server server(AF_INET, SOCK_STREAM, kPort);
	server.CreateListener();

	if (!server.BindAddress()) {
		return -1;
	}

	int socket;
	while (true) {
		socket = server.AcceptSocket();
		Client client;
		thread client_thread(ProcessClient, socket, ref(client), ref(server));
		client_thread.detach();
	}
}