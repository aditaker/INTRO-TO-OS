#include "range_tree.h"

Node* rootNode = NULL;
Node** root = &rootNode;

//Insertion code begins

struct Node* BSTInsert(Node* node, Node *ptr) 
{
	/* If the tree is empty, return a new node */
    if (node == NULL) 
       return ptr;
	if (ptr->address < node->address) 
	{ 
		node->left  = BSTInsert(node->left, ptr); 
		node->left->parent = node; 
	} 
	else if (ptr->address > node->address) 
	{ 
		node->right = BSTInsert(node->right, ptr); 
		node->right->parent = node; 
	} 

	/* return the (unchanged) node pointer */
	return node; 
} 

void rotateLeft(Node **node, Node *ptr) 
{ 
	struct Node *ptr_right = ptr->right; 

	ptr->right = ptr_right->left; 

	if (ptr->right != NULL) 
		ptr->right->parent = ptr; 

	ptr_right->parent = ptr->parent; 

	if (ptr->parent == NULL) {
		*node = ptr_right; 
	}

	else if (ptr == ptr->parent->left) 
		ptr->parent->left = ptr_right; 

	else
		ptr->parent->right = ptr_right; 

	ptr_right->left = ptr; 
	ptr->parent = ptr_right; 
} 

void rotateRight(Node **node, Node *ptr) 
{ 
	struct Node *ptr_left = ptr->left; 

	ptr->left = ptr_left->right; 

	if (ptr->left != NULL) 
		ptr->left->parent = ptr; 

	ptr_left->parent = ptr->parent; 

	if (ptr->parent == NULL) {
		*node = ptr_left; 
	}

	else if (ptr == ptr->parent->left) 
		ptr->parent->left = ptr_left; 

	else
		ptr->parent->right = ptr_left; 

	ptr_left->right = ptr; 
	ptr->parent = ptr_left; 
} 

// This function fixes violations caused by BST insertion 
void fixViolation(Node **node, Node *ptr) 
{ 
	Node *parent_ptr = NULL; 
	Node *grand_parent_ptr = NULL; 

	while ((ptr != *node) && (ptr->color != 'B') && 
		(ptr->parent != NULL) && (ptr->parent->color == 'R'))
	{

		parent_ptr = ptr->parent; 
		grand_parent_ptr = ptr->parent->parent; 

		/*  Case : A 
			Parent of ptr is left child of Grand-parent of pt */
		if (parent_ptr == grand_parent_ptr->left) 
		{ 

			Node *uncle_ptr = grand_parent_ptr->right; 

			/* Case : 1 
			   The uncle of ptr is also red 
			   Only Recoloring required */
			if (uncle_ptr != NULL && uncle_ptr->color == 'R') 
			{ 
				grand_parent_ptr->color = 'R'; 
				parent_ptr->color = 'B'; 
				uncle_ptr->color = 'B'; 
				ptr = grand_parent_ptr; 
			} 

			else
			{ 
				/* Case : 2 
				   ptr is right child of its parent 
				   Left-rotation required */
				if (ptr == parent_ptr->right) 
				{ 
					rotateLeft(node, parent_ptr); 
					ptr = parent_ptr; 
					parent_ptr = ptr->parent; 
				} 

				/* Case : 3 
				   ptr is left child of its parent 
				   Right-rotation required */
				rotateRight(node, grand_parent_ptr); 
				char temp = parent_ptr->color;
				parent_ptr->color = grand_parent_ptr->color;
				grand_parent_ptr->color = temp;
				ptr = parent_ptr; 
			} 
		} 

		/* Case : B 
		   Parent of ptr is right child of Grand-parent of ptr */
		else
		{ 
			struct Node *uncle_ptr = grand_parent_ptr->left; 

			/*  Case : 1 
				The uncle of ptr is also red 
				Only Recoloring required */
			if ((uncle_ptr != NULL) && (uncle_ptr->color == 'R')) 
			{ 
				grand_parent_ptr->color = 'R'; 
				parent_ptr->color = 'B'; 
				uncle_ptr->color = 'B'; 
				ptr = grand_parent_ptr; 
			} 
			else
			{ 
				/* Case : 2 
				   ptr is left child of its parent 
				   Right-rotation required */
				if (ptr == parent_ptr->left) 
				{ 
					rotateRight(node, parent_ptr); 
					ptr = parent_ptr; 
					parent_ptr = ptr->parent; 
				} 

				/* Case : 3 
				   ptr is right child of its parent 
				   Left-rotation required */
				rotateLeft(node, grand_parent_ptr); 
				char temp = parent_ptr->color;
				parent_ptr->color = grand_parent_ptr->color;
				grand_parent_ptr->color = temp;
				ptr = parent_ptr; 
			} 
		} 
	} 
	(*node)->color = 'B'; 
}

void insertNode(void *address, int len) {
	Node *old = searchInitialAddress(*root, address);
	if(old != NULL)
	{
		old->freed = false;
		old->len = len;
		return;
	}
	Node *tmp = NULL;
	while(root != NULL && ((tmp = searchOverlappingRange(*root, address, len)) != NULL))
	{
		deleteNode(tmp);
	}

	Node *node = (struct Node*)malloc(sizeof(struct Node));
	node->address = address;
	node->len = len;
	node->freed = false;
	node->parent = NULL;
	node->right = NULL;
	node->left = NULL;
	node->color = 'R';
	
	if(root != NULL) {
		*root = BSTInsert(*root, node);
		fixViolation(root, node);
	}
}

//Insertion code ends


//Deletion code begins

// check if node is left child of parent 
bool isOnLeft(Node* node) { 
	return node == node->parent->left; 
} 

// returns pointer to uncle 
Node *uncleNode(struct Node *node) { 
	// If no parent or grandparent, then no uncle 
	if (node->parent == NULL || node->parent->parent == NULL) 
		return NULL; 

	if (isOnLeft(node->parent)) 
		// uncle on right 
		return node->parent->parent->right; 
	else
		// uncle on left 
		return node->parent->parent->left; 
}

// returns pointer to sibling 
struct Node *siblingNode(struct Node* node) { 
	// sibling null if no parent 
	if (node->parent == NULL) 
		return NULL; 
	if (isOnLeft(node)) 
		return node->parent->right;
	return node->parent->left; 
} 

// moves node down and moves given node in its place 
void moveDown(Node* node, Node *parent) { 
	if (node->parent != NULL) { 
		if (isOnLeft(node)) { 
			node->parent->left = parent; 
		} else { 
			node->parent->right = parent; 
		}
	}
	parent->parent = node->parent; 
	node->parent = parent; 
} 

bool hasRedChild(Node* node) { 
	return (node->left != NULL && node->left->color == 'R') || 
		(node->right != NULL && node->right->color == 'R'); 
} 

// find node that do not have a left child 
// in the subtree of the given node 
struct Node *successor(struct Node *node) { 
	struct Node *temp = node;

	while (temp->left != NULL) 
		temp = temp->left; 

	return temp; 
} 

// find node that replaces a deleted node in BST 
Node *BSTreplace(struct Node *node) { 
	// when node have 2 children 
	if (node->left != NULL && node->right != NULL) 
		return successor(node->right); 

	// when leaf 
	if (node->left == NULL && node->right == NULL) 
		return NULL; 

	// when single child 
	if (node->left != NULL) 
		return node->left; 
	else
		return node->right; 
} 

void swapColors(Node *node1, Node *node2) { 
	char temp;
	temp = node1->color; 
	node1->color = node2->color; 
	node2->color = temp; 
} 

void swapValues(Node *node1, Node *node2) { 
	void *temp; 
	temp = node1->address; 
	node1->address = node2->address; 
	node2->address = temp; 
	int tmp;
	tmp = node1->len; 
	node1->len = node2->len; 
	node2->len = tmp; 
} 

// fix red red at given node 
void fixRedRed(Node *node) { 
	// if x is root color it black and return 
	if (node == *root) { 
		node->color = 'B'; 
		return; 
	} 

	// initialize parent, grandparent, uncle 
	Node *parent = node->parent, *grandparent = parent->parent, *uncle = uncleNode(node); 

	if (parent->color != 'B') { 
		if (uncle != NULL && uncle->color == 'R') { 
			// uncle red, perform recoloring and recurse 
			parent->color = 'B'; 
			uncle->color = 'B'; 
			grandparent->color = 'R'; 
			fixRedRed(grandparent); 
		} else { 
			// Else perform LR, LL, RL, RR 
			if (isOnLeft(parent)) { 
				if (isOnLeft(node)) { 
					// for left right 
					swapColors(parent, grandparent); 
				} else { 
					rotateLeft(root, parent); 
					swapColors(node, grandparent); 
				}
				// for left left and left right 
				rotateRight(root, grandparent); 
			} else { 
				if (isOnLeft(node)) { 
					// for right left 
					rotateRight(root, parent); 
					swapColors(node, grandparent); 
				} else {
					swapColors(parent, grandparent); 
				}
				// for right right and right left 
				rotateLeft(root, grandparent); 
			}
		}
	}
}

void fixDoubleBlack(Node *node) { 
	if (node == *root)
		// Reached root 
		return; 

	Node *sibling = siblingNode(node), *parent = node->parent; 
	if (sibling == NULL) {
		// No sibiling, double black pushed up 
		fixDoubleBlack(parent); 
	} else { 
		if (sibling->color == 'R') {
			// Sibling red 
			parent->color = 'R'; 
			sibling->color = 'B'; 
			if (isOnLeft(sibling)) { 
				// left case 
				rotateRight(root, parent); 
			} else { 
				// right case 
				rotateLeft(root, parent); 
			}
			fixDoubleBlack(node); 
		} else {
			// Sibling black 
			if (hasRedChild(sibling)) {
				// at least 1 red children 
				if (sibling->left != NULL && sibling->left->color == 'R') {
					if (isOnLeft(sibling)) { 
						// left left 
						sibling->left->color = sibling->color; 
						sibling->color = parent->color; 
						rotateRight(root, parent); 
					} else { 
						// right left 
						sibling->left->color = parent->color; 
						rotateRight(root, sibling); 
						rotateLeft(root, parent); 
					}
				} else {
					if (isOnLeft(sibling)) { 
						// left right 
						sibling->right->color = parent->color; 
						rotateLeft(root, sibling); 
						rotateRight(root, parent); 
					} else { 
						// right right 
						sibling->right->color = sibling->color; 
						sibling->color = parent->color; 
						rotateLeft(root, parent); 
					}
				}
				parent->color = 'B'; 
			} else {
				// 2 black children 
				sibling->color = 'R'; 
				if (parent->color == 'B') 
					fixDoubleBlack(parent); 
				else
					parent->color = 'B'; 
			} 
		} 
	} 
} 

// deletes the given node 
void deleteNode(Node *v) { 
	Node *u = BSTreplace(v); 

	// True when u and v are both black 
	bool uvBlack = ((u == NULL || u->color == 'B') && (v->color == 'B')); 
	struct Node *parent = v->parent; 

	if (u == NULL) { 
		// u is NULL therefore v is leaf 
		if (v == *root) { 
			// v is root, making root null 
			root = NULL; 
		} else { 
			if (uvBlack) { 
				// u and v both black 
				// v is leaf, fix double black at v 
				fixDoubleBlack(v); 
			} else { 
				// u or v is red 
				if (siblingNode(v) != NULL) 
					// sibling is not null, make it red" 
					siblingNode(v)->color = 'R'; 
			} 

			// delete v from the tree 
			if (isOnLeft(v)) { 
				parent->left = NULL; 
			} else { 
				parent->right = NULL; 
			} 
		} 
		free(v); 
		return; 
	} 

	if (v->left == NULL || v->right == NULL) { 
		// v has 1 child 
		if (v == *root) { 
			// v is root, assign the value of u to v, and delete u 
			v->address = u->address;
			v->len = u->len; 
			v->left = v->right = NULL; 
			free(u); 
		} else { 
			// Detach v from tree and move u up 
			if (isOnLeft(v)) { 
				parent->left = u; 
			} else { 
				parent->right = u; 
			} 
			free(v); 
			u->parent = parent; 
			if (uvBlack) { 
				// u and v both black, fix double black at u 
				fixDoubleBlack(u); 
			} else { 
				// u or v red, color u black 
				u->color = 'B'; 
			} 
		} 
		return; 
	} 

	// v has 2 children, swap values with successor and recurse 
	swapValues(u, v); 
	deleteNode(u); 
} 

// Deletion code ends


struct Node* searchInitialAddress(struct Node* root, void *address) {
	if(!root)
		return NULL;
	if(root->address == address)
		return root;
	if(root->address < address)
		return searchInitialAddress(root->right, address);
	else
		return searchInitialAddress(root->left, address);
}

struct Node* searchOverlappingRange(struct Node* root, void *address, int length) {
	if(!root)
		return NULL;
	if(root->address < address && address < (root->address + root->len))
		return root;
	if(address < root->address && root->address < (address + length))
		return root;
	if(root->address < address)
		return searchOverlappingRange(root->right, address, length);
	else
		return searchOverlappingRange(root->left, address, length);
}

struct Node* searchNonInitialAddress(struct Node* root, void *address) {
	if(!root)
		return NULL;
	if(root->address == address)
		return NULL;
	if(root->address < address && address < (root->address + root->len))
		return root;
	if(root->address < address)
		return searchNonInitialAddress(root->right, address);
	else
		return searchNonInitialAddress(root->left, address);
}

struct Node* searchValidRange(struct Node* root, void *address, int len)
{
	if(!root)
		return NULL;
	if(root->address <= address && ((root->address + root->len) >= (address + len)))
		return root;
	if(root->address < address)
		return searchValidRange(root->right, address, len);
	else
		return searchValidRange(root->left, address, len);
}
