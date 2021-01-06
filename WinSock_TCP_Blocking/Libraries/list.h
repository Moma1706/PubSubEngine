#pragma once
#pragma warning(disable:4996)
#include<stdio.h>
#include<stdlib.h>
#include <ws2tcpip.h>

#include "list.h"

#define MESSAGE_SIZE 512

typedef struct node {	//struct used for publisher 
	char message[MESSAGE_SIZE];
	struct node* next;
} node_t;

typedef struct node_s {	//struct used for subscriber
	SOCKET socket;
	struct node_s* next;
} node_s_t;

//CRITICAL_SECTION cs_list_pub;
//CRITICAL_SECTION cs_list_sub;

// create list
void init_list(node_t** head);
void init_list_sub(node_s_t** head);

//add element to the ebd of the list
void push_to_the_end(node_t** head, char* val);
void push_to_the_end_sub(node_s_t** head, SOCKET socket);

//iterate and print list
void iterate_list(node_t* head);
void iterate_list_sub(node_s_t* head);
