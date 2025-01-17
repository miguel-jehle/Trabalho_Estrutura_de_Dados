#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_T 50

typedef struct jogador{
    int id;
    int num_camisa;
    char posicao[5];
    char nome[31];
    int dia;
    char mes[11];
    int ano;
    int idade;
    int part_sel;
    int gol_sel;
    char pais_time[21];
    char time[31];
    int capitao; // 1 ou 0 
    char sele[31];
}TJ;

typedef struct arvbm{
    int folha; //4 0-3
    int nchaves; //4 4-7
    char prox[30]; //sizeof(char)*5 8-12
    TJ chaves[(2*MAX_T)-1]; // 
    char filhos[2*MAX_T][30]; //arq de 0000 até 9999
}TABM;

char * TABM_busca(char* arq, int id);
void TABM_libera_no(TABM * no);
void imp(char* raiz,int andar, int t);
void TABM_imprime(char** raiz,int t);
void copia(TABM * T, TABM * aux, int t);
TJ copia_chaves (TJ aux, TJ T);
TABM * TABM_cria_no(int t);
TABM * divisao(TABM *S, int i, TABM* T, int t, int * cont);
TABM * insere_nao_completo(TABM *S, TJ * jogador, int t, int * cont);
char * TABM_insere(TJ *jogador, int t, char ** raiz, int * cont);
char * TABM_remover(char* no, int ch, int t);
void TABM_cria(int t, int *cont, char ** arq);
void printa_arqb(char* entrada, int t);
void le_dados(char * arquivo, char ** raiz, int t, int* cont);