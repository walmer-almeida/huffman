#include "Structures.h"
void insert_heap(hash_table *ht){
    heap *h= create_heap();
    int i;
    for(i=0; i<HASHSIZE; i++){
        if(ht->table[i]->frequency != 0){
            enqueue(h, ht->table[i]);
        }else{
            free(ht->table[i]);
        }
    }
}
int main(){
    FILE *file;
    file= fopen("teste.txt", "rb");
    if(file == NULL){
        printf("Arquivo não encontrado\n");
        return 0;
    }
    unsigned char c;
    hash_table *ht= create_hash_table();
    while(!feof(file)){
        c= fgetc(file);
        put(ht, c);
    }
    insert_heap(ht);
    //print_hash_table(ht);

    fclose(file);
    return 0;
}