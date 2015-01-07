/*
 *  bst.h
 *  Binart Search Tree. Uses both recursive and iterative functions to go through the tree.
 *  Inspired by the Sedgewick "Algorithms in C".
 *
 *  Created by Luca Giovagnoli on 17/09/11.
 */

#define PREORDER -1
#define INORDER 0
#define POSTORDER 1

typedef struct tree BST;

BST* create_BST(int (*compara)(void*,void*),void (*stampa)(void*),void (*free_dato)(void*));

/** SEARCHING the TREE **/
void* min_BST(BST* albero);
void* max_BST(BST* albero);
void* successor_BST(BST* albero,void* key);
void* predecessor_BST(BST* albero,void* key);
void* search_BST(BST* albero,void* key);
void* select_BST(BST* albero,int k);

/** INSERT **/
void insert_leaf_BST(BST* albero, void* key, void* dati);
void insert_root_BST(BST* albero, void* key, void* dati);

void partiziona_BST(BST* albero, void* chiave);


/** TREE PROPERTIES**/
int get_number_nodes(BST* albero);
int get_height(BST* albero);

/** PRINT **/
void stampa_BST(BST* albero,int ordine);
void draw_BST(BST* tree); ///if the tree is very dense dont use the draw function, it will be very confusing and overlapping
void debug_BST(BST* tree);///prints each level of the tree, all the nodes. In the brackets there are the non-NULL right or left pointers of the node

/** FREE and DELETE ELEMENT**/
void delete_BST(BST* albero, void* chiave);
void free_BST(BST* albero);




