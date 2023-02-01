/*
 * TAD LEF
 * Autor:
 *   Mateus Siqueira Ruzene - GRR:20221223
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "liblef.h"
/*
 * Cria uma LEF vazia
 */
lef_t *cria_lef()
{
    lef_t *lef;
    if (!(lef = malloc(sizeof(lef_t))))
        return NULL;

    lef->Primeiro = NULL;

    return lef;
}

/*
 * Destroi a LEF e retorna NULL.
 */
lef_t *destroi_lef(lef_t *l)
{
    nodo_lef_t *aux;

    while (l->Primeiro != NULL)
    {
        aux = l->Primeiro;
        l->Primeiro = (l->Primeiro)->prox;
        free(aux->evento);
        free(aux);
    }
    free(l);
    l = NULL;

    return l;
}

/*
 * Adiciona um evento na primeira posicao da LEF. Uma nova copia
 * da estrutura evento sera feita.
 * Retorna 1 em caso de sucesso, 0 caso contrario.
 */
int adiciona_inicio_lef(lef_t *l, evento_t *evento)
{
    nodo_lef_t *novo;
    evento_t *eventoCopy;

    if (!(eventoCopy = malloc(sizeof(evento_t))))
        return 0;
    if (!(novo = malloc(sizeof(nodo_lef_t))))
        return 0;

    memcpy(eventoCopy, evento, sizeof(evento_t));

    novo->evento = eventoCopy;
    novo->prox = l->Primeiro;
    l->Primeiro = novo;

    return 1;
}

/*
 * Adiciona um evento de acordo com o valor evento->tempo na LEF.
 * Uma nova copia da estrutura evento sera feita
 * Retorna 1 em caso de sucesso, 0 caso contrario.
 */
int adiciona_ordem_lef(lef_t *l, evento_t *evento)
{
    nodo_lef_t *novo, *aux;
    evento_t *eventoCopy;

    if (!(l->Primeiro))
        return adiciona_inicio_lef(l, evento);

    if (!(eventoCopy = malloc(sizeof(evento_t))))
        return 0;
    if (!(novo = malloc(sizeof(nodo_lef_t))))
        return 0;

    memcpy(eventoCopy, evento, sizeof(evento_t));

    aux = l->Primeiro;
    novo->evento = eventoCopy;
    while ((aux->prox) && eventoCopy->tempo >= ((aux->prox)->evento)->tempo)
    {
        aux = aux->prox;
    }

    novo->prox = aux->prox;
    aux->prox = novo;

    return 1;
}

/*
 * Retorna e retira o primeiro evento da LEF.
 * A responsabilidade por desalocar
 * a memoria associada eh de quem chama essa funcao.
 */
evento_t *obtem_primeiro_lef(lef_t *l)
{
    evento_t *evento;
    nodo_lef_t *aux;

    evento = (l->Primeiro)->evento;
    aux = l->Primeiro;
    l->Primeiro = (l->Primeiro)->prox;

    free(aux);
    return evento;
}