/*
 * TAD fila
 * Autor:
 *   Mateus Siqueira Ruzene - GRR:20221223
 */
#include <stdlib.h>
#include <stdio.h>
#include "libfila.h"

/*
 * Cria uma fila vazia e a retorna, se falhar retorna NULL.
 */
fila_t * cria_fila (){
    fila_t *fila;

    if(!(fila = malloc(sizeof(fila_t))))
        return NULL;

    fila->tamanho = 0;

    return fila;
}

/*
 * Remove todos os elementos da fila, libera espaco e devolve NULL.
 */
fila_t * destroi_fila (fila_t *f){
    nodo_f_t *aux;
    // if(!(aux = malloc(sizeof(nodo_f_t))))
    //     return f;
    while(f->fim != NULL){
        aux = (f->ini);
        f->ini = (f->ini)->prox;
        free(aux);
        aux = NULL;
    }
    free(f);
    f = NULL;

    return f;
}

/*
 * Retorna 1 se a fila esta vazia e 0 caso contrario.
 */
int fila_vazia (fila_t *f){
    if(f->tamanho == 0)
        return 1;
    
    return 0;
}

/*
 * Retorna o tamanho da fila, isto eh, o numero de elementos presentes nela.
 */
int tamanho_fila (fila_t *f){
    return f->tamanho;
}

/*
 * Insere o elemento no final da fila (politica FIFO).
 * Retorna 1 se a operacao foi bem sucedida e 0 caso contrario.
 */
int insere_fila (fila_t *f, int elemento){
    nodo_f_t *novo;

    if(fila_vazia(f)){
        if(!(f->ini = malloc(sizeof(nodo_f_t))) && !(f->fim = malloc(sizeof(nodo_f_t))))
            return 0;

        (f->ini)->chave = elemento;
        (f->ini)->prox = NULL;
        f->fim = f->ini;
        
        return 1;
    } else {
        if(!(novo = malloc(sizeof(nodo_f_t))))
            return 0;
        novo->chave = elemento;
        novo->prox = NULL;
        (f->fim)->prox = novo;
        f->fim = novo;

        return 1;
    }    
}

/*
 * Remove o elemento do inicio da fila (politica FIFO) e o retorna.
 * Retorna 1 se a operacao foi bem sucedida e 0 caso contrario.
 */
int retira_fila (fila_t *f, int *elemento){
    if(fila_vazia(f))
        return 0;
}

