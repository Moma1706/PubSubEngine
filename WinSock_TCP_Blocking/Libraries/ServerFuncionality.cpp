#include "ServerFuncionality.h"

DWORD WINAPI thread_function(LPVOID lpParam) {

	SUB_PARAMS* params = (SUB_PARAMS*)lpParam;

	SOCKET listenSocket = INVALID_SOCKET;

	PUBLISHER_MESSAGE* publisher_message;

	// Socket used for communication with client
	SOCKET acceptedSocket[MAX_CLIENTS];
	short lastIndex = 0;
	int iResult;
	// Buffer used for storing incoming data
	char recvbuf[DEFAULT_BUFLEN];

	if (InitializeWindowsSockets() == false)
	{
		// we won't log anything since it will be logged
		// by InitializeWindowsSockets() function
		return 1;
	}

	// Prepare address information structures
	addrinfo* resultingAddress = NULL;
	addrinfo hints;

	fd_set readfds;
	FD_ZERO(&readfds);

	// maksimalni period cekanja select funkcije
	timeval timeVal;
	timeVal.tv_sec = 1;
	timeVal.tv_usec = 0;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;       // IPv4 address
	hints.ai_socktype = SOCK_STREAM; // Provide reliable data streaming
	hints.ai_protocol = IPPROTO_TCP; // Use TCP protocol
	hints.ai_flags = AI_PASSIVE;     // 

	
	// Resolve the server address and port
	iResult = getaddrinfo(NULL, params->port, &hints, &resultingAddress);
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

							ht_set_pub(params->publisher_map, publisher_message->topic, publisher_message->message); // add new list if there is new key if not add message to the list

							node_s_t* list_subs = ht_get_sub(params->subscriber_map, publisher_message->topic);

							if (list_subs != NULL)
							{
								send_message_to_clients(publisher_message->message, list_subs);
							}
						}

						if (iResult <= 50) //message form subscriber
						{
							printf("Message recieved from Subscriber: \nTopic: %s\n", recvbuf);

							ht_set_sub(params->subscriber_map, recvbuf, acceptedSocket[i]); //add new sub to the map or add new topic 

							node_t* list_to_send = ht_get(params->publisher_map, recvbuf); // get messages that client asked for

							send_messages_to_client(list_to_send, acceptedSocket[i]);

							printf("Bytes Sent: %ld\n", iResult);
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

	return 1;
}

void send_messages_to_client(node_t* head, SOCKET acceptedSocket)
{
	node_t* current = head;

	while (current != NULL) {

		Sleep(200);
		int iResult = send(acceptedSocket, current->message, (int)strlen(current->message)+1, 0);

		if (iResult == SOCKET_ERROR)
		{
			printf("send failed with error: %d\n", WSAGetLastError());
			closesocket(acceptedSocket);
			WSACleanup();
			break;
		}
		current = current->next;
	}
}

void send_message_to_clients(char* message, node_s_t* head) {
	
	node_s_t* current = head;

	while (current != NULL)
	{
		Sleep(200);
		int iResult = send(current->socket, message, (int)strlen(message) + 1, 0);

		if (iResult == SOCKET_ERROR)
		{
			printf("send failed with error: %d\n", WSAGetLastError());
			closesocket(current->socket);
			WSACleanup();
			break;
		}
		current = current->next;
	}

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