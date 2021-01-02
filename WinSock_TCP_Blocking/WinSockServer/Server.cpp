#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "../Libraries/Hash_Map.h"
#include "../Libraries/list.h"
#include "../Libraries/ServerFuncionality.h"

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27016"
#define MAX_CLIENTS 100
#define BUFER_SIZE 50

typedef struct PUBLISHER_MESSAGE_ST {
	char topic[BUFER_SIZE];
	char message[DEFAULT_BUFLEN];
} PUBLISHER_MESSAGE;

typedef struct SUBSCRIBER_ST {
	char topic[DEFAULT_BUFLEN];
	SOCKET accsocket = INVALID_SOCKET;
} SUBSCRIBER;

bool InitializeWindowsSockets();

int  main(void)
{
	PUBLISHER_MESSAGE* publisher_message;
	SUBSCRIBER* subscriber_struct;
		
	ht_t* publisher_map = ht_create();
	ht_t* subscriber_map = ht_create();
			
	int error;

	// Socket used for listening for new clients 
	SOCKET listenSocket = INVALID_SOCKET;
	// Socket used for communication with client
	SOCKET acceptedSocket[MAX_CLIENTS];
	short lastIndex = 0;
	// variable used to store function return value
	int iResult;
	// Buffer used for storing incoming data
	char recvbuf[DEFAULT_BUFLEN];
	
	fd_set readfds;
	FD_ZERO(&readfds);

	// maksimalni period cekanja select funkcije
	timeval timeVal;
	timeVal.tv_sec = 1;
	timeVal.tv_usec = 0;


	if (InitializeWindowsSockets() == false)
	{
		// we won't log anything since it will be logged
		// by InitializeWindowsSockets() function
		return 1;
	}

	// Prepare address information structures
	addrinfo *resultingAddress = NULL;
	addrinfo hints;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;       // IPv4 address
	hints.ai_socktype = SOCK_STREAM; // Provide reliable data streaming
	hints.ai_protocol = IPPROTO_TCP; // Use TCP protocol
	hints.ai_flags = AI_PASSIVE;     // 

	// Resolve the server address and port
	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &resultingAddress);
	if (iResult != 0)
	{
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	// Create a SOCKET for connecting to server
	listenSocket = socket(AF_INET,      // IPv4 address famly
		SOCK_STREAM,  // stream socket
		IPPROTO_TCP); // TCP

	if (listenSocket == INVALID_SOCKET)
	{
		printf("socket failed with error: %ld\n", WSAGetLastError());
		freeaddrinfo(resultingAddress);
		WSACleanup();
		return 1;
	}

	unsigned long mode = 1;
	iResult = ioctlsocket(listenSocket, FIONBIO, &mode);

	// Setup the TCP listening socket - bind port number and local address 
	// to socket
	iResult = bind(listenSocket, resultingAddress->ai_addr, (int)resultingAddress->ai_addrlen);
	if (iResult == SOCKET_ERROR)
	{
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(resultingAddress);
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}

	// Since we don't need resultingAddress any more, free it
	freeaddrinfo(resultingAddress);

	// Set listenSocket in listening mode
	iResult = listen(listenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR)
	{
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}

	printf("Server initialized, waiting for clients.\n");
	
	while (1) {
		FD_ZERO(&readfds);
		if (lastIndex != MAX_CLIENTS)
		{
			FD_SET(listenSocket, &readfds);
		}

		for (int i = 0; i < lastIndex; i++) {
			if (acceptedSocket[i] != INVALID_SOCKET)
			{

				FD_SET(acceptedSocket[i], &readfds);
			}
		}
		int result = select(0, &readfds, NULL, NULL, &timeVal);

		if (result == 0) {
			
		}
		else if (result == SOCKET_ERROR) {

			printf("listen failed with error: %d\n", WSAGetLastError());
			closesocket(listenSocket);
			WSACleanup();
			return 1;

		}
		else if (FD_ISSET(listenSocket, &readfds))
		{
			acceptedSocket[lastIndex] = accept(listenSocket, NULL, NULL);

			if (acceptedSocket[lastIndex] == INVALID_SOCKET)
			{
				printf("accept failed with error: %d\n", WSAGetLastError());
				closesocket(listenSocket);
				WSACleanup();
				return 1;
			}

			unsigned long mode = 1;
			iResult = ioctlsocket(acceptedSocket[lastIndex], FIONBIO, &mode);
			lastIndex++;
		}
		else
		{
			for (int i = 0; i < lastIndex; i++)
			{
				if (FD_ISSET(acceptedSocket[i], &readfds))
				{
					// Receive data until the client shuts down the connection7
					iResult = recv(acceptedSocket[i], recvbuf, DEFAULT_BUFLEN, 0);

					if (iResult > 0)
					{
						if (iResult > 50) //message forom publisher
						{
							//recieve struct from publisher
							publisher_message = (PUBLISHER_MESSAGE*)recvbuf;
							printf("Message received from Publisher: \nTopic: %s\nMessage: %s \n", publisher_message->topic, publisher_message->message);
						
							ht_set_pub(publisher_map, publisher_message->topic, publisher_message->message); // add new list if there is new key if not add message to the list
							node_t* test = ht_get(publisher_map, publisher_message->topic);
							iterate_list(test);

						}
						else if (iResult <= 50) //message form subscriber
						{
							printf("Message recieved from Subscriber: \nTopic: %s\n", recvbuf);

							node_t* list_to_send = ht_get(publisher_map, recvbuf); // get messages that client asked for

							send_messages_to_client(list_to_send, acceptedSocket[i]);

							printf("Bytes Sent: %ld\n", iResult);
													
							strcpy(subscriber_struct->topic, recvbuf);
							subscriber_struct->accsocket = acceptedSocket[i];

							
							
														
						}
					}
					else if (iResult == 0)
					{
						// connection was closed gracefully
						printf("Connection with client closed.\n");
						closesocket(acceptedSocket[i]);
						// sort array and clean last place
						for (int j = i; j < lastIndex - 1; j++)
						{
							acceptedSocket[j] = acceptedSocket[j + 1];
						}
						acceptedSocket[lastIndex - 1] = 0;

						lastIndex--;
					}
					else
					{
						// there was an error during recv
						printf("recv failed with error: %d\n", WSAGetLastError());
						closesocket(acceptedSocket[i]);
						// sort array and clean last place
						for (int j = i; j < lastIndex - 1; j++)
						{
							acceptedSocket[j] = acceptedSocket[j + 1];
						}
						acceptedSocket[lastIndex - 1] = 0;

						lastIndex--;						
					}

				}
				else
				{
					printf("No message recieved\n");
					Sleep(2000);
				}
			}
		}
	}
		
	// cleanup
	closesocket(listenSocket);
	WSACleanup();

	return 0;
}

bool InitializeWindowsSockets()
{
	WSADATA wsaData;
	// Initialize windows sockets library for this process
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("WSAStartup failed with error: %d\n", WSAGetLastError());
		return false;
	}
	return true;
}


