#include "hashHuff.h"

/*
 * Recieves nothing;
 * Creates a hash with all ascii characters and frequency=0;
 * Returns the created hash.
*/
hash_table *create_hash_table(){ 
	hash_table *new_hash= (hash_table*)malloc(sizeof(hash_table));
	element *new_element;
	int i;
	for(i=0;i<HASHSIZE;i++){
		new_element= (element*)malloc(sizeof(element));
		new_hash->table[i]= new_element;
		new_hash->table[i]->byte= (unsigned char)i;
		new_hash->table[i]->frequency= 0;
	}
	return new_hash;
}
/*
 * Recieves a hash table and a byte;
 * Increases the byte frequency at hash table's byte position;
 * Returns nothing.
 */
void put(hash_table *ht, void *byte){
	int h = (int)byte % HASHSIZE;
	ht->table[h]->frequency++;
}
/*
 * Recieves a hash table;
 * Prints the whole hash table;
 * Returns nothing.
*/
void print_hash_table(hash_table *ht){
	int i;
	element *aux;
	for(i=0; i<HASHSIZE; i++){
		aux= ht->table[i];
		printf("posicao: %d ", i);
		printf("-> byte: %c frequency: %lld\t", (char)aux->byte, aux->frequency);
		printf("\n");
	}
}

int main(){
	hash_table *hash_table= create_hash_table();
	unsigned char byte;
	while(scanf("%c", &byte) != EOF){
		put(hash_table, byte);
	}
	print_hash_table(hash_table);
return 0;
}
