#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include"list.h"
#include "Hash_Map.h"

#define DEFAULT_BUFLEN 512
#define DEFAULT_PUB_PORT "27016"
#define DEFAULT_SUB_PORT "27500"
#define MAX_CLIENTS 100
#define BUFER_SIZE 50

typedef struct PUBLISHER_MESSAGE_ST {
	char topic[BUFER_SIZE];
	char message[DEFAULT_BUFLEN];
} PUBLISHER_MESSAGE;

typedef struct SUB_PARAMS_ST {
	ht_t* publisher_map;
	ht_s_t* subscriber_map;
	char* port;
}SUB_PARAMS;

typedef struct PUB_PARAMS_ST {
	ht_t* publisher_map;
	ht_s_t* subscriber_map;
	char* port;
}PUB_PARAMS;

// function used for listening for subs in subthread
DWORD WINAPI thread_function(LPVOID lpParam);


//send all messages with that topic to one client
void send_messages_to_client(node_t* head, SOCKET acceptedSocket); 

//send current message to all clients subscribed to cureent topic
void send_message_to_clients(char* message, node_s_t* head);

bool InitializeWindowsSockets();

