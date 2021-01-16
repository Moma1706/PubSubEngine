#include"list.h"
#include"Hash_Map.h"

void init_list(node_t** head) 
{
	*head = NULL;
}

void init_list_sub(node_s_t** head)
{
	*head = NULL;
}

void push_to_the_end(node_t** head, char* message) 
{
	node_t* new_element = (node_t*)malloc(sizeof(node_t));
		
	strcpy(new_element->message, message);
	 
	new_element->next = NULL;

	if (*head == NULL) {
		*head = new_element;
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
		*head = new_element;
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

	node_s_t* prev = *head;
	node_s_t* temp = prev->next;
		
	if (prev == NULL)
	{
		return;
	}

	if (prev->socket == socket)
	{
		*head = temp;
		free(prev);
		return;
	}

	while (temp != NULL)
	{
		if (temp->socket == socket)
		{
			prev->next = temp->next;
			free(temp);
			return;
		}
		prev = temp;
		temp = temp->next;
	}
	
	return;

}