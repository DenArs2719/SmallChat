#pragma comment(lib,"ws2_32.lib")
#pragma warning(disable:4996) 
#include <iostream>
#include <winsock2.h>
#include <Ws2tcpip.h>
using namespace std;

SOCKET Connections[100]; //sockets array
int Counter = 0;

//a function that takes the index of the connection in the socket array
void ClientHandler(int index);

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


	SOCKADDR_IN addr; //stores the address

	int sizeOfAddr = sizeof(addr);

	addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //stores ip address
	addr.sin_port = htons(1111); // port of program identification by incoming data
	addr.sin_family = AF_INET;


	//create a socketand assign the result to the socket function
	//AF_INET - Ethernet protocols(интернет протоколы)
	//SOCK_STREAM - указывает на протокол устанавливающий соединение
	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);
	


	//bind an address to a socket
	// первый параметр -  предварительно созданный сокет
	// второй - указатель на структуру SOCKADDR
	// третий - размер стуктуры SOCKADDR
	bind(sListen, (SOCKADDR*)&addr, sizeof(addr));
	

	//when the local address and port are bound to the socket, we start listening on the port to wait for a connection from the client side
	//первый параметр - сокет
	//второй параметр - максимальное допустимое число запросов ожидающих обработки
	listen(sListen, SOMAXCONN);
	

	SOCKET newConnection;
	//keep the connection with the client
	//первый параметр - наш сокет
	//второй параметр - указатель на структуру SOCKADDR
	// третий - сслыка на размер стуктуры SOCKADDR

	for (int i = 0; i < 100; i++)
	{
		//the function returns a pointer to a new socket, which can be used to communicate with the client, if it returns zero, the client could not connect to the server
		newConnection = accept(sListen, (SOCKADDR*)&addr, &sizeOfAddr);

		if (newConnection == 0)
		{
			cout << "Error #2 " << endl;
		}
		else
		{
			cout << "Client Connected" << endl;
			string message = "Hello,Its my first network program ";

			int messageLen = message.size();

			send(newConnection, (char*)&messageLen, sizeof(int), NULL);//send info about message length

			send(newConnection, message.c_str(), messageLen, NULL);//send message to the client
			Connections[i] = newConnection;
			Counter++;

			//create a stream in which we will send a message
			//3 параметр - указатель на функцию 
			//4 параметр - элемент в процедуре
			//Error return  0
			//success - will return the Hadle of the created stream
			CreateThread(NULL,NULL,(LPTHREAD_START_ROUTINE)ClientHandler,(LPVOID)(i),NULL,NULL);

		}
	}
	return 0;
}

void ClientHandler(int index)
{
	int msgLength = 0;

	// receive and send a message
	while (true)
	{
		//we accept package with size
		recv(Connections[index], (char*)&msgLength, sizeof(int), NULL);

		char* msg = new char[msgLength + 1];
		msg[msgLength] = '\0';

		// accepts the message sent by the client
		recv(Connections[index],msg, msgLength,NULL);
		//cout << msg << endl;

		// send a message to all clients, except for the one that sent it
		for (int i = 0; i < 100; i++)
		{
			if (i == index)
			{
				continue;
			}
			
			////send info about message length
			send(Connections[i], (char*)&msgLength, sizeof(int), NULL);

			//send message
			send(Connections[i], msg, msgLength, NULL);
		}

		delete[] msg;
	}
}

