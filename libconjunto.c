#include <stdlib.h>
#include <stdio.h>

#include "libconjunto.h"

int busca_binaria(int vetor[], int esq, int dir, int x)
{
    while (esq <= dir)
    {
        int m = esq + (dir - esq) / 2;

        if (vetor[m] == x)
            return m;
        if (vetor[m] < x)
            esq = m + 1;
        else
            dir = m - 1;
    }

    return -1;
}

conjunto_t *cria_cjt(int max)
{
    conjunto_t *c = malloc(sizeof *c);
    if (!c)
        return NULL;

    c->max = max;
    c->card = 0;

    if (max)
    {
        c->v = malloc(max * sizeof(int));
        if (!c->v)
            return NULL;
    }
    else
    {
        c->v = NULL;
    }

    return c;
}

conjunto_t *destroi_cjt(conjunto_t *c)
{
    if (!c)
        return NULL;

    if (c->v)
        free(c->v);

    free(c);

    return NULL;
}

int vazio_cjt(conjunto_t *c)
{
    if (!c)
        return 0;

    return !c->card;
}

int cardinalidade_cjt(conjunto_t *c)
{
    if (!c)
        return 0;

    return c->card;
}

int insere_cjt(conjunto_t *c, int elemento)
{
    int i;
    if (!c || c->card == c->max)
        return 0;

    for (i = 0; i < c->card; i++)
        if (elemento == c->v[i])
            return 1;

    c->v[c->card++] = elemento;

    return 1;
}

static void remove_elemento(conjunto_t *c, int ind)
{
    int i;
    for (i = ind; i < c->card - 1; i++)
        c->v[i] = c->v[i + 1];

    c->card--;
}

int retira_cjt(conjunto_t *c, int elemento)
{
    int i;
    if (!c)
        return 0;

    for (i = 0; i < c->card; i++)
        if (elemento == c->v[i])
        {
            remove_elemento(c, i);
            return 1;
        }

    return 0;
}

int pertence_cjt(conjunto_t *c, int elemento)
{
    int i;
    if (!c)
        return 0;

    for (i = 0; i < c->card; i++)
        if (elemento == c->v[i])
            return 1;

    return 0;
}

int contido_cjt(conjunto_t *c1, conjunto_t *c2)
{
    int i;
    if (!c1 || !c2)
        return 0;

    for (i = 0; i < c1->card; i++)
        if (!pertence_cjt(c2, c1->v[i]))
            return 0;

    return 1;
}

int sao_iguais_cjt(conjunto_t *c1, conjunto_t *c2)
{
    if (!c1 || !c2 || c1->card != c2->card)
        return 0;

    return contido_cjt(c1, c2);
}

conjunto_t *diferenca_cjt(conjunto_t *c1, conjunto_t *c2)
{
    int i;
    if (!c1 || !c2)
        return NULL;

    conjunto_t *cjt = cria_cjt(c1->max);
    if (!cjt)
        return NULL;

    for (i = 0; i < c1->card; i++)
        if (!pertence_cjt(c2, c1->v[i]))
            cjt->v[cjt->card++] = c1->v[i];

    return cjt;
}

conjunto_t *interseccao_cjt(conjunto_t *c1, conjunto_t *c2)
{
    int i;
    if (!c1 || !c2)
        return NULL;

    conjunto_t *cjt = cria_cjt(c1->max);
    if (!cjt)
        return NULL;

    for (i = 0; i < c1->card; i++)
        if (pertence_cjt(c2, c1->v[i]))
        {
            cjt->v[cjt->card++] = c1->v[i];
        }

    return cjt;
}

conjunto_t *uniao_cjt(conjunto_t *c1, conjunto_t *c2)
{
    int i;
    if (!c1 || !c2)
        return NULL;

    conjunto_t *cjt, *dif;

    cjt = cria_cjt(c1->max + c2->max);
    if (!cjt)
        return NULL;

    dif = diferenca_cjt(c2, c1);

    for (i = 0; i < c1->card; i++)
        cjt->v[i] = c1->v[i];

    cjt->card = c1->card;

    for (i = 0; i < dif->card; i++)
        cjt->v[cjt->card + i] = dif->v[i];

    cjt->card += dif->card;

    return cjt;
}

conjunto_t *copia_cjt(conjunto_t *c)
{
    int i;
    if (!c)
        return NULL;

    conjunto_t *cjt = cria_cjt(c->max);
    if (!cjt)
        return NULL;

    for (i = 0; i < c->card; i++)
        cjt->v[i] = c->v[i];

    cjt->card = c->card;
    cjt->ptr = c->ptr;

    return cjt;
}

conjunto_t *cria_subcjt_cjt(conjunto_t *c, int n)
{
    conjunto_t *cjt;
    int i, elemento;

    if (!c)
        return NULL;

    if (n >= c->card)
        return copia_cjt(c);

    cjt = cria_cjt(n);

    if (!cjt)
        return NULL;

    for (i = 0; i < n; i++)
    {
        elemento = rand() % 9;
        if (i > 0)
        {
            if (elemento == busca_binaria(cjt->v, 0, n, elemento))
                elemento = rand() % 9;
            else
                cjt->v[i] = c->v[elemento];
        }
        else
            cjt->v[i] = c->v[elemento];
    }
    cjt->card = n;

    return cjt;
}

static void ordena_cjt(conjunto_t *c)
{
    int min, temp, i, j;

    for (i = 0; i < c->card - 1; i++)
    {
        min = i;

        for (j = i + 1; j < c->card; j++)
            if (c->v[min] > c->v[j])
                min = j;

        temp = c->v[i];
        c->v[i] = c->v[min];
        c->v[min] = temp;
    }
}

void imprime_cjt(conjunto_t *c)
{
    int i;
    if (!c->card)
        printf("Conjunto vazio.\n");

    ordena_cjt(c);

    for (i = 0; i < c->card; i++)
        printf("%d%c", c->v[i], i == c->card - 1 ? '\n' : ' ');
}

void inicia_iterador_cjt(conjunto_t *c)
{
    c->ptr = 0;
}

int incrementa_iterador_cjt(conjunto_t *c, int *ret_iterador)
{
    *ret_iterador = c->v[c->ptr++];

    return c->ptr < c->max;
}

int retira_um_elemento_cjt(conjunto_t *c)
{
    int ind = rand() % c->card,
        elemento = c->v[ind];

    retira_cjt(c, elemento);

    return elemento;
}