#pragma comment(lib,"ws2_32.lib")
#pragma warning(disable:4996) 
#include <iostream>
#include <winsock2.h>
#include <Ws2tcpip.h>
using namespace std;

SOCKET Connections[100]; //sockets array
int Counter = 0;

//функция которая принимает индекс соединения в сокет массиве
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


	SOCKADDR_IN addr; //хранит адресс

	int sizeOfAddr = sizeof(addr);

	addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //хранит ip адрес
	addr.sin_port = htons(1111); // порт индентификации программы поступающими данными
	addr.sin_family = AF_INET;


	//создаем сокет и присваем результат функции сокет .
	//AF_INET -семейство интренет протоколов
	//SOCK_STREAM - указывает на протокол устанавливающий соединение
	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);
	


	//привязываем адрес сокету
	// первый параметр -  предварительно созданный сокет
	// второй - указатель на структуру SOCKADDR
	// третий - размер стуктуры SOCKADDR
	bind(sListen, (SOCKADDR*)&addr, sizeof(addr));
	

	//когда локальный адрес и порт привязаны к сокету ,приступаем  к прослушиванию порта ,для ожидания соединения со стороны клиента
	//первый араметр - сокет
	//второй параметр - максимальное допустимое число запросов ожидающих обработки
	listen(sListen, SOMAXCONN);
	

	SOCKET newConnection;
	//удерживаем соедниенение с клиентом
	//первый параметр - наш сокет
	//второй параметр - указатель на структуру SOCKADDR
	// третий - сслыка на размер стуктуры SOCKADDR

	for (int i = 0; i < 100; i++)
	{
		//функция возвращает указатель на новый сокет,который можно использовать для общения с клиентом,если вернет ноль,то клиент не смог подключиться к серверу
		newConnection = accept(sListen, (SOCKADDR*)&addr, &sizeOfAddr);

		if (newConnection == 0)
		{
			cout << "Error #2 " << endl;
		}
		else
		{
			cout << "Client Connected" << endl;
			char message[256] = "Hello,Its my first network program ";
			send(newConnection, message, sizeof(message), NULL);//send message to the client
			Connections[i] = newConnection;
			Counter++;

			//создаем поток в котором будем отправлять сообщение
			//3 параметр - указатель на функцию 
			//4 параметр - элемент в процедуре
			//ошибка вернет 0
			//успех - вернет Hadle созданного потока
			CreateThread(NULL,NULL,(LPTHREAD_START_ROUTINE)ClientHandler,(LPVOID)(i),NULL,NULL);

		}
	}
	return 0;
}

void ClientHandler(int index)
{
	char msg[256];

	//получение и отправление сообщения
	while (true)
	{
		//принимает сообщение отправленное клиетом
		recv(Connections[index],msg,sizeof(msg),NULL);

		//отправляем сообщение всем клиентам,кроме того,который его отправил
		for (int i = 0; i < 100; i++)
		{
			if (i == index)
			{
				continue;
			}
			
			send(Connections[i], msg, sizeof(msg), NULL);
		}
	}
}

