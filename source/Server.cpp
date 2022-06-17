#include <iostream>
#include <string>
#include <winsock2.h>
#include <Windows.h>

#pragma comment(lib, "ws2_32.lib")

#pragma warning(disable: 4996)

class Server {
	//WSAStartup
public:
	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);
	SOCKET sListen;
	SOCKET serverSock;
	SOCKADDR_IN addr;
	int sizeofaddr;
	Server() {
		WSAStartup(DLLVersion, &wsaData);
		sizeofaddr = sizeof(addr);
		addr.sin_addr.s_addr = inet_addr("127.0.0.1");
		addr.sin_port = htons(1111);
		addr.sin_family = AF_INET;

		sListen = socket(AF_INET, SOCK_STREAM, 0);

		bind(sListen, (LPSOCKADDR)&addr, sizeof(addr));
		listen(sListen, 10);

		serverSock = accept(sListen, (SOCKADDR*)&addr, &sizeofaddr);
	}

	~Server() {
		closesocket(sListen);
		closesocket(serverSock);
		WSACleanup();
	}

	void sendToClient(char* msg) {
		send(serverSock, msg, sizeof(msg), 1);
	}
};

int main() {
	std::cout << "start\n";
	Server server;

	char message[10] = "hello";
	while (1) {
		std::cin >> message;
		server.sendToClient(message);
	}

	return 0;
}
