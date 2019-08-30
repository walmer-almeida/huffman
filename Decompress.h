#ifndef _DECOMPRESS_H_
#define _DECOMPRESS_H_
#include "Structures.h"

/*
 * Recieves a unsigned char byte;
 * Creates a hufftree node. Used in mountHTFromFile function;
 * Returns the created node.
*/
huffTree *createCompNode(unsigned char character)
{
	unsigned char *aux = (unsigned char *)malloc(sizeof(unsigned char));
    *aux = character;
	huffTree *new = (huffTree *)malloc(sizeof(huffTree));
    new->frequency = 0;
    new->byte = aux;
    new->right = NULL;
    new->left = NULL;
    return new;
}

/*
 * Recieves a file pointer;
 * Creates the Huffmantree by running the file recursively;
 * Returns the Huffman tree.
*/
huffTree *mountHTFromFile(FILE *compactedFile){
    unsigned char currentByte;
    huffTree *aux;
    currentByte = fgetc(compactedFile);
    if (currentByte=='*'){
        aux = createCompNode(currentByte);
        aux->left= mountHTFromFile(compactedFile);
        aux->right= mountHTFromFile(compactedFile);
    }
    else if (currentByte==(char)SLASH){
        currentByte = fgetc(compactedFile);
        aux = createCompNode(currentByte);
    }
    else{
        aux = createCompNode(currentByte);
    }
    return aux;
}

/*
 * Recieves 2 file pointers(for the compressed and decompressed file),the huffman tree root 
 * and its size,the compacted file's trash size and the compacted file size;
 * Reads the compressed bytes and use their bits to run the huffman tree, 
 * printing the normal byte (now in the decompressed file) when it reaches at a leaf ;
 * Returns nothing.
*/
void printBytes(FILE *compactedFile, FILE *decompressedFile, huffTree *root, short compTreeSize, short compTrashSize, int fileSize){
    long int byteNumber,currBit,numOfBits;
    unsigned char currentByte;
    huffTree *aux=root;
    for (byteNumber = 0; byteNumber < fileSize; byteNumber++)
    {
        currentByte = fgetc(compactedFile);
        if (byteNumber==fileSize-1)  numOfBits = 8-compTrashSize;
        else numOfBits = 8;
        for (currBit = 0; currBit < numOfBits; currBit++)
        {
            if (isBitISet(currentByte,7-currBit)) aux = aux->right;
            else aux = aux -> left;
            if (isLeaf(aux))
            {
                fprintf(decompressedFile,"%c",*(unsigned char*)aux->byte);
                aux=root;
            }
        }
    }
}

/*
 * Recieves a file pointer and its name string;
 * Creates the decompressed file, by reading the header and using the functions above;
 * Returns 0.
*/
int decompress(FILE *compactedFile, char string_file[]){
    int fileSize=0;
    short compTrashSize, compTreeSize = 8191; //8191 = '0001111111111111'
    char decompressed_file_string[100];
    unsigned char firstByte, secondByte;
    huffTree *bt = NULL;
    FILE *decompressedFile;
    getchar();
    printf("Enter the output file name:\n");
    scanf("%[^\n]s", decompressed_file_string);
    compactedFile = fopen(string_file,"rb");
    decompressedFile = fopen(decompressed_file_string,"wb");
    fseek(compactedFile,0,SEEK_END);
    fileSize = ftell(compactedFile);
    rewind(compactedFile);
    printf("Compressed file size: %d bytes\n",fileSize);
    printf("Reading Header...\n");
    firstByte = fgetc(compactedFile);
    secondByte = fgetc(compactedFile);
    compTrashSize = firstByte>>5;
    compTreeSize = (firstByte<<8 | secondByte) & compTreeSize;
    bt = mountHTFromFile(compactedFile);
    fseek(compactedFile,compTreeSize+2,SEEK_SET);
    printBytes(compactedFile,decompressedFile,bt,compTreeSize,compTrashSize,fileSize);
    fclose(compactedFile);
    fclose(decompressedFile);
    return 0;
}
#endif