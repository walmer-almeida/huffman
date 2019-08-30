#include "Compress.h"
#include "Decompress.h"

int main(){
    char fileName[1000];
    int type= 0;
    printf("Do you want to compress(1), decompress(2) or Close(3)?\n");
    scanf("%d", &type);
    getchar();
    FILE *originalFile;
    if(type == 1){
        printf("Enter file name\n");
        scanf("%[^\n]s", fileName);
        originalFile= fopen(fileName, "rb");
        Compress(originalFile, fileName);
    }else if(type == 2){
        printf("Enter file name\n");
        scanf("%[^\n]s", fileName);
        originalFile= fopen(fileName, "rb");
        decompress(originalFile, fileName);
    }
    else{
        printf("Closing.. Bye!\n");
    }
    return 0;
}