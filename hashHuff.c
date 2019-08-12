#include <stdio.h>
#include <stdlib.h>
#define TAM 256

typedef struct element element;
struct element{
	void *byte;//(key)
	long long int frequency;//(value)
};
typedef struct hash_table hash_table;
struct hash_table{
	element *table[TAM];
};
/*
cria a hash, com cada character da tabela ascii como "byte" e coloca a "frequency"= 0 para todos os characteres
*/
hash_table *create_hash_table(){ 
	hash_table *new_hash= (hash_table*)malloc(sizeof(hash_table));
	element *new_element;
	int i;
	for(i=0;i<TAM;i++){
		new_element= (element*)malloc(sizeof(element));
		new_hash->table[i]= new_element;
		new_hash->table[i]->byte= (unsigned char)i;
		new_hash->table[i]->frequency= 0;
	}
	return new_hash;
}
/*
A partir do "byte" recebido, incrementar a "frequency" para cada "byte" correspondente
*/
void put(hash_table *ht, void *byte){
	int h = (int)byte % TAM;
	ht->table[h]->frequency++;
}
/*
Imprime toda a hash_table
*/
void print_hash_table(hash_table *ht){
	int i;
	element *aux;
	for(i=0; i<TAM; i++){
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
