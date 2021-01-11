#include<limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#define TABLE_SIZE 20000

typedef struct entry_t {   //struct for publishers
	char* key;
	node_t* value;
	struct entry_t* next;
} entry_t;

typedef struct {	
	entry_t** entries;
} ht_t;


typedef struct entry_s_t { //struct for subsribers
	char* key;
	node_s_t* value;
	struct entry_s_t* next;
} entry_s_t;

typedef struct {
	entry_s_t** entries;
} ht_s_t;


typedef struct entry_socket {  //struct used for storing subscribers with socket as a key and topic as a value
	SOCKET key;
	char* value;
	entry_socket* next;
};

typedef struct {
	entry_socket** entries;
} ht_socket_t;


//CRITICAL_SECTION cs_map_pub;

//hash function return hash value of key
extern unsigned int hash(const char* key);  

//ht_par function alocates mamory and adds key value par to map
extern entry_t* ht_pair(const char* key, node_t* value);
extern entry_s_t* ht_pair_sub(const char* key, node_s_t* value);
extern entry_socket* ht_pair_socket(SOCKET key, char* value);

// create function initialize map
extern ht_t* ht_create(void);
extern ht_s_t* ht_create_sub(void);
extern ht_socket_t* ht_create_socket(void);

//check if there is an elemet with taht key if  yes add to list if not create list and add element 
extern void ht_set_pub(ht_t* hashtable, const char* key, char* value);
extern void ht_set_sub(ht_s_t* hashtable, const char* key, SOCKET socket);
extern void ht_set_socket(ht_socket_t* hashtable, SOCKET key, char* value);

//get element by key returns list of elements whitch are plased under the same key
extern node_t* ht_get(ht_t* hashtable, const char* key);
extern node_s_t* ht_get_sub(ht_s_t* hashtable, const char* key);
extern char* ht_get_topic(ht_socket_t* hashtable, SOCKET key);