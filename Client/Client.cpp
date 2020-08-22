#pragma comment(lib,"ws2_32.lib")
#pragma warning(disable:4996) 
#include <iostream>
#include <winsock2.h>
#include <Ws2tcpip.h>
using namespace std;

SOCKET Connection;


// function for accepting a message by the server
void ClientHandler();

int main()
{
	WSADATA wsaData;
	WORD DDLVersion = MAKEWORD(2, 1);

	//if good return 0
	if (WSAStartup(DDLVersion, &wsaData) != 0)
	{
		cout << "Error" << endl;
		exit(1);
	}


	SOCKADDR_IN addr; //stores address

	int sizeOfAddr = sizeof(addr);

	addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //stores ip address
	addr.sin_port = htons(1111); // port of program identification by incoming data
	addr.sin_family = AF_INET;


	// create a new socket in the client to connect to the server
	Connection = socket(AF_INET, SOCK_STREAM, NULL);


	// check if the client cannot connect to the server should return 0
    // trying to connect to the server
	//1 - только что созданный сокет
	//2 - указатель на структуру SOCKADDR
	//3 - размер структуры SOCKADDR
	if (connect(Connection, (SOCKADDR*)&addr, sizeof(addr)) != 0)
	{
		cout << "Error , failed connect to server" << endl;
		return 1;
	}
	

	cout << "Connected" << endl; //connected to server


	//char message[256];
	////Connection - socket хранит соединение с сервером от которого прийдет сообщение
	//recv(Connection, message, sizeof(message),NULL);//take message from server
	//
	////выводи на экран наше сообщение
	//cout << message << endl;

	//create thread
	CreateThread(NULL,NULL,(LPTHREAD_START_ROUTINE)ClientHandler,NULL,NULL,NULL);

	char msg[256];


	// read the characters written by the client
	while (true)
	{
		cin.getline(msg,sizeof(msg));


		// send a message
		send(Connection, msg, sizeof(msg),NULL);

		//pause
		Sleep(10);
	}

	return 0;
}

void ClientHandler()
{
	// store the passed string
	char message[256];

	// accept the message and print it to the screen
	while (true)
	{
		//take message from server
		recv(Connection, message, sizeof(message),NULL);
		cout << message << endl;;
	}

}
