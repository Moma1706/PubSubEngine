#pragma once
#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <ws2tcpip.h>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>

#define DEFAULT_BUFLEN 512
#define DEFAULT_PUB_PORT 27016
#define DEFAULT_SUB_PORT 27500
#define BUFER_SIZE 50

typedef struct PUBLISHER_MESSAGE_ST {
	char topic[BUFER_SIZE];
	char message[BUFER_SIZE];
} PUBLISHER_MESSAGE;

typedef struct THREAD_PARAMS_ST {
	SOCKET socket;
	char buffer[DEFAULT_BUFLEN];
}THREAD_PARAMS;

 void Connect(SOCKET socket, int port);
 void Subscribe(char* topic, SOCKET socket);
 void Publish(SOCKET socket, PUBLISHER_MESSAGE publisher_message);
 DWORD WINAPI recieve_function(LPVOID lpParam);