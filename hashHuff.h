#ifndef _HASHHUFF_H_
#define _HASHHUFF_H_
#include <stdio.h>
#include <stdlib.h>
#define HASHSIZE 256
typedef struct element element;
struct element{
	void *byte;//(key)
	long long int frequency;//(value)
};
typedef struct hash_table hash_table;
struct hash_table{
	element *table[HASHSIZE];
};

hash_table *create_hash_table();
void put(hash_table *ht, void *byte);
void print_hash_table(hash_table *ht);
#endif