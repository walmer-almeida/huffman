#ifndef TREE_H_
#define TREE_H_
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#define SLASH 92
typedef struct _HTREE huffTree;
struct _HTREE
{
    void *byte;
    long long int frequency;
    huffTree *left;
    huffTree *right;
};

bool comparing(void *itemA,unsigned char itemB);
bool isLeaf(huffTree *root);
huffTree *createNode(void *byte,long long int frequency);
int treeSize(huffTree*root);
void printTreeInFile(huffTree *root, FILE *compactFile);
#endif