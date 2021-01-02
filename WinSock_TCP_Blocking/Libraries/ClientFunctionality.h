#pragma once
#include <ws2tcpip.h>

typedef struct PUBLISHER_MESSAGE_ST {
	char topic[50];
	char message[50];
} PUBLISHER_MESSAGE;

 void Connect();
 void Subscribe(char* topic);
 void Publish(SOCKET socket, PUBLISHER_MESSAGE publisher_message);