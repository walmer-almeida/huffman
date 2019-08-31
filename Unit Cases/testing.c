#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include "Structures.h"
#include "AuxFunctions.h"
#include "Compress.h"
#include "Decompress.h"



void putEntryHash(hashTable *ht){
    FILE *input = NULL;
    char string_file[50]="teste.txt"; 
    input = fopen(string_file,"rb");
    unsigned char currentByte;
    while(!feof(input)){
        currentByte = fgetc(input);
        if (!feof(input))
        {
            put(ht,&currentByte);
        }
    }
    rewind(input);
    fclose(input);
}

//START HASH FUNCTS

void testHashCreation(){   
	hashTable * ht = createHashTable();
	int i;
    for (i=0;i<HASHSIZE;i++){
		CU_ASSERT_PTR_NULL(ht->table[i]);
	}
    free(ht);
}

void testElementCreation(){
    unsigned char testcharacter = '*';
    element *aux = createElement(&testcharacter,sizeof(unsigned char));
    CU_ASSERT_EQUAL(*(unsigned char*)aux->byte,testcharacter);
    CU_ASSERT_EQUAL(aux->frequency,1);
    free(aux);
}

void testHashInsertion(){
    unsigned char currentByte;
    hashTable *ht = createHashTable();
    putEntryHash(ht);
    // Input = AAAAAABBBBBCCCCDDDEEF
    int a='A',b='B',c='C',d='D',e='E',f='F';
    CU_ASSERT_EQUAL(ht->table[a]->frequency, 6);
    CU_ASSERT_EQUAL(ht->table[b]->frequency, 5);
    CU_ASSERT_EQUAL(ht->table[c]->frequency, 4);
    CU_ASSERT_EQUAL(ht->table[d]->frequency, 3);
    CU_ASSERT_EQUAL(ht->table[e]->frequency, 2);
    CU_ASSERT_EQUAL(ht->table[f]->frequency, 1);
    free(ht);
}

//START HEAP FUNCTS
void testHeapCreation(){
    heap *h = createHeap();
    for (int i = 0; i < MAX_HEAP_SIZE; i++)
    {
        CU_ASSERT_PTR_NULL(h->data[i]);
    }
    free(h);
}

void testHeapInsertion(){
    hashTable *ht=createHashTable();
    putEntryHash(ht);
    heap *h = NULL;
    h = insertHeapFromHash(ht);
    int i=1;
    CU_ASSERT_EQUAL(h->size,6);
    CU_ASSERT_EQUAL(h->data[i]->frequency,1);
    // CU_ASSERT_EQUAL(h->data[getLeftIndex(h,i)],2);
    // CU_ASSERT_EQUAL(h->data[getRightIndex(h,i)],3);
    free(h);
}

void testHeapDequeue(){
    hashTable *ht=createHashTable();
    putEntryHash(ht);
    heap *h = NULL;
    h = insertHeapFromHash(ht);
    huffTree *aux = dequeue(h);
    CU_ASSERT_EQUAL(*(unsigned char*)aux->byte,'F');
    aux = dequeue(h);
    CU_ASSERT_EQUAL(*(unsigned char*)aux->byte,'E');
    aux = dequeue(h);
    CU_ASSERT_EQUAL(*(unsigned char*)aux->byte,'D');
    aux = dequeue(h);
    CU_ASSERT_EQUAL(*(unsigned char*)aux->byte,'C');
    aux = dequeue(h);
    CU_ASSERT_EQUAL(*(unsigned char*)aux->byte,'B');
    aux = dequeue(h);
    CU_ASSERT_EQUAL(*(unsigned char*)aux->byte,'A');
    aux = dequeue(h);
    CU_ASSERT_PTR_NULL(aux);
    free(aux);
}

//START TREE FUNCTS
huffTree *createTestTree()
{
    unsigned char a='A',b='B',ast='*';
    long long int freq=0;
	huffTree *root = createNode(&ast,freq);
	root->left = createNode(&a,freq);
	root->right = createNode(&b,freq);

	return root;
}
void testTreeExample()
{
	huffTree *root = createTestTree();
	CU_ASSERT_EQUAL(*((unsigned char*)root->byte), '*');
	CU_ASSERT_EQUAL(*((unsigned char*)root->left->byte), 'A');
	CU_ASSERT_EQUAL(*((unsigned char*)root->right->byte), 'B');
	CU_ASSERT_EQUAL(treeSize(root), 3);
}

int runTestTree(huffTree *root){
    huffTree *aux = root;
    if (!isLeaf(aux))
    {
        CU_ASSERT_EQUAL( *(unsigned char*)aux->byte,'*');
        return 0;
    }
    runTestTree(aux->left);
    runTestTree(aux->right);
}
void testTreeInsertion(){
    hashTable *ht=createHashTable();
    heap *h = NULL;
    huffTree *bt=NULL;
    putEntryHash(ht);
    h = insertHeapFromHash(ht);
    bt = createTreeFromHeap(h);
    CU_ASSERT_EQUAL(treeSize(bt),11);
    runTestTree(bt);
    CU_ASSERT_EQUAL(*(unsigned char*)bt->left->left->byte,'C');
    CU_ASSERT_EQUAL(*(unsigned char*)bt->left->right->byte,'B');
    CU_ASSERT_EQUAL(*(unsigned char*)bt->right->left->left->left->byte,'F');
    CU_ASSERT_EQUAL(*(unsigned char*)bt->right->left->left->right->byte,'E');
    CU_ASSERT_EQUAL(*(unsigned char*)bt->right->left->right->byte,'D');
    CU_ASSERT_EQUAL(*(unsigned char*)bt->right->right->byte,'A');
}

//COMPHASH TEST

void testCompHashcreation(){
    compHash *cht = createCompHash();
	for (int i=0;i<HASHSIZE;i++){
		CU_ASSERT_PTR_NULL(cht->table[i]);
	}
}

void testCompHashElements(){

    hashTable *ht=createHashTable();
    putEntryHash(ht);
    heap *h=insertHeapFromHash(ht);
    huffTree *bt=createTreeFromHeap(h);
    compHash *cht=createCompHash();
    char testString[50];
    generateCompressedBytes(bt,0,cht,testString);
    int a='A',b='B',c='C',d='D',e='E',f='F';
    CU_ASSERT_STRING_EQUAL(cht->table[a]->bitsString,"11");
    CU_ASSERT_STRING_EQUAL(cht->table[b]->bitsString,"01");
    CU_ASSERT_STRING_EQUAL(cht->table[c]->bitsString,"00");
    CU_ASSERT_STRING_EQUAL(cht->table[d]->bitsString,"101");
    CU_ASSERT_STRING_EQUAL(cht->table[e]->bitsString,"1001");
    CU_ASSERT_STRING_EQUAL(cht->table[f]->bitsString,"1000");
}

int main(){
    if (CUE_SUCCESS != CU_initialize_registry()) return CU_get_error();

    CU_pSuite hashSuite = NULL;
    hashSuite = CU_add_suite("test_Hash",0,0);
    if (NULL==CU_add_test(hashSuite,"test_Hash_Creation",testHashCreation)){
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL==CU_add_test(hashSuite,"test_Element_Creation",testElementCreation)){
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL==CU_add_test(hashSuite,"test_Hash_Insertion",testHashInsertion)){
        CU_cleanup_registry();
        return CU_get_error();
    }
    //END OF FIRST SUITE
    CU_pSuite heapSuite = NULL;
    heapSuite = CU_add_suite("test_Heap",0,0);
    if (NULL==CU_add_test(heapSuite,"test_Heap_Creation",testHeapCreation)){
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL==CU_add_test(heapSuite,"test_Heap_Insertion",testHeapInsertion)){
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL==CU_add_test(heapSuite,"test_Heap_Dequeue",testHeapDequeue)){
        CU_cleanup_registry();
        return CU_get_error();
    }
    //END OF SECOND SUITE

    CU_pSuite treeSuite = NULL;
    treeSuite = CU_add_suite("test_Tree",0,0);

    if (NULL==CU_add_test(treeSuite,"test_Tree_Example",testTreeExample)){
        CU_cleanup_registry();
        return CU_get_error();
    }

    
    if (NULL==CU_add_test(treeSuite,"test_Tree_Insertion",testTreeInsertion)){
        CU_cleanup_registry();
        return CU_get_error();
    }
    //END OF THIRD SUITE

    CU_pSuite compHashSuite = NULL;
    compHashSuite = CU_add_suite("test_CompHash",0,0);

    if (NULL==CU_add_test(compHashSuite,"test_CompHash_Creation",testCompHashcreation)){
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL==CU_add_test(compHashSuite,"test_CompHash_Creation",testCompHashElements)){
        CU_cleanup_registry();
        return CU_get_error();
    }

    //END OF FOURTH SUITE
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}