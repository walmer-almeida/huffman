#ifndef _STRUCTURES_H_
#define _STRUCTURES_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "AuxFunctions.h"

/*SART OF HASH FUNCTIONS*/
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
/*END OF HASH FUNCTIONS*/
/*START OF TREE FUNCTIONS*/
#define SLASH 92
typedef struct _HTREE huffTree;
struct _HTREE
{
    void *byte;
    long long int frequency;
    huffTree *left;
    huffTree *right;
};

/* 
 * Recieves a tree node;
 * Check if the node is a leaf;
 * Return true if tree node is a leaf, false if not.
 */
bool isLeaf(huffTree *root){
    return (root->left==NULL && root->right==NULL);
}

/*
 * Recieves a byte and its frequency;
 * Creates a tree node;
 * Return the created node.
*/
huffTree *createNode(void *byte,long long int frequency){
    huffTree *newNode = (huffTree*) malloc(sizeof(huffTree));
    newNode->byte = byte;
    newNode->frequency = frequency;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}
huffTree *create_tree(void *byte, long long int frequency, huffTree *L, huffTree *R){
	huffTree *newNode = (huffTree*) malloc(sizeof(huffTree));
	newNode->byte = byte;
    newNode->frequency = frequency;
    newNode->left = L;
    newNode->right = R;
    return newNode;
}
/*
 * Recieves the tree root;
 * Checks the tree size by running it recursively.
 * Returns the tree size at the end.
*/
int treeSize(huffTree*root){
    if (isLeaf(root)){
        if(comparing(root->byte,'*')||comparing(root->byte,(char)SLASH)){
            return 2;//the size here its 2 because the printed node will use 2 chars("/*")
        }
        else return 1;
    }
    else{
        return 1 + treeSize(root->left) + treeSize(root->right);   
    }
}
/*
 * Recieves the tree root and the compacted file pointer;
 * Prints the huffman tree (in pre order) at the compacted file right after the header;
 * Returns nothing.
*/
void printTreeInFile(huffTree *root, FILE *compactFile){
    if(root!=NULL){
        if (isLeaf(root) && (comparing(root->byte,'*')||comparing(root->byte,(char)SLASH)))
        {
            fprintf(compactFile,(char)SLASH);
        }
        fprintf(compactFile,"%c",(unsigned char)root->byte);
        printTreeInFile(root->left,compactFile);
        printTreeInFile(root->right,compactFile);
    }
}

void printTree(huffTree *root){
	printf(" (");
    if(root!=NULL){
        if (isLeaf(root) && (comparing(root->byte,'*')||comparing(root->byte,(char)SLASH)))
        {
			printf("%c",(char)SLASH);
        }
        printf("%c",(unsigned char)root->byte);
        printTree(root->left);
        printTree(root->right);
    }
	printf(") ");
}
/*END OF TREE FUNCTIONS*/
/*START OF HEAP FUNCTIONS*/
#define MAX_HEAP_SIZE 257
typedef struct _heap heap;
struct _heap{
	int size;
	huffTree *data[MAX_HEAP_SIZE];
};
heap *create_heap(){
    int i;
	heap *new_heap= (heap*)malloc(sizeof(heap));
    for(i=0; i < MAX_HEAP_SIZE; i++){
        new_heap->data[i]= NULL;
    }
	new_heap->size= 0;
	return new_heap;
}
int is_empty(heap *h){
	return (h->size == 0);
}
int get_parent_index(heap *h, int i){
	return i/2;
}
int get_left_index(heap *h, int i){
	return 2*i;
}
int get_right_index(heap *h, int i){
	return 2*i + 1;
}
void swap(huffTree **a, huffTree **b){
	huffTree *aux;
	aux= *a;
    *a= *b;
    *b= aux;
}
void min_heapify(heap *h, int i){
	int smallest;
	int left_index = get_left_index(h, i);
	int right_index = get_right_index(h, i);
	if(left_index <= h->size && h->data[left_index]->frequency < h->data[i]->frequency){
		smallest = left_index;
	}else{
		smallest = i;
	}
	if(right_index <= h->size && h->data[right_index]->frequency < h->data[smallest]->frequency){
		smallest = right_index;
	}
	if(h->data[i]->byte != h->data[smallest]->byte){
		swap(&h->data[i], &h->data[smallest]);
		min_heapify(h, smallest);
	}
}
void enqueue(heap *h, huffTree *item){//adiciona o elemento

	if(h->size >= MAX_HEAP_SIZE){
		printf("Heap overflow");
	}else{
		h->data[++h->size] = item;
		int key_index = h->size;
		int parent_index = get_parent_index(h, h->size);
		while(parent_index >= 1 && h->data[key_index]->frequency < h->data[parent_index]->frequency){

			swap(&h->data[key_index], &h->data[parent_index]);
			key_index = parent_index;
			parent_index = get_parent_index(h, key_index);
		}
	}
}
void *dequeue(heap *h){//tira o elemento da frente(no caso da max_heapify, o maior elemento)
	if(is_empty(h)){
		printf("Heap underflow");
		return NULL;
	}else{
		huffTree *item = h->data[1];
		h->data[1] = h->data[h->size];
		h->size--;
		min_heapify(h, 1);
		return item;
	}
}


huffTree *create_Tree_from_heap(heap *h){
	huffTree *L, *R, *aux;
	unsigned char byte_aux= '*';
	while(h->size > 1){
		L= dequeue(h);
		R= dequeue(h);
		aux= create_tree(&byte_aux, L->frequency + R->frequency, L, R);
		enqueue(h, aux);
	}
	return dequeue(h);
}
/*END OF HEAP FUNCTIONS*/
#endif