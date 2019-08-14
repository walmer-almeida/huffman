#include "Tree.h"

bool comparing(void *itemA,unsigned char itemB){
    return (*((unsigned char*)itemA)== itemB);
}
//start of tree functions

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