#include "ClientFunctionality.h"
#include<stdio.h>



void Connect(SOCKET socket, int port) {

	// create and initialize address structure
	sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
	serverAddress.sin_port = htons(port);
	// connect to server specified in serverAddress and socket connectSocket

	if (connect(socket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
	{
		printf("Unable to connect to server.\n");
		closesocket(socket);
		WSACleanup();
	}
}

void Subscribe(char* topic, SOCKET socket) {
	int iResult = send(socket, topic, (int)strlen(topic) + 1, 0);

	if (iResult == SOCKET_ERROR)
	{
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(socket);
		WSACleanup();
		return;
	}

	printf("Bytes Sent: %ld\n", iResult);
}

void Publish(SOCKET socket, PUBLISHER_MESSAGE publisher_message) {
	
	int iResult = send(socket, (char*)&publisher_message, sizeof(publisher_message), 0);

	if (iResult == SOCKET_ERROR)
	{
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(socket);
		WSACleanup();
		return;
	}

	printf("Bytes Sent: %ld\n", iResult);
}

DWORD WINAPI recieve_function(LPVOID lpParam)
{
	THREAD_PARAMS* params = (THREAD_PARAMS*)lpParam;
	
	while (1)
	{
		int iResult = recv(params->socket, params->buffer, DEFAULT_BUFLEN, 0);
		if (iResult > 0) {
			printf("%s\n", params->buffer);
		}
		else if (iResult == 0)
		{
			// connection was closed gracefully
			printf("Connection with client closed.\n");
			closesocket(params->socket);
		}
		else
		{
			// there was an error during recv
			printf("recv failed with error: %d\n", WSAGetLastError());
			closesocket(params->socket);
		}
	}
}