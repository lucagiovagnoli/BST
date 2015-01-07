/*
 *  bst.c
 *
 *  Created by Luca Giovagnoli on 17/09/11.
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "bst.h"

typedef struct st_nodo{
    void* key;
	void* info;
	int num_nodi;
	struct st_nodo * dx;
	struct st_nodo * sx;
}NODE_BST;

struct tree{
	int (*compare)(void*,void*);
	void (*stampa)(void*);
	void (*free_dato)(void*);
	NODE_BST* root;
	int n;
	int altezza;
};

NODE_BST* rotate_sx(NODE_BST* albero);
NODE_BST* rotate_dx(NODE_BST* albero);
NODE_BST* estrai_min(NODE_BST* albero);
NODE_BST* estrai_max(NODE_BST* albero);

void calcola_altezza(NODE_BST* root,int h,int* max);
void free_node(NODE_BST* nodo,void (*free_data)(void*));
void free_albero(NODE_BST* root, void (*free_data)(void*));
void stampa_ricorsiva(NODE_BST* root,void (*stampa)(void*),int ordine);

NODE_BST* partiziona(NODE_BST* root,void* key,int (*compare)(void*,void*));
NODE_BST* insert_leaf_recursive(NODE_BST* root,void* key,void* dati,int (*compare)(void*,void*));
NODE_BST* inserisci_radice_ricorsiva(NODE_BST* root,void* key,void* dati,int (*compare)(void*,void*));
NODE_BST* delete_node(NODE_BST* root,void* key,int (*compare)(void*,void*),void (*libera)(void*));

void* get_info(NODE_BST* node){
    if(node==NULL)return NULL;
    else return node->info;
}


/******* ALLOCATE *******/
NODE_BST* nuovo_nodo(void* key, void* dati){
	NODE_BST* nuovo = (NODE_BST*)calloc(1,sizeof(NODE_BST));
	nuovo->key = key;
	nuovo->info = dati;
	nuovo->num_nodi = 1;
	return nuovo;
}

BST* create_BST(int (*compare)(void*,void*),void (*stampa)(void*),void (*free_dato)(void*)){
	BST* albero = (BST*)calloc(1, sizeof(BST));
	albero->compare = compare;
	albero->stampa = stampa;
	albero->free_dato = free_dato;
	return albero;
}


/******** INSERT ********/
NODE_BST* insert_leaf_recursive(NODE_BST* root,void* key, void* dati,int (*compare)(void*,void*)){

	if (root == NULL) return nuovo_nodo(key, dati);

	if (compare(key,root->key)>0) root->dx = insert_leaf_recursive(root->dx,key,dati,compare);
	else root->sx = insert_leaf_recursive(root->sx,key,dati,compare);

	(root->num_nodi)++;
	return root;
}

void insert_leaf_BST(BST* albero,void* key, void* dati){

	albero->root = insert_leaf_recursive(albero->root,key,dati,albero->compare);
	(albero->n)++;
}


NODE_BST* inserisci_radice_ricorsiva(NODE_BST* root,void* key, void* dati,int (*compare)(void*,void*)){

	if (root == NULL) return nuovo_nodo(key, dati);

	(root->num_nodi)++;
	if (compare(key,root->key)>0) {
		root->dx = inserisci_radice_ricorsiva(root->dx,key,dati,compare);
		root = rotate_sx(root);
	}
	else {
		root->sx = inserisci_radice_ricorsiva(root->sx,key,dati,compare);
		root = rotate_dx(root);
	}

	return root;
}

void insert_root_BST(BST* albero, void* key, void* dati){

	albero->root = inserisci_radice_ricorsiva(albero->root, key, dati, albero->compare);
	(albero->n)++;
}


/******* SEARCH ********/
NODE_BST* cerca(NODE_BST* root,void* key,int (*compare)(void*,void*)){

	if (root == NULL || compare(key,root->key)==0) return root;
	if (compare(key,root->key)>0) return cerca(root->dx,key,compare);
	if (compare(key,root->key)<0) return cerca(root->sx,key,compare);
}

void* search_BST(BST* albero,void* key){
	return get_info(cerca(albero->root,key,albero->compare));
}

/// select the k-th element (in ascending order) using the count in nodes
NODE_BST* select_recursive(NODE_BST* root,int k){

	int t;
	if (root==NULL) return NULL;

	if (root->sx != NULL) t = root->sx->num_nodi;
	else t = 0;

	if (t == k-1) return root;
	if (t > k-1) return select_recursive(root->sx,k);
	else return select_recursive(root->dx,k-1-t);
}

void* select_BST(BST* albero,int k){
	return select_recursive(albero->root,k)->info;
}


/// recursive versions
NODE_BST* estrai_min(NODE_BST* root){
	if (root==NULL) return NULL;
	if (root->sx == NULL) return root;
	return estrai_min(root->sx);
}

NODE_BST* estrai_max(NODE_BST* root){

	if (root==NULL) return NULL;
	if (root->dx == NULL) return root;
	return estrai_max(root->dx);
}

/// iterative versions, more efficient
void* min_BST(BST* albero){
    NODE_BST* elem  = albero->root;
    if(elem==NULL) return NULL;
    while(elem->sx!=NULL){
        elem = elem->sx;
    }
	return elem->info;
}
void* max_BST(BST* albero){
    NODE_BST* elem  = albero->root;
    if(elem==NULL) return NULL;
    while(elem->dx!=NULL){
        elem = elem->dx;
    }
	return elem->info;
}

/** successor is the min of the right sub-tree. If there is no sub-tree (dx==NULL)
 *  then the successor is the last ancestor where we turned left. **/

void* successor_BST(BST* albero,void* key){
    NODE_BST* elem = albero->root, *last_left_turn=NULL;

    while(elem!=NULL){
        int comparison = albero->compare(key,elem->key);
        if(comparison<0) {
            last_left_turn = elem;
            elem = elem->sx;
        }
        else if (comparison > 0) elem = elem->dx;
        else{
            if(elem->dx==NULL)  return get_info(last_left_turn);
            else return get_info(estrai_min(elem->dx));
        }
    }
    return NULL;
}

void* predecessor_BST(BST* albero,void* key){
    NODE_BST* elem = albero->root, *last_right_turn=NULL;

    while(elem!=NULL){
        int comparison = albero->compare(key,elem->key);
        if(comparison<0) elem = elem->sx;
        else if (comparison > 0) {
            last_right_turn = elem;
            elem = elem->dx;
        }
        else{
            if(elem->sx==NULL)  return get_info(last_right_turn);
            else return get_info(estrai_max(elem->sx));
        }
    }
    return NULL;
}

/********* PARTITION *********/
NODE_BST* rotate_sx(NODE_BST* root){

	NODE_BST* temp = root->dx;
	root->dx = temp->sx;
	temp->sx=root;

	temp->num_nodi = root->num_nodi;
	root->num_nodi = 1;
	if (root->dx != NULL) 	root->num_nodi += root->dx->num_nodi;
	if (root->sx != NULL) 	root->num_nodi += root->sx->num_nodi;

	return temp;
}

NODE_BST* rotate_dx(NODE_BST* root){

	NODE_BST* temp = root->sx;
	root->sx = temp->dx;
	temp->dx=root;

	temp->num_nodi = root->num_nodi;
	root->num_nodi = 1;
	if (root->dx != NULL) 	root->num_nodi += root->dx->num_nodi;
	if (root->sx != NULL) 	root->num_nodi += root->sx->num_nodi;

	return temp;
}

NODE_BST* partiziona(NODE_BST* root,void* key,int (*compare)(void*,void*)){

	if (root==NULL) return NULL;

	if (compare(key, root->key)==0) return root;
	if (compare(key, root->key)>0) {
		root->dx = partiziona(root->dx, key, compare);
		root = rotate_sx(root);
	}
	else {
		root->sx = partiziona(root->sx, key, compare);
		root = rotate_dx(root);
	}
	return root;
}

void partiziona_BST(BST* albero,void* key){

	albero->root = partiziona(albero->root,key,albero->compare);
}

NODE_BST* subtree_successor(NODE_BST* root){
	if (root==NULL) return NULL;
	if(root->dx!=NULL)	return estrai_min(root->dx);
}
NODE_BST* subtree_predecessor(NODE_BST* root){
	if (root==NULL) return NULL;
	return estrai_max(root->sx);
}

/******** DELETE *********/
NODE_BST* delete_node(NODE_BST* node,void* key,int (*compare)(void*,void*),void (*libera)(void*)){

	NODE_BST* temp,* to_delete;

	if (node == NULL) return NULL;
	(node->num_nodi)--;
    int comparison = compare(key,node->key);
	if (comparison==0){
        if(node->dx==NULL && node->sx == NULL){
            free_node(node, libera);
            return NULL;
        }
        if(node->dx != NULL && node->sx != NULL){
            to_delete = node;
            temp = node->sx;
			node = partiziona(node->dx, subtree_successor(node)->key, compare);
			node->sx = temp;
			node->num_nodi += temp->num_nodi;
            free_node(to_delete, libera);
			return node;
		}
		if(node->dx != NULL){
            temp = node->dx;
            free_node(node, libera);
            return temp;
		}
		if(node->sx != NULL){
            temp = node->sx;
            free_node(node, libera);
            return temp;
        }
	//	if (temp!=NULL) (node->num_nodi) += temp->num_nodi;
	}
	else if (comparison>0) node->dx = delete_node(node->dx,key,compare,libera);
	else node->sx = delete_node(node->sx,key,compare,libera);

	return node;
}

void delete_BST(BST* albero,void* key){
	albero->root = delete_node(albero->root, key, albero->compare,albero->free_dato);
	(albero->n)--;
}

/***** MEMORY FREE ******/
void free_node(NODE_BST* nodo, void (*free_data)(void*)){

//    free(nodo->key); ///CAREFUL, you still cant provide a free function for the key
    if(free_data!= NULL) free_data(nodo->info);
	free(nodo);
}

void free_albero(NODE_BST* root, void (*free_data)(void*)){

	if (root == NULL) return;
	free_albero(root->sx,free_data);
	free_albero(root->dx,free_data);
	free_node(root, free_data);
}

void free_BST(BST* albero){

	free_albero(albero->root, albero->free_dato);
}


/**** PRINT ******/
int get_number_nodes(BST* albero){
    return albero->n;
}

void compute_height(NODE_BST* root,int h,int* max){

	if (h>*max) *max = h;
	if (root==NULL) return;
	compute_height(root->sx,h+1,max);
	compute_height(root->dx,h+1,max);
}

int get_height(BST* albero){

	int max = INT_MIN;
	compute_height(albero->root, 0, &max);
    return max;
}

void draw_node(NODE_BST* node, int height, int current_level,void (*stampa)(void*)){

    if(node==NULL) return;

    stampa(node->info);

    //print edges
    printf("\033[%dB",1);   // cursor down 1 level
    if(node->dx != NULL) {
        printf("\\");
        printf("\033[%dD",1); //  go back one
    }
    printf("\033[%dD",2); //move left
    if(node->sx != NULL) {
        printf("/");
        printf("\033[%dD",1); //  go back one
    }

    printf("\033[%dB",1);   // cursor down 1 level1
   // printf("------\033[%dD",6); //  print delimiter and go back cursor
//    printf("\033[%dB",1);   // cursor down 1 level

    printf("\033[%dD",(height-current_level)); //move left
    draw_node(node->sx,height,current_level+1,stampa);
    printf("\033[%dC",(height-current_level)); //move right
    printf("\033[%dC",3+(height-current_level)); //move right
    draw_node(node->dx,height,current_level+1,stampa);
    printf("\033[%dD",(height-current_level)); //move left

    printf("\033[%dD",1); //move left
    printf("\033[%dA",2);   // cursor up

}

void draw_BST(BST* tree){

    int i,z,h = get_altezza(tree);

    printf("\033[%dC",h*3); //move right
    draw_node(tree->root,h,0,tree->stampa);
    printf("\033[%dB",h*4+1);   // cursor down height*2+1 lines
}


void print_level(NODE_BST* node,int level_to_print,int current_level,void (*stampa)(void*)){

    if (node == NULL) return;

    if(level_to_print==current_level){
        stampa(node->info);
        printf("(");
        if(node->sx!=NULL) printf("l");
        if(node->dx!=NULL) printf("r");
        printf(")");
        return;
    }

    print_level(node->sx,level_to_print,current_level+1,stampa);
    print_level(node->dx,level_to_print,current_level+1,stampa);

}

void debug_BST(BST* tree){
    int i,z,h = get_altezza(tree);

    printf("\n");
    for(i=0;i<h;i++){
        print_level(tree->root,i,0,tree->stampa);
        printf("\n------------------------------------------------------------\n");
    }

}


void stampa_ricorsiva(NODE_BST* root,void (*stampa)(void*),int ordine){

	if (root==NULL) return;

	if (ordine == PREORDER) {printf("%d",root->num_nodi);stampa(root->info);}
	stampa_ricorsiva(root->sx,stampa,ordine);
	if (ordine == INORDER) stampa(root->info);
	stampa_ricorsiva(root->dx,stampa,ordine);
	if (ordine == POSTORDER) stampa(root->info);
}

void stampa_BST(BST* albero,int ordine){

	stampa_ricorsiva(albero->root,albero->stampa,ordine);
}
