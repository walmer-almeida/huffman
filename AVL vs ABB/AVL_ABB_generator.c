#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct binaryTree bt;
typedef struct avlTree avl;


//     ### ABB ###


struct binaryTree
{
	int item;
	bt *left;
	bt *right;
};

bt *createBinaryTree(int item, bt *left, bt *right)
{
	bt *new_bt = (bt*) malloc(sizeof(bt));
	new_bt->item = item;
	new_bt->left = left;
	new_bt->right = right;

	return new_bt;
}

bt *btAddLeaf(bt *tree, int item)
{
	if(tree == NULL)
	{
		tree = createBinaryTree(item, NULL, NULL);
	}
	else if(tree->item > item)
	{
		tree->left = btAddLeaf(tree->left, item);
	}
	else
	{
		tree->right = btAddLeaf(tree->right, item);
	}

	return tree;
}


//		### AVL ###


struct avlTree
{
	int item;
	int h;
	avl *left;
	avl *right;
};

int max(int a, int b)
{
	return (a > b) ? a : b;
}

int height(avl *tree)
{
	if(tree == NULL)
	{
		return -1;
	}
	else
	{
		return 1 + max(height(tree->left), height(tree->right));
	}
}

avl *createAVLTree(int item, avl *left, avl *right)
{
	avl *new = (avl*)malloc(sizeof(avl));
	new->item = item;
	new->left = left;
	new->right = right;
	new->h = height(new);

	return new;
}

avl *rotateLeft(avl *tree)
{
	avl *subtreeRoot = NULL;
	if(tree != NULL && tree->right != NULL)
	{
		subtreeRoot = tree->right;
		tree->right	= subtreeRoot->left;
		subtreeRoot->left = tree;
	}
	subtreeRoot->h = height(subtreeRoot);
	tree->h = height(tree);

	return subtreeRoot;
}

avl *rotateRight(avl *tree)
{
	avl *subtreeRoot = NULL;
	if(tree != NULL & tree->left != NULL)
	{
		subtreeRoot = tree->left;
		tree->left = subtreeRoot->right;
		subtreeRoot->right = tree;
	}
	subtreeRoot->h = height(subtreeRoot);
	tree->h = height(tree);

	return subtreeRoot;
}

int balanceFactor(avl *tree)
{
	if(tree != NULL)
	{
		return (height(tree->left) - height(tree->right));
	}
	return 0;
}

avl *avlAddLeaf(avl *tree, int item)
{
	if(tree == NULL)
	{
		return createAVLTree(item, NULL, NULL);
	}
	else if(tree->item > item)
	{
		tree->left = avlAddLeaf(tree->left, item);
	}
	else
	{
		tree->right = avlAddLeaf(tree->right, item);
	}

	tree->h = height(tree);
	avl *child;

	if(balanceFactor(tree) == 2 || balanceFactor(tree) == -2)
	{
		if(balanceFactor(tree) == 2)
		{
			child = tree->left;
			if(balanceFactor(child) == -1)
			{
				tree->left = rotateLeft(child);
			}
			tree = rotateRight(tree);
		}
		else if(balanceFactor(tree) == -2)
		{
			child = tree->right;

			if(balanceFactor(child) == 1)
			{
				tree->right = rotateRight(child);
			}
			tree = rotateLeft(tree);
		}
	}
	return tree;
}

int ABB_AVL_counter(avl *tree, int item)
{
	if(tree == NULL || tree->item == item)
	{
		return 1;
	}
	else if(tree->item > item)
	{
		return 1 + ABB_AVL_counter(tree->left, item);
	}
	else
	{
		return 1 + ABB_AVL_counter(tree->right, item);
	}
}

int main()
{
	bt *binaryTree = NULL;
	avl *avlTree = NULL; 

	FILE *Comp_Data = NULL;

	Comp_Data = fopen("CompData.txt", "w");

	fprintf(Comp_Data, "%s\t%s\t%s\t%s\n", "Comparações_AVL", "Tamanho_AVL", "Comparações_ABB", "Tamanho_ABB");

	int i, x, index, array_size, counter;
	int countAVL = 0, countABB = 0;
	srand(time(NULL));

	printf("Insira o tamanho do array:\t");
	scanf("%d", &array_size);
	printf("Insira quantas buscas deve-se fazer\t");
	scanf("%d", &counter);

	int *array = (int*)malloc(sizeof(int) * array_size);
	int *array_visited = (int*)calloc(array_size, sizeof(int));

	for(i = 0; i < array_size; i++)
	{
		x = rand();
		array[i] = x;
		binaryTree = btAddLeaf(binaryTree, x);
		avlTree = avlAddLeaf(avlTree, x);
	}

	for(i = 0; i < counter; i++)
	{
		index = rand() % array_size;
		if(!array_visited[index])
		{
			countABB = ABB_AVL_counter(binaryTree, array[index]);
			countAVL = ABB_AVL_counter(avlTree, array[index]);
			array_visited[index] = 1;

			fprintf(Comp_Data, "%d\t%d\t%d\t%d\n", countAVL, index+1, countABB, index+1);
		}
	}

	free(array);
	free(array_visited);

	return 0;
}