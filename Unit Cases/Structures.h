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

typedef struct hashTable hashTable;
struct hashTable{
	element *table[HASHSIZE];
};

/*
 * Recieves nothing;
 * Creates a hash with all ascii characters and frequency=0;
 * Returns the created hash.
*/
hashTable *createHashTable(){ 
	hashTable *newHash= (hashTable*)malloc(sizeof(hashTable));
	int i;
	for(i=0;i<HASHSIZE;i++){
		newHash->table[i]=NULL;
	}
	return newHash;
}

element *createElement(void *item,size_t dataSize){
	element *newElement = (element*)malloc(sizeof(element));
	newElement->byte = malloc(dataSize);
	*(unsigned char*)(newElement->byte) = *(unsigned char*) item;
	newElement->frequency=1;
	return newElement;
}
/*
 * Recieves a hash table and a byte;
 * Increases the byte frequency at hash table's byte position;
 * Returns nothing.
 */
void put(hashTable *ht, void *byte){
	int key; 
	key = (*(unsigned char *)byte % HASHSIZE);
	if (ht->table[key]!=NULL)
	{
		ht->table[key]->frequency++;
	}
	else{
		ht->table[key] = createElement(byte,sizeof(unsigned char));
	}
	

}
/*
 * Recieves a hash table;
 * Prints the whole hash table;
 * Returns nothing.
*/
void printHashTable(hashTable *ht){
	int i;
	for(i=0; i<HASHSIZE; i++){
		if(ht->table[i] != NULL){
			printf("posicao: %d ", i);
			printf("-> byte: %c frequency: %lld\t", *(char*)ht->table[i]->byte, ht->table[i]->frequency);
			printf("\n");
		}
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
    newNode->byte = malloc(sizeof(unsigned char));
	*(unsigned char * )newNode -> byte = *(unsigned char *) byte;
    newNode->frequency = frequency;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

huffTree *createTree(void *byte, long long int frequency, huffTree *L, huffTree *R){
	huffTree *newNode = (huffTree*) malloc(sizeof(huffTree));
	newNode->byte = malloc(sizeof(unsigned char));
	*(unsigned char * )newNode -> byte = *(unsigned char *) byte;
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
    if(root != NULL){
        if (isLeaf(root) && (comparing(root->byte,'*')||comparing(root->byte,(char)SLASH)))
        {
            fputc((char)SLASH, compactFile);
        }
        fprintf(compactFile,"%c", *(unsigned char*)root->byte);
        printTreeInFile(root->left,compactFile);
        printTreeInFile(root->right,compactFile);
    }
}


void printTree(huffTree *root){
	printf(" (");
    if(root != NULL){
        printf("%c", *(char*)root->byte);
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
heap *createHeap(){
    int i;
	heap *new_heap= (heap*)malloc(sizeof(heap));
    for(i=0; i < MAX_HEAP_SIZE; i++){
        new_heap->data[i]= NULL;
    }
	new_heap->size= 0;
	return new_heap;
}
int isEmpty(heap *h){
	return (h->size == 0);
}

int getParentIndex(heap *h, int i){
	return i/2;
}
int getLeftIndex(heap *h, int i){
	return 2*i;
}
int getRightIndex(heap *h, int i){
	return 2*i + 1;
}
void swap(huffTree **a, huffTree **b){
	huffTree *aux;
	aux= *a;
    *a= *b;
    *b= aux;
}
void minHeapify(heap *h, int i){
	int smallest;
	int left_index = getLeftIndex(h, i);
	int right_index = getRightIndex(h, i);
	if(left_index <= h->size && h->data[left_index]->frequency <= h->data[i]->frequency){
		smallest = left_index;
	}else{
		smallest = i;
	}
	if(right_index <= h->size && h->data[right_index]->frequency < h->data[smallest]->frequency){
		smallest = right_index;
	}
	if(h->data[i]->byte != h->data[smallest]->byte){
		swap(&h->data[i], &h->data[smallest]);
		minHeapify(h, smallest);
	}
}
void enqueue(heap *h, huffTree *item){//adiciona o elemento

	if(h->size >= MAX_HEAP_SIZE){
		printf("Heap overflow");
	}else{
		h->data[++h->size] = item;
		int key_index = h->size;
		int parent_index = getParentIndex(h, h->size);
		while(parent_index >= 1 && h->data[key_index]->frequency <= h->data[parent_index]->frequency){

			swap(&h->data[key_index], &h->data[parent_index]);
			key_index = parent_index;
			parent_index = getParentIndex(h, key_index);
		}
	}
}
void *dequeue(heap *h){//tira o elemento da frente(no caso da max_heapify, o maior elemento)
	if(isEmpty(h)){
		//printf("Heap underflow");
		return NULL;
	}else{
		huffTree *item = h->data[1];
		h->data[1] = h->data[h->size];
		h->size--;
		minHeapify(h, 1);
		return item;
	}
}

heap *insertHeapFromHash(hashTable *ht){
    heap *h= createHeap();
    huffTree *node;
    int i;
    for(i=0; i<HASHSIZE; i++){
        if(ht->table[i] != NULL){
            node= createNode(ht->table[i]->byte, ht->table[i]->frequency);
            enqueue(h, node);
        }
    }
    return h;
}

huffTree *createTreeFromHeap(heap *h){
	huffTree *L, *R, *aux;
	unsigned char byte_aux= '*';
	while(h->size > 1){
		L= dequeue(h);
		R= dequeue(h);
		aux= createTree(&byte_aux, L->frequency + R->frequency, L, R);
		enqueue(h, aux);
	}
	return dequeue(h);
}
/*END OF HEAP FUNCTIONS*/
/*START OF COMPRESSED BYTES' HASH FUNCTIONS*/
typedef struct compElement compElement;
typedef struct compHash compHash;
struct compElement{
	char bitsString[50];
	int numberOfBits;
}; 
struct compHash
{
	compElement *table[HASHSIZE];
};

/*
 * Recieves nothing;
 * Creates a compHash with all their tables NULL compressed bytes and their bit count;
 * Returns the created hash.
*/
compHash *createCompHash(){
	int i;
	compHash *newCompHash = (compHash*)malloc(sizeof(compHash));
	for (i = 0; i < HASHSIZE; i++)
	{
		newCompHash->table[i] = NULL;
	}
	return newCompHash;
}

/*
 * Recieves a compHash, a byte, a string and the string size;
 * Put your compressed byte and the amount of bits given a normal byte;
 * Returns nothing.
*/
void putComp(compHash *cht,void *item, char bits[],int strSize){
	int elementPos = *(unsigned char*) item;
	int i;
	compElement *newCompElement = (compElement*) malloc(sizeof(compElement));
	newCompElement->numberOfBits = strSize;
	for ( i = 0; i < strSize; i++)
	{
		newCompElement->bitsString[i] = bits[i];
	}
	cht->table[elementPos]= newCompElement;
}

/*
 * Recieves a compHash;
 * Print all tables that are not null;
 * Returns nothing.
*/
void printHash(compHash *ht)
{
	for (int i = 0; i < 256; i++)
	{
		if (ht->table[i] != NULL)
		{
			printf("%d ->", i);
			for (int j = 0; j < ht->table[i]->numberOfBits; j++)
			{
				printf("%c", ht->table[i]->bitsString[j]);
			}
			printf(" [%d]\n", ht->table[i]->numberOfBits);
		}
	}
}
/*END OF COMPRESSED BYTES' HASH FUNCTIONS*/
#endif