#include "Structures.h"
void Compress(FILE *file, char string_file[]){
    printf("Enter file name\n");

    if(file == NULL){
        printf("File not found\n");
        return 0;
    }printf("a");
    unsigned char c;
    hash_table *ht= create_hash_table();
    while(!feof(file)){
        c= fgetc(file);
        put(ht, c);
    }
    heap *h;
    h= insert_heap_from_hash(ht);
    huffTree *bt;
    bt= create_Tree_from_heap(h);
    free(h);
    printTree(bt);
    fclose(file);
}
int main(){
    char string_file[100];
    int type= 0;
    FILE *file;
    printf("Do you want to compress(1) or decompress(2)?\n");
    scanf("%d", type);
    getchar();
    if(type == 1){
        printf("Enter file name\n");
        scanf("%[^\n]s", string_file);
        file= fopen(string_file, "rb");
        Compress(file, string_file);
    }else if(type == 2){
        printf("a");
    }else{
        printf("b");
    }
    return 0;
}