#ifndef _COMPRESS_H_
#define _COMPRESS_H_
#include "Structures.h"

/*
 * Recieves a Huffman tree,a counter,a compHash and a string;
 * Runs the tree recursively,generating the compressed bytes' paths and putting them in the compHash;
 * Returns nothing.
*/
void generateCompressedBytes(huffTree *root,int count, compHash *cht, char path[]){
    if (isLeaf(root)){
        putComp(cht,root->byte,path,count);
    }
    else{
        path[count] = '0';
        generateCompressedBytes(root->left, count + 1, cht, path);
        path[count] = '1';
        generateCompressedBytes(root->right, count + 1, cht, path);
    }
}

/*
 * Recieves a compHash and a normal Hash;
 * Sum all the compressed bytes*their frequency(stored at the normal hash);
 * Returns 8- the rest of the sum divided by 8, wich equals to the trash size.
*/
int trasHSize(compHash *cht,hashTable *ht){
    
    int i,sum=0;
    for (i = 0; i < HASHSIZE; i++)
    {
        if (ht->table[i]!=NULL)
        {
            sum+= (cht->table[i]->numberOfBits)*(ht->table[i]->frequency);
        }
    }
    return 8 - (sum % 8);
}

/*
 * Recieves a file pointer and its string;
 * Creates a compressed file;
 * Returns nothing.
*/
void Compress(FILE *originalFile, char string_file[]){
    unsigned char currentByte, compactedByte=0;
    char treePath[100];
    short trash,tree,header;
    int count=0;
    heap *h=NULL;
    hashTable *ht= createHashTable();
    huffTree *bt=NULL;
    compHash *cht = createCompHash();
    FILE *compactedFile;
    originalFile = fopen(string_file,"rb");
    
    printf("Reading File..\n");
    while(!feof(originalFile)){        
        currentByte= fgetc(originalFile);
        if(!feof(originalFile)){
            put(ht, &currentByte);    
        }
    }
    rewind(originalFile);

    printf("Creating heap..\n");
    h= insertHeapFromHash(ht);
    
    printf("Creating Huffman tree..\n");
    bt= createTreeFromHeap(h);
    generateCompressedBytes(bt,0,cht,treePath);

    printf("Starting compression!\n");
    printf("Creating compacted file..\n");
    string_file = strtok(string_file,".");
    string_file = strcat(string_file,".huff");
    compactedFile = fopen(string_file,"wb");
    
    printf("Writing header..\n");
    trash = trasHSize(cht,ht);
    tree = treeSize(bt);
    trash = trash << 13;
    header = trash | tree;

    //First byte
    currentByte = header>>8;
    fputc(currentByte,compactedFile);
    //Second byte
    currentByte = header;
    fputc(currentByte,compactedFile);
    printTreeInFile(bt,compactedFile);
    fseek(compactedFile,2+tree,SEEK_SET);
    printf("Writing compressed bytes..\n");
    while (!feof(originalFile))
    {
        currentByte= fgetc(originalFile);
        if(!feof(originalFile)) {
            for (int i = 0; i < cht->table[(int)currentByte]->numberOfBits; i++)
            {
                if (cht->table[(int)currentByte]->bitsString[i] == '1') {
                    compactedByte = setBit(compactedByte,7-count);
                }
                count++;
                if (count==8) {
                    fprintf(compactedFile,"%c",compactedByte);
                    compactedByte = 0;
                    count = 0;
                }        
            }
        }
    }
    
    if (trash!=0)fputc(currentByte,compactedFile);
    
   
    free(bt);
    free(h);
    free(ht);
    free(cht);
    fclose(originalFile);
    fclose(compactedFile);
}

#endif