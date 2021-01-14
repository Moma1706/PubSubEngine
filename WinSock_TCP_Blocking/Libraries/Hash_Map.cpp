#include"Hash_Map.h"

CRITICAL_SECTION cs_map_pub;
CRITICAL_SECTION cs_map_sub;
CRITICAL_SECTION cs_map_socket;

unsigned int hash(const char* key) {
	unsigned long int value = 0;
	unsigned int i = 0;
	unsigned int key_len = strlen(key);

	// do several rounds of multiplication
	for (; i < key_len; ++i) {
		value = value * 37 + key[i];
	}

	// make sure value is 0 <= value < TABLE_SIZE
	value = value % TABLE_SIZE;

	return value;
}

entry_t* ht_pair(const char* key,  node_t* value) {
	// allocate the entry
	entry_t* entry = (entry_t*)malloc(sizeof(entry_t) * 1);
	entry->key = (char*)malloc(strlen(key) + 1);
	entry->value = (node_t*)malloc(sizeof(node_t) + 1);

	EnterCriticalSection(&cs_map_pub);
	// copy the key and value in place
	strcpy(entry->key, key);
	entry->value = value;
	LeaveCriticalSection(&cs_map_pub);

	// next starts out null but may be set later on
	entry->next = NULL;

	return entry;
}

entry_s_t* ht_pair_sub(const char* key, node_s_t* value) {
	// allocate the entry
	entry_s_t* entry = (entry_s_t*)malloc(sizeof(entry_s_t) * 1);
	entry->key = (char*)malloc(strlen(key) + 1);
	entry->value = (node_s_t*)malloc(sizeof(node_s_t) + 1);

	EnterCriticalSection(&cs_map_sub);
	// copy the key and value in place
	strcpy(entry->key, key);
	entry->value = value;
	LeaveCriticalSection(&cs_map_sub);

	// next starts out null but may be set later on
	entry->next = NULL;

	return entry;
}

entry_socket* ht_pair_socket(SOCKET key, char* value) {
	// allocate the entry
	entry_socket* entry = (entry_socket*)malloc(sizeof(entry_socket) * 1);
	entry->key = (SOCKET)malloc(sizeof(key) + 1);
	entry->value = (char*)malloc(strlen(value) + 1);

	EnterCriticalSection(&cs_map_socket);
	// copy the key and value in place
	entry->key = key;
	strcpy(entry->value, value);
	LeaveCriticalSection(&cs_map_socket);

	// next starts out null but may be set later on
	entry->next = NULL;

	return entry;
}

ht_t* ht_create(void) {

	InitializeCriticalSection(&cs_map_pub);
	// allocate table
	ht_t* hashtable = (ht_t*)malloc(sizeof(ht_t) * 1);

	// allocate table entries
	hashtable->entries = (entry_t**)malloc(sizeof(entry_t*) * TABLE_SIZE);

	// set each to null (needed for proper operation)
	int i = 0;
	EnterCriticalSection(&cs_map_pub);
	for (; i < TABLE_SIZE; ++i) {
		hashtable->entries[i] = NULL;
	}
	LeaveCriticalSection(&cs_map_pub);

	return hashtable;
}

ht_s_t* ht_create_sub(void) {
	InitializeCriticalSection(&cs_map_sub);
	// allocate table
	ht_s_t* hashtable = (ht_s_t*)malloc(sizeof(ht_s_t) * 1);

	// allocate table entries
	hashtable->entries = (entry_s_t**)malloc(sizeof(entry_s_t*) * TABLE_SIZE);

	// set each to null (needed for proper operation)
	int i = 0;
	EnterCriticalSection(&cs_map_sub);
	for (; i < TABLE_SIZE; ++i) {
		hashtable->entries[i] = NULL;
	}
	LeaveCriticalSection(&cs_map_sub);

	return hashtable;
}


ht_socket_t* ht_create_socket(void) {
	InitializeCriticalSection(&cs_map_socket);
	// allocate table
	ht_socket_t* hashtable = (ht_socket_t*)malloc(sizeof(ht_socket_t) * 1);

	// allocate table entries
	hashtable->entries = (entry_socket**)malloc(sizeof(entry_socket*) * TABLE_SIZE);

	// set each to null (needed for proper operation)
	int i = 0;
	EnterCriticalSection(&cs_map_socket);
	for (; i < TABLE_SIZE; ++i) {
		hashtable->entries[i] = NULL;
	}
	LeaveCriticalSection(&cs_map_socket);

	return hashtable;
}

void ht_set_pub(ht_t* hashtable, const char* key, char* message) {
	unsigned int slot = hash(key);

	// try to look up an entry set
	EnterCriticalSection(&cs_map_pub);
	entry_t* entry = hashtable->entries[slot];
	LeaveCriticalSection(&cs_map_pub);
	
	// no entry means slot empty, insert immediately
	if (entry == NULL) {
		
		node_t* list;
		init_list(&list);
		push_to_the_end(&list, message);

		hashtable->entries[slot] = ht_pair(key, list);
		
		return;
	}

	entry_t* prev;

	// walk through each entry until either the end is
	// reached or a matching key is found
	while (entry != NULL) {
		// check key
		if (strcmp(entry->key, key) == 0) {
			// match found, replace value
			push_to_the_end(&hashtable->entries[slot]->value, message);
			
			return;
		}
		// walk to next
		prev = entry;
		entry = prev->next;
	}
}

void ht_set_sub(ht_s_t* hashtable, const char* key, SOCKET socket) {
	unsigned int slot = hash(key);


	// try to look up an entry set
	EnterCriticalSection(&cs_map_sub);
	entry_s_t* entry = hashtable->entries[slot];
	LeaveCriticalSection(&cs_map_sub);

	// no entry means slot empty, insert immediately
	if (entry == NULL) {

		node_s_t* list;
		init_list_sub(&list);
		push_to_the_end_sub(&list, socket);

		hashtable->entries[slot] = ht_pair_sub(key, list);

		return;
	}

	entry_s_t* prev;

	// walk through each entry until either the end is
	// reached or a matching key is found
	while (entry != NULL) {
		// check key
		if (strcmp(entry->key, key) == 0) {
			// match found, replace value
			push_to_the_end_sub(&hashtable->entries[slot]->value, socket);
			
			return;
		}
		// walk to next
		prev = entry;
		entry = prev->next;
	}
}

void ht_set_socket(ht_socket_t* hashtable, SOCKET key, char* value) {
	
	unsigned int slot = key;


	// try to look up an entry set
	EnterCriticalSection(&cs_map_socket);
	entry_socket* entry = hashtable->entries[slot];
	LeaveCriticalSection(&cs_map_socket);

	// no entry means slot empty, insert immediately
	if (entry == NULL) {

		hashtable->entries[slot] = ht_pair_socket(key, value);

		return;
	}

	entry_socket* prev;

	// walk through each entry until either the end is
	// reached or a matching key is found
	while (entry != NULL) {
		// check key
		if (entry->key == key){
			
			strcpy(entry->value, value);
			
			return;
		}
		// walk to next
		prev = entry;
		entry = prev->next;
	}
}

node_t* ht_get(ht_t* hashtable, const char* key) {
	unsigned int slot = hash(key);

	// try to find a valid slot
	EnterCriticalSection(&cs_map_pub);
	entry_t* entry = hashtable->entries[slot];
	LeaveCriticalSection(&cs_map_pub);

	// no slot means no entry
	if (entry == NULL) {
		return NULL;
	}

	// walk through each entry in the slot, which could just be a single thing
	while (entry != NULL) {
		// return value if found
		if (strcmp(entry->key, key) == 0) {
			return entry->value;
		}

		// proceed to next key if available
		entry = entry->next;
	}

	// reaching here means there were >= 1 entries but no key match
	return NULL;
}

node_s_t* ht_get_sub(ht_s_t* hashtable, const char* key) {
	unsigned int slot = hash(key);

	// try to find a valid slot
	EnterCriticalSection(&cs_map_sub);
	entry_s_t* entry = hashtable->entries[slot];
	LeaveCriticalSection(&cs_map_sub);

	// no slot means no entry
	if (entry == NULL) {
		return NULL;
	}

	// walk through each entry in the slot, which could just be a single thing
	while (entry != NULL) {
		// return value if found
		if (strcmp(entry->key, key) == 0) {
			return entry->value;
		}

		// proceed to next key if available
		entry = entry->next;
	}

	// reaching here means there were >= 1 entries but no key match
	return NULL;
}

char* ht_get_topic(ht_socket_t* hashtable, SOCKET key){
	unsigned int slot = key;

	// try to find a valid slot
	EnterCriticalSection(&cs_map_socket);
	entry_socket* entry = hashtable->entries[slot];
	LeaveCriticalSection(&cs_map_socket);

	// no slot means no entry
	if (entry == NULL) {
		return NULL;
	}

	// walk through each entry in the slot, which could just be a single thing
	while (entry != NULL) {
		// return value if found
		if (entry->key == key){
			return entry->value;
		}

		// proceed to next key if available
		entry = entry->next;
	}

	// reaching here means there were >= 1 entries but no key match
	return NULL;
}

void delete_cs()
{
	DeleteCriticalSection(&cs_map_pub);
	DeleteCriticalSection(&cs_map_sub);
	DeleteCriticalSection(&cs_map_socket);
}

void ht_del_pub(ht_t* hashtable, const char* key) {
	unsigned int bucket = hash(key);

	// try to find a valid bucket
	entry_t* entry = hashtable->entries[bucket];

	// no bucket means no entry
	if (entry == NULL) {
		return;
	}

	entry_t* prev = entry;
	int idx = 0;

	// walk through each entry until either the end is reached or a matching key is found
	while (entry != NULL) {
		// check key
		if (strcmp(entry->key, key) == 0) {
			// first item and no next entry
			if (entry->next == NULL && idx == 0) {
				hashtable->entries[bucket] = NULL;
			}

			// first item with a next entry
			if (entry->next != NULL && idx == 0) {
				hashtable->entries[bucket] = entry->next;
			}

			// last item
			if (entry->next == NULL && idx != 0) {
				prev->next = NULL;
			}

			// middle item
			if (entry->next != NULL && idx != 0) {
				prev->next = entry->next;
			}

			// free the deleted entry
			free(entry->key);
			free(entry->value);
			free(entry);

			return;
		}

		// walk to next
		prev = entry;
		entry = prev->next;

		++idx;
	}
}


void ht_del_sub(ht_s_t* hashtable, const char* key) {
	unsigned int bucket = hash(key);

	// try to find a valid bucket
	entry_s_t* entry = hashtable->entries[bucket];

	// no bucket means no entry
	if (entry == NULL) {
		return;
	}

	entry_s_t* prev = entry;
	int idx = 0;

	// walk through each entry until either the end is reached or a matching key is found
	while (entry != NULL) {
		// check key
		if (strcmp(entry->key, key) == 0) {
			// first item and no next entry
			if (entry->next == NULL && idx == 0) {
				hashtable->entries[bucket] = NULL;
			}

			// first item with a next entry
			if (entry->next != NULL && idx == 0) {
				hashtable->entries[bucket] = entry->next;
			}

			// last item
			if (entry->next == NULL && idx != 0) {
				prev->next = NULL;
			}

			// middle item
			if (entry->next != NULL && idx != 0) {
				prev->next = entry->next;
			}

			// free the deleted entry
			free(entry->key);
			free(entry->value);
			free(entry);

			return;
		}

		// walk to next
		prev = entry;
		entry = prev->next;

		++idx;
	}
}

void ht_del_socket(ht_socket_t* hashtable, SOCKET key) {

	unsigned int bucket = key;

	// try to find a valid bucket
	entry_socket* entry = hashtable->entries[bucket];

	// no bucket means no entry
	if (entry == NULL) {
		return;
	}

	entry_socket* prev = entry;
	int idx = 0;

	// walk through each entry until either the end is reached or a matching key is found
	while (entry != NULL) {
		// check key
		if (entry->key == key) {
			// first item and no next entry
			if (entry->next == NULL && idx == 0) {
				hashtable->entries[bucket] = NULL;
			}

			// first item with a next entry
			if (entry->next != NULL && idx == 0) {
				hashtable->entries[bucket] = entry->next;
			}

			// last item
			if (entry->next == NULL && idx != 0) {
				prev->next = NULL;
			}

			// middle item
			if (entry->next != NULL && idx != 0) {
				prev->next = entry->next;
			}

			// free the deleted entry
			//free(&entry->key);
			//free(entry->value);
			free(entry);

			return;
		}

		// walk to next
		prev = entry;
		entry = prev->next;

		++idx;
	}
}

void ht_delete_map_pub(ht_t* hashtable) {

	for (int i = 0; i < TABLE_SIZE; ++i) {

		entry_t* entry = hashtable->entries[i];

		if (entry == NULL) {
			continue;
		}
				
		if (entry != NULL) {

			ht_del_pub(hashtable, entry->key);

			break;
		}
	}
}

void ht_delete_map_sub(ht_s_t* hashtable) {

	for (int i = 0; i < TABLE_SIZE; ++i) {

		entry_s_t* entry = hashtable->entries[i];

		if (entry == NULL) {
			continue;
		}

		if (entry != NULL) {

			ht_del_sub(hashtable, entry->key);

			break;
		}
	}
}

void ht_delete_map_socket(ht_socket_t* hashtable) {

	for (int i = 0; i < TABLE_SIZE; ++i) {

		entry_socket* entry = hashtable->entries[i];

		if (entry == NULL) {
			continue;
		}

		if (entry != NULL) {

			ht_del_socket(hashtable, entry->key);

			break;
		}
	}
}