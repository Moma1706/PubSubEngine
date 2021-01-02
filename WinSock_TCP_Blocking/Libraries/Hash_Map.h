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

//hash function return hash value of key
extern unsigned int hash(const char* key);  

//ht_par function alocates mamory and adds key value par to map
extern entry_t* ht_pair(const char* key, node_t* value);
extern entry_s_t* ht_pair_sub(const char* key, node_s_t* value);

// create function initialize map
extern ht_t* ht_create(void);
extern ht_s_t* ht_create_sub(void);

//check if there is an elemet with taht key if  yes add to list if not create list and add element 
extern void ht_set_pub(ht_t* hashtable, const char* key, char* value);
extern void ht_set_sub(ht_t* hashtable, const char* key, SOCKET socket);

//get element by key returns list of elements whitch are plased under the same key
extern node_t* ht_get(ht_t* hashtable, const char* key);
extern node_s_t* ht_get_sub(ht_t* hashtable, const char* key);