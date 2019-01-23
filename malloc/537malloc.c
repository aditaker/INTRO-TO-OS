#include "537malloc.h"

void preFreeHandler(void *address) {
	struct Node* node = searchInitialAddress(*root, address);
	if(node && node->freed == true) {
		printf("Freeing memory that was previously freed (double free).\n");
		exit(1);
	}
	else if(!node) {
		if(searchNonInitialAddress(*root,address))
			printf("Freeing memory from non-initial byte of allocated memory.\n");
		else
			printf("Freeing memory that has not be allocated with malloc537().\n");
		exit(1);
	}
}

void preReallocHandler(void *address) {
	struct Node* node = searchInitialAddress(*root, address);
	if(!node) {
		if(searchNonInitialAddress(*root,address))
			printf("Realloc'ing memory from non-initial byte of allocated memory.\n");
		else
			printf("Realloc'ing memory that has not be allocated with malloc537().\n");
		exit(1);
	}
}

void *malloc537(size_t size){
	if(size == 0)
		printf("Warning: Malloc'ing memory of size 0\n");
	void *address = malloc(size);
	insertNode(address, size);
	return address;
}

void free537(void *ptr){
	struct Node* node = searchInitialAddress(*root, ptr);
	preFreeHandler(ptr);
	free(ptr);
	node->freed = true;
}

void *realloc537(void *ptr, size_t size){
	if(ptr == NULL)
		return malloc537(size);
	preReallocHandler(ptr);
	if(size == 0)
		printf("Warning: Realloc'ing memory to size 0\n");

	struct Node* node = searchInitialAddress(*root, ptr);
	ptr = realloc(ptr, size);
	if(ptr != node->address) {
		node->freed = true;
	}
	insertNode(ptr, size);
	return ptr;
}

void memcheck537(void *ptr, size_t size){
	struct Node *tmp = searchValidRange(*root, ptr, size);
	if(tmp == NULL)
	{
		printf("Given memory range is not allocated\n");
		exit(1);
	}
}

void inorderHelper(struct Node *root) 
{ 
	if (root == NULL) 
		return; 

	inorderHelper(root->left); 
	printf("%p %d %d ",root->address, root->freed, root->len); 
	inorderHelper(root->right); 
} 

void print() {
	printf("\n");
	inorderHelper(*root);
	printf("\n");
}

// int main() {
// 	// int *a = (int *)malloc537(7*sizeof(int));
// 	// int *y = (int *)malloc537(6*sizeof(int));
// 	// int *b = (int *)malloc537(5*sizeof(int));
// 	// int *c = (int *)malloc537(4*sizeof(int));
// 	// int *x = (int *)malloc537(3*sizeof(int));
// 	// int *z = (int *)malloc537(2*sizeof(int));
// 	// int *k = (int *)malloc537(1*sizeof(int));
// 	// int *t = (int *)malloc537(9*sizeof(int));
// 	// inorderHelper(*root);
// 	// free537(t);
// 	// free537(z);
// 	// free537(c);
// 	// //deleteNode(search(*root,5));
// 	// printf("\n");
// 	// inorderHelper(*root);
// 	// printf("\n");
// 	// void * tmp = malloc(10);
// 	// printf("%x\n", tmp);
// 	// free(tmp);
// 	// tmp = malloc(10);
// 	// printf("%x\n", tmp);

// 	void *tmp = malloc537(10);
// 	memcheck537(tmp, 5);
// 	print();
// 	void *tmp1 = malloc537(10);
// 	print();
// 	void *tmp2 = malloc537(10);
// 	print();

// 	free537(tmp1);
// 	free537(tmp2);
// 	print();
// 	tmp = realloc537(tmp, 15);
// 	print();

// 	return 0;
// }
