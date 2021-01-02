#include "ClientFunctionality.h"
#include<stdio.h>



void Connect() {

}
void Subscribe(char* topic) {

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