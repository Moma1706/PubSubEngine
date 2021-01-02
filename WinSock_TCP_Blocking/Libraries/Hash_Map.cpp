#include"Hash_Map.h"
#include "list.h"

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

	// copy the key and value in place
	strcpy(entry->key, key);
	entry->value = value;

	// next starts out null but may be set later on
	entry->next = NULL;

	return entry;
}

entry_s_t* ht_pair_sub(const char* key, node_s_t* value) {
	// allocate the entry
	entry_s_t* entry = (entry_s_t*)malloc(sizeof(entry_s_t) * 1);
	entry->key = (char*)malloc(strlen(key) + 1);
	entry->value = (node_s_t*)malloc(sizeof(node_s_t) + 1);

	// copy the key and value in place
	strcpy(entry->key, key);
	entry->value = value;

	// next starts out null but may be set later on
	entry->next = NULL;

	return entry;
}

ht_t* ht_create(void) {
	// allocate table
	ht_t* hashtable = (ht_t*)malloc(sizeof(ht_t) * 1);

	// allocate table entries
	hashtable->entries = (entry_t**)malloc(sizeof(entry_t*) * TABLE_SIZE);

	// set each to null (needed for proper operation)
	int i = 0;
	for (; i < TABLE_SIZE; ++i) {
		hashtable->entries[i] = NULL;
	}

	return hashtable;
}

ht_s_t* ht_create_sub(void) {
	// allocate table
	ht_s_t* hashtable = (ht_s_t*)malloc(sizeof(ht_s_t) * 1);

	// allocate table entries
	hashtable->entries = (entry_s_t**)malloc(sizeof(entry_s_t*) * TABLE_SIZE);

	// set each to null (needed for proper operation)
	int i = 0;
	for (; i < TABLE_SIZE; ++i) {
		hashtable->entries[i] = NULL;
	}

	return hashtable;
}

void ht_set_pub(ht_t* hashtable, const char* key, char* message) {
	unsigned int slot = hash(key);

	// try to look up an entry set
	entry_t* entry = hashtable->entries[slot];
	
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
			//iterate_list(hashtable->entries[slot]->value);
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
	entry_s_t* entry = hashtable->entries[slot];

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
			//iterate_list(hashtable->entries[slot]->value);
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
	entry_t* entry = hashtable->entries[slot];

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
	entry_s_t* entry = hashtable->entries[slot];

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
