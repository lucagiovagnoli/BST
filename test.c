#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bst.h"

void print_data(void* str){
    printf("%s",str);
}

void free_data(void* data){
    free((char*)data);
}

int main (int argc, const char * argv[]) {

	    int i;
    char* data[N];
    char matrix[N][BUF] = {{"France\0"},{"Paris\0"},{"England\0"},{"London\0"},{"Sweden\0"},{"Stockholm\0"},{"Germany\0"},
        {"Berlin\0"},{"Norway\0"}, {"Oslo\0"},{"Italy\0"}, {"Rome\0"},{"Spain\0"}, {"Madrid\0"},{"Estonia\0"}, {"Tallinn\0"},{"Netherlands\0"}, {"Amsterdam\0"},{"Ireland\0"}, {"Dublin\0"}};

	BST* albero = create_BST(strcmp, print_string, free_data);

    for(i=0;i<N;i++){
        printf("Inserting element %d:\n",i);
        data[i] = (char*)malloc(BUF*sizeof(char));
        strcpy(data[i],matrix[i]);
        insert_leaf_BST(albero, matrix[i], data[i]);
        bst_debug(table);
    }

    print_data(hash_table_get(table,"France"));
    hash_table_remove(table,"Rome");
    hash_table_print(table);
    hash_table_free(table);

	char prova1[2] = "a\0";
	char prova2[2] = "s\0";
	char prova3[2] = "e\0";
	char prova4[2] = "c\0";
	char prova5[2] = "r\0";
	char prova6[2] = "h\0";
	char prova7[2] = "g\0";
	char prova8[2] = "i\0";
	char prova9[2] = "n\0";
	char prova10[2] = "l\0";
	char prova11[2] = "x\0";
	char prova12[2] = "t\0";
	char prova13[2] = "z\0";

	char prova14[2] = "b\0";
	char prova15[2] = "y\0";


	insert_leaf_BST(albero, prova2, prova2);
	insert_leaf_BST(albero, prova3, prova3);
	insert_leaf_BST(albero, prova4, prova4);
	insert_leaf_BST(albero, prova5, prova5);
	insert_leaf_BST(albero, prova6, prova6);
	insert_leaf_BST(albero, prova7, prova7);
	insert_leaf_BST(albero, prova8, prova8);
	insert_leaf_BST(albero, prova9, prova9);
	insert_leaf_BST(albero, prova10, prova10);
	insert_leaf_BST(albero, prova11, prova11);
	insert_leaf_BST(albero, prova12, prova12);
	insert_leaf_BST(albero, prova13, prova13);


//	insert_root_BST(albero, prova14, prova14);
//	insert_root_BST(albero, prova15, prova15);

	printf("\nInorder (Ordinamento crescente) : ");
	stampa_BST(albero, INORDINE);

	char chiave[2] = "s\0";
//    partiziona_BST(albero, &chiave);
	delete_BST(albero, chiave);

 //   draw_BST(albero);
    debug_BST(albero);

	printf("\nPreorder : ");
	stampa_BST(albero, PREORDINE);
	printf("\nInorder (Ordinamento crescente) : ");
	stampa_BST(albero, INORDINE);
	printf("\nPostorder : ");
	stampa_BST(albero, POSTORDINE);

	printf("\n\n-------------------------\n");

	printf("\nElemento massimo : ");
	printf("%s",max_BST(albero));
	printf("\nElemento minimo : ");
    printf("%s",min_BST(albero));

	printf("\nIl successore di i è : ");
	printf("%s", successor_BST(albero, "i"));
	printf("\nIl predecessore di g è : ");
	printf("%s", predecessor_BST(albero, "g"));

	printf("\nRicerca di i : ");
    printf("%s",search_BST(albero, "i"));


	printf("\nElemento number 7th : ");
    printf("%s",select_BST(albero, 7));

	printf("\nNumero di nodi dell'albero : ");
	printf("%d",get_numero_nodi(albero));

	printf("\nAltezza massima dell'albero : ");
	printf("%d",get_altezza(albero));

	free_BST(albero);

	printf("\nHello, World!\n");
    return 0;
}


