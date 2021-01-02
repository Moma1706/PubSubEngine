#include "ServerFuncionality.h"

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
