
#include"../Libraries/ClientFunctionality.h"

// Initializes WinSock2 library
// Returns true if succeeded, false otherwise.
bool InitializeWindowsSockets();

int  main() 
{
    // socket used to communicate with server
    SOCKET connectSocket = INVALID_SOCKET;
    
	// struct uset to send topic and message
	PUBLISHER_MESSAGE publisher_message;

	// variable used to store function return value
    int iResult;

    if(InitializeWindowsSockets() == false)
    {
		// we won't log anything since it will be logged
		// by InitializeWindowsSockets() function
		return 1;
    }

    // create a socket
    connectSocket = socket(AF_INET,
                           SOCK_STREAM,
                           IPPROTO_TCP);

    if (connectSocket == INVALID_SOCKET)
    {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

	Connect(connectSocket, DEFAULT_PUB_PORT);

	while(1)
    {
        printf("Enter topic: ");
        gets_s(publisher_message.topic, BUFER_SIZE);
        printf("Enter message: ");
        gets_s(publisher_message.message, BUFER_SIZE);

		Publish(connectSocket, publisher_message);
	}

    // cleanup
    closesocket(connectSocket);
    WSACleanup();

    return 0;
}

bool InitializeWindowsSockets()
{
    WSADATA wsaData;
	// Initialize windows sockets library for this process
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0)
    {
        printf("WSAStartup failed with error: %d\n", WSAGetLastError());
        return false;
    }
	return true;
}
