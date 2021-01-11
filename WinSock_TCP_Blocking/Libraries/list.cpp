#include"list.h"
#include"Hash_Map.h"

void init_list(node_t** head) 
{
	//InitializeCriticalSection(&cs_list_pub);
	//EnterCriticalSection(&cs_list_pub);
	*head = NULL;
	//LeaveCriticalSection(&cs_list_pub);
}

void init_list_sub(node_s_t** head)
{
	//InitializeCriticalSection(&cs_list_sub);
	//EnterCriticalSection(&cs_list_pub);
	*head = NULL;
	//LeaveCriticalSection(&cs_list_pub);
}

void push_to_the_end(node_t** head, char* message) 
{
	node_t* new_element = (node_t*)malloc(sizeof(node_t));
		
	strcpy(new_element->message, message);
	 
	new_element->next = NULL;

	if (*head == NULL) {
		//EnterCriticalSection(&cs_list_pub);
		*head = new_element;
		//LeaveCriticalSection(&cs_list_pub);
		return;
	}

	node_t* temp = *head;
	while (temp->next != NULL) {
		temp = temp->next;
	}
	temp->next = new_element;
}

void push_to_the_end_sub(node_s_t** head, SOCKET socket)
{
	node_s_t* new_element = (node_s_t*)malloc(sizeof(node_s_t));

	new_element->socket = socket;

	new_element->next = NULL;

	if (*head == NULL) {
		//EnterCriticalSection(&cs_list_pub);
		*head = new_element;
		//LeaveCriticalSection(&cs_list_pub);
		return;
	}

	node_s_t* temp = *head;
	while (temp->next != NULL) {
		temp = temp->next;
	}
	temp->next = new_element;
}

void iterate_list(node_t* head) {
	node_t* current = head;

	while (current != NULL) {
		printf("%s\n", current->message);
		current = current->next;
	}
}

void iterate_list_sub(node_s_t* head) {
	node_s_t* current = head;

	while (current != NULL) {
		printf("%d\n", current->socket);
		current = current->next;
	}
}

void remove_sub(node_s_t** head, SOCKET socket) {

	node_s_t* current = *head;
	node_s_t* prev = *head;
	
	if (current == NULL)
	{
		return;
	}

	while (current != NULL)
	{
		if (current->socket == socket)
		{
			prev->next = current->next;
			free(current);
			return;
		}
		prev = current;
		current = current->next;
	}



}