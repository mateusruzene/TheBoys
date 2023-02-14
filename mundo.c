#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "libfila.h"
#include "liblef.h"
#include "libconjunto.h"

#define TEMPO_INICIAL 0
#define FIM_DO_MUNDO 34944
#define N_TAMANHO_MUNDO 20000

#define N_HABILIDADES 10
#define N_MISSOES 100
#define N_HEROIS 50
#define N_LOCAIS 8

#define ID_CHEGADA 1
#define ID_SAIDA 2
#define ID_MISSAO 3
#define ID_FIM 4
#define ID_INICIO 0

struct heroi
{
  int id;
  int paciencia;
  int idade;
  int experiencia;
  conjunto_t *habilidades_heroi;
};
typedef struct heroi heroi_t;

struct local
{
  int id;
  int lotacao_max;
  conjunto_t *herois_local;
  fila_t *fila_espera;
  int x;
  int y;
};
typedef struct local local_t;

struct mundo
{
  int tempo_atual;
  int tamanho_mundo;
  heroi_t *herois[N_HEROIS];
  local_t *locais[N_LOCAIS];
  int total_herois;
  int total_locais;
  conjunto_t *habilidades_mundo;
};
typedef struct mundo mundo_t;

/* ------------------- FUNCOES AUXILIARES ------------------- */
int calcula_distancia(local_t *local, local_t *prox_local)
{
  int distancia;
  int x = (prox_local->x - local->x) * (prox_local->x - local->x);
  int y = (prox_local->y - local->y) * (prox_local->y - local->y);

  distancia = sqrt(x + y);

  return distancia;
}
int calcula_velocidade(heroi_t *heroi)
{
  int idade = heroi->idade - 40;

  if (idade > 0)
    return 100 - idade;

  return 100 - 0;
}
int aleat(int min, int max)
{
  int num;

  num = rand() % (max - min) + min;

  return num;
}

int calcula_tpl(int paciencia)
{
  int a = 1;
  int b = paciencia / (10 + aleat(-2, 6));

  if (a > b)
    return a;

  return b;
}

int calcula_tdl(local_t *local, local_t *prox_local, heroi_t *heroi)
{
  int tdl;

  int distancia = calcula_distancia(local, prox_local);
  int velocidade = calcula_velocidade(heroi);

  tdl = distancia / velocidade;

  return tdl;
}
/* ------------------- FIM FUNCOES AUXILIARES ------------------- */

/* ------------------- HEROIS ------------------- */
/* Cria heroi com dados aleatórios */
heroi_t *cria_heroi(mundo_t *mundo, int id)
{
  heroi_t *heroi;
  int hab = aleat(2, 5); /* Idade vai de 2 a 5 */

  if (!(heroi = malloc(sizeof(heroi_t))))
    return NULL;

  heroi->id = id;
  heroi->idade = aleat(18, 100);    /* Idade vai de 18 a 100 */
  heroi->paciencia = aleat(0, 100); /* Paciencia vai de 0 a 100 */
  heroi->experiencia = 0;
  heroi->habilidades_heroi = cria_subcjt_cjt(mundo->habilidades_mundo, hab);

  return heroi;
}

/* Cria uma quantidade (MAX_HEROIS) de herois para o mundo */
void cria_herois(mundo_t *mundo)
{
  int i;

  for (i = 0; i < N_HEROIS; i++)
    mundo->herois[i] = cria_heroi(mundo, i);
}
/* Adiciona experiencia aos herois que foram a missao */

/* Retorna os dados do heroi */
heroi_t *pega_heroi(mundo_t *mundo, int id_heroi)
{
  heroi_t *heroi = (mundo->herois[id_heroi]);

  return heroi;
}

void adiciona_exp_herois(mundo_t *mundo, conjunto_t *local_escolhido)
{
  heroi_t *heroi;
  int i;

  for (i = 0; i < local_escolhido->card; i++)
  {
    heroi = pega_heroi(mundo, local_escolhido->v[i]);
    heroi->experiencia++;
  }
}

heroi_t *destroi_heroi(heroi_t *heroi)
{
  heroi->habilidades_heroi = destroi_cjt(heroi->habilidades_heroi);
  free(heroi);

  return heroi;
}
/* ------------------- FIM HEROIS ------------------- */

/* ------------------- LOCAIS ------------------- */
/* Cria heroi com dados aleatórios */
local_t *cria_local(int id)
{
  local_t *local;

  if (!(local = malloc(sizeof(local_t))))
    return local;

  local->id = id;
  local->lotacao_max = aleat(5, 30); /* vai de 5 a 30 */
  local->herois_local = cria_cjt(local->lotacao_max);
  local->fila_espera = cria_fila();
  local->x = aleat(0, N_TAMANHO_MUNDO - 1);
  local->y = aleat(0, N_TAMANHO_MUNDO - 1);

  return local;
}

/* Cria uma quantidade (MAX_LOCAIS) de locais para o mundo */
void cria_locais(mundo_t *mundo)
{
  int i;

  for (i = 0; i < N_LOCAIS; i++)
    mundo->locais[i] = cria_local(i);
}
/* Retorna os dados do heroi */
local_t *pega_local(mundo_t *mundo, int id_local)
{
  local_t *local = (mundo->locais[id_local]);

  return local;
}

conjunto_t *pega_habilidades_local(mundo_t *mundo, local_t *local)
{
  heroi_t *heroi;
  conjunto_t *hab_herois = cria_cjt(N_HABILIDADES), *aux;
  int i;

  for (i = 0; i < local->herois_local->card; i++)
  {
    heroi = pega_heroi(mundo, local->herois_local->v[i]);
    aux = hab_herois;
    hab_herois = uniao_cjt(hab_herois, heroi->habilidades_heroi);
    aux = destroi_cjt(aux);
  }

  return hab_herois;
}

local_t *destroi_local(local_t *local)
{
  destroi_fila(local->fila_espera);
  destroi_cjt(local->herois_local);

  free(local);
  local = NULL;

  return local;
}
/* ------------------- FIM LOCAIS ------------------- */

/* ------------------- MUNDO ------------------- */
mundo_t *cria_mundo()
{
  int i;
  mundo_t *mundo;

  if (!(mundo = malloc(sizeof(mundo_t))))
    return NULL;

  mundo->tempo_atual = TEMPO_INICIAL;
  mundo->tamanho_mundo = N_TAMANHO_MUNDO;
  mundo->total_herois = N_HEROIS;
  mundo->total_locais = N_LOCAIS;
  mundo->habilidades_mundo = cria_cjt(N_HABILIDADES);

  for (i = 0; i <= N_HABILIDADES - 1; i++)
    insere_cjt(mundo->habilidades_mundo, i);

  return mundo;
}

mundo_t *destroi_mundo(mundo_t *mundo)
{
  int i, j;

  for (i = 0; i < N_HEROIS; i++)
    destroi_heroi(mundo->herois[i]);
  for (j = 0; j < N_LOCAIS; j++)
    destroi_local(mundo->locais[j]);

  mundo->habilidades_mundo = destroi_cjt(mundo->habilidades_mundo);
  free(mundo);
  mundo = NULL;

  return mundo;
}
/* ------------------- FIM MUNDO ------------------- */

/* ------------------- EVENTOS ------------------- */
/* Cria eventos de chegada, saida, missão no inicio da fila*/
void cria_eventos_no_inicio(lef_t *lef_mundo, int tipo, int dado1, int dado2, int tempo)
{
  evento_t evento;

  evento.tipo = tipo;
  evento.tempo = tempo;
  evento.dado1 = dado1; /* sempre com o id_heroi */
  evento.dado2 = dado2; /* id_local ou id_missão */
  adiciona_inicio_lef(lef_mundo, &evento);
}
/* Cria eventos de chegada, saida, missão e fim*/
void cria_eventos(lef_t *lef_mundo, int tipo, int dado1, int dado2, int tempo)
{
  evento_t evento;

  evento.tipo = tipo;
  evento.tempo = tempo;
  evento.dado1 = dado1; /* sempre com o id_heroi */
  evento.dado2 = dado2; /* id_local ou id_missão */
  adiciona_ordem_lef(lef_mundo, &evento);
}
/* Cria os primeiros eventos de chegada dos herois */
void cria_eventos_iniciais_chegada(lef_t *lef_mundo)
{
  int i, locais, tempo;

  for (i = 0; i <= N_HEROIS - 1; i++)
  {
    tempo = aleat(0, 97 * 7);
    locais = aleat(0, N_LOCAIS);
    cria_eventos(lef_mundo, ID_CHEGADA, i, locais, tempo);
  }
}
/* Cria os primeiros eventos de missao */
void cria_eventos_iniciais_missao(lef_t *lef_mundo)
{
  int i, tempo;

  for (i = 0; i < N_MISSOES; i++)
  {
    tempo = aleat(0, FIM_DO_MUNDO);
    cria_eventos(lef_mundo, ID_MISSAO, -1, i, tempo);
  }
}
/* ------------------- FIM EVENTOS ------------------- */

/* ------------------- FUNCOES DISPARA ------------------- */
/* dispara os eventos de chegada e cria os de saida */
void dispara_chegada(mundo_t *mundo, evento_t *evento, lef_t *lef_mundo)
{
  heroi_t *heroi = pega_heroi(mundo, evento->dado1);
  local_t *local = pega_local(mundo, evento->dado2);

  int tpl;
  int decisao = (heroi->paciencia) / 4 - tamanho_fila(local->fila_espera);
  int lotacao = cardinalidade_cjt(local->herois_local);

  if (lotacao == local->lotacao_max)
  {
    if (decisao > 0)
    {
      insere_fila(local->fila_espera, heroi->id);

      printf("%6d:CHEGADA HEROI %2d LOCAL %d (%2d/%2d), FILA %2d\n", (evento->tempo), (evento->dado1), (evento->dado2), cardinalidade_cjt(local->herois_local), local->lotacao_max, tamanho_fila(local->fila_espera));

      return;
    }
    else
    {
      cria_eventos(lef_mundo, ID_SAIDA, evento->dado1, evento->dado2, mundo->tempo_atual);

      printf("%6d:CHEGADA HEROI %2d LOCAL %d (%2d/%2d), DESISTE \n", (evento->tempo), (evento->dado1), (evento->dado2), cardinalidade_cjt(local->herois_local), (local->lotacao_max));

      return;
    }
  }
  else
  {
    tpl = calcula_tpl(heroi->paciencia);
    insere_cjt(local->herois_local, evento->dado1);

    printf("%6d:CHEGADA HEROI %2d LOCAL %d (%2d/%2d), ENTRA\n", (evento->tempo), (evento->dado1), (evento->dado2), cardinalidade_cjt(local->herois_local), (local->lotacao_max));

    cria_eventos(lef_mundo, ID_SAIDA, evento->dado1, evento->dado2, (mundo->tempo_atual + tpl));

    return;
  }
}
/* Dispara os eventos de saida e cria novos eventos de chegada*/
void dispara_saida(mundo_t *mundo, evento_t *evento, lef_t *lef_mundo)
{
  local_t *prox_local;
  heroi_t *heroi = pega_heroi(mundo, evento->dado1);
  local_t *local = pega_local(mundo, evento->dado2);
  int fila_primeiro_heroi;
  int tdl, id_prox_local, novo_tempo;

  id_prox_local = aleat(0, N_LOCAIS);
  prox_local = pega_local(mundo, id_prox_local);
  tdl = calcula_tdl(local, prox_local, heroi);
  novo_tempo = mundo->tempo_atual + (tdl / 15);

  if (!(vazia_fila(local->fila_espera)))
  {
    retira_fila(local->fila_espera, &fila_primeiro_heroi);
    retira_cjt(local->herois_local, evento->dado1);

    printf("%6d:SAIDA   HEROI %2d LOCAL %d (%2d/%2d), REMOVE FILA HEROI %2d\n", (evento->tempo), (evento->dado1), (evento->dado2), cardinalidade_cjt(local->herois_local), (local->lotacao_max), fila_primeiro_heroi);

    cria_eventos_no_inicio(lef_mundo, ID_CHEGADA, fila_primeiro_heroi, evento->dado2, mundo->tempo_atual);
    cria_eventos(lef_mundo, ID_CHEGADA, heroi->id, id_prox_local, novo_tempo);
  }
  else
  {
    retira_cjt(local->herois_local, evento->dado1);

    printf("%6d:SAIDA   HEROI %2d LOCAL %d (%2d/%2d) \n", (evento->tempo), (evento->dado1), (evento->dado2), cardinalidade_cjt(local->herois_local), (local->lotacao_max));

    cria_eventos(lef_mundo, ID_CHEGADA, evento->dado1, id_prox_local, novo_tempo);
  }
}

void dispara_missao(mundo_t *mundo, evento_t *evento, lef_t *lef_mundo)
{
  local_t *local;
  conjunto_t *missao, *hab_herois, *local_escolhido, *local_anterior;
  int i, hab_missao, id_local_escolhido;

  local_escolhido = cria_cjt(N_HABILIDADES);

  hab_missao = aleat(3, 6);
  missao = cria_subcjt_cjt(mundo->habilidades_mundo, hab_missao);

  printf("%6d:MISSAO %2d HAB_REQ ", evento->tempo, evento->dado2);
  imprime_cjt(missao);

  for (i = 0; i < N_LOCAIS; i++)
  {
    local = mundo->locais[i];

    printf("%6d:MISSAO %2d HAB_EQL %d:", evento->tempo, evento->dado2, i);
    hab_herois = pega_habilidades_local(mundo, local);
    imprime_cjt(hab_herois);

    if (contido_cjt(missao, hab_herois))
      if (vazio_cjt(local_escolhido) || cardinalidade_cjt(mundo->locais[i]->herois_local) < cardinalidade_cjt(local_escolhido))
      {
        local_anterior = local_escolhido;
        local_escolhido = copia_cjt(mundo->locais[i]->herois_local);
        id_local_escolhido = i;
        local_anterior = destroi_cjt(local_anterior);
      }

    hab_herois = destroi_cjt(hab_herois);
  }

  if (vazio_cjt(local_escolhido))
  {
    int novo_tempo = aleat(evento->tempo, FIM_DO_MUNDO);

    printf("%6d:MISSAO %2d IMPOSSIVEL \n", evento->tempo, evento->dado2);
    cria_eventos(lef_mundo, ID_MISSAO, -1, evento->dado2, novo_tempo);
  }
  else
  {
    printf("%6d:MISSAO %2d HAB_EQS %d: ", evento->tempo, evento->dado2, id_local_escolhido);
    imprime_cjt(local_escolhido);
    adiciona_exp_herois(mundo, local_escolhido);
  }

  hab_herois = destroi_cjt(hab_herois);
  local_escolhido = destroi_cjt(local_escolhido);
  missao = destroi_cjt(missao);
}

/* Dispara os eventos do mundo */
void dispara_eventos(mundo_t *mundo, lef_t *lef_mundo, evento_t *evento)
{
  switch (evento->tipo)
  {
  case ID_CHEGADA:
    dispara_chegada(mundo, evento, lef_mundo);
    break;
  case ID_SAIDA:
    dispara_saida(mundo, evento, lef_mundo);
    break;
  case ID_MISSAO:
    dispara_missao(mundo, evento, lef_mundo);
    break;
  case ID_FIM:
    printf("%6d:FIM DO MUNDO\n", (evento->tempo));
    break;
  case ID_INICIO:
    printf(" %6d:INICIO MUNDO \n", (evento->tempo));
    break;
  default:
    printf("ERRO \n");
    break;
  }
}
/* ------------------- FIM FUNCOES DISPARA ------------------- */

int main()
{
  mundo_t *mundo = cria_mundo(); /* Todos os dados do mundo */
  lef_t *lef_mundo = cria_lef(); /* Lista de eventos mundo */
  evento_t *evento;

  /* srand(time(NULL)); */ /* Semente que sempre vai aleatorizar as simulações, fazendo que nenhuma seja igual a outra */
  srand(121324);

  cria_herois(mundo);
  cria_locais(mundo);

  cria_eventos(lef_mundo, ID_INICIO, -1, -1, 0); /* EVENTO DE INICIO DO MUNDO */
  cria_eventos_iniciais_chegada(lef_mundo);
  cria_eventos_iniciais_missao(lef_mundo);
  cria_eventos(lef_mundo, ID_FIM, -1, -1, FIM_DO_MUNDO); /* EVENTO DE FIM
   DO MUNDO */

  /* Dispara os eventos do mundo */
  while (mundo->tempo_atual < FIM_DO_MUNDO)
  {
    evento = obtem_primeiro_lef(lef_mundo);
    mundo->tempo_atual = evento->tempo;
    dispara_eventos(mundo, lef_mundo, evento);
    free(evento);
  }

  /* IMPRIME AS EXPERIENCIAS DOS HEROIS NO FINAL */
  int i;
  for (i = 0; i <= mundo->total_herois - 1; i++)
  {
    printf("HEROI %2d EXPERIENCIA %2d \n", (mundo->herois[i])->id, (mundo->herois[i])->experiencia);
  }

  lef_mundo = destroi_lef(lef_mundo);
  mundo = destroi_mundo(mundo);

  return 0;
}