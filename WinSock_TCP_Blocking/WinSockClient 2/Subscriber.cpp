

#include"../Libraries/ClientFunctionality.h"

// Initializes WinSock2 library
// Returns true if succeeded, false otherwise.
bool InitializeWindowsSockets();

int main() 
{
	DWORD SubThredId;
	HANDLE SubThread;

	THREAD_PARAMS thread_params;

    // socket used to communicate with server
    SOCKET connectSocket = INVALID_SOCKET;
    // variable used to store function return value
    int iResult;
    // message to send
	char messageToSend[BUFER_SIZE];
	char recv_bufer[DEFAULT_BUFLEN];

	fd_set readfds;
	FD_ZERO(&readfds);

	// maksimalni period cekanja select funkcije
	timeval timeVal;
	timeVal.tv_sec = 1;
	timeVal.tv_usec = 0;

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

	Connect(connectSocket, DEFAULT_SUB_PORT);
	
	// Send an prepared message with null terminator included
	printf("Enter topic on which you wnat to subscribe: ");
	gets_s(messageToSend, BUFER_SIZE);

	Subscribe(messageToSend, connectSocket);
	
	printf("Messages for %s are: \n", messageToSend);
		
	strcpy(thread_params.buffer, recv_bufer);
	thread_params.socket = connectSocket;

	SubThread = CreateThread(NULL, 0, &recieve_function, &thread_params, 0, &SubThredId);

	printf("If you want to subscribe to new topic press 'N'\n");

	while (1)
	{
		gets_s(messageToSend, BUFER_SIZE);
		if (strcmp(messageToSend,"N") == 0) {

			printf("Enter topic on which you wnat to subscribe: ");
			gets_s(messageToSend, BUFER_SIZE);

			Subscribe(messageToSend, connectSocket);
		}
		if (strcmp(messageToSend, "Q") == 0) {
			break;
		}
	}
	   	
    // cleanup
    closesocket(connectSocket);
    WSACleanup();
	CloseHandle(SubThread);
	
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
