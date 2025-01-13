#ifndef TABELASIMBOLOS
#define TABELASIMBOLOS

#include <stdio.h>
#include <stdlib.h>
#include "AnalisadorLexico.h"
#include <stdbool.h>

#define MAX_TAM 100
#define TAM_CATEGORIA 4

enum ESCOPO 
{
    GLOBAL,     // 0
    LOCAL,       // 1
};

enum TIPO
{
    NUM_INT,        // 0
    NUM_REAL,       // 1
    CARACTER_CHAR,  // 2
    BOOLEANO,       // 3
    NA_TIPO         // 4
};

enum CATEGORIA
{
    VAR_GLOBAL,                 // 0
    VAR_LOCAL,                  // 1
    PROCEDIMENTO,               // 2
    PARAMETRO_PROCEDIMENTO,     // 3
    PROTOTIPO_PROCEDIMENTO      // 4
};

enum PASSAGEM
{
    POR_COPIA_VALOR,            // 0
    POR_REFERENCIA_ENDERECO,    // 1
    NA_PASSAGEM                 // 2
};

enum ZUMBI
{
    VIVO,       // 0
    ZUMBI,      // 1
    NA          // 2
};

enum ARRAY
{
   VAR_SIMPLES,
   ID_ARRAY_VETOR,
   ID_ARRAY_MATRIZ 
};                                                

enum CONST_STATUS {
    EH_CONST,
    NAO_CONST
};

union VALOR_CONST {
    int valor_int;
    float valor_real;
    char valor_char;
    bool valor_bool; 
    // não aplica tipo:
    char valor_na;
    char nao_aplica[20];
};

typedef 
    struct {
        char lexema[TAM_MAX_LEXEMA];
        enum ESCOPO escopo;
        enum TIPO tipo;
        enum CATEGORIA categoria;
        enum PASSAGEM passagem;
        enum ZUMBI zumbi;
        enum ARRAY array;
        int dim1;         // Dimensão 1
        int dim2;         // Dimensão 2
        enum CONST_STATUS eh_const;    // Indica se é constante
        union VALOR_CONST valor_const;
        int endereco;
        int rotulo;       // Rótulo (caso de procedimento)
        int valorConst;
    } REG_TS;


typedef
    struct {
        int tamTab;
        REG_TS tabela_simb[MAX_TAM];
    }  TAB_SIMB;

extern TAB_SIMB tabela_idef;
extern TOKEN tk;
extern REG_TS tb;
extern int escopoAtual;

void Iniciar_tabela();
void Imprimir_tabela();
int Consultar_tabela(const char lexema[]);
int Consultar_escopo(char lexema[], int escopo);
int Inserir_tabela(char lexema[], int escopo, int tipo, int categoria, int zumbi, int eh_const, union VALOR_CONST valor_const, int array,int dim1, int dim2, int valorConst, int passagem);
int Remover_ultimo();
void TrasformarZumbi(int escopoAtual);
void RemoverVariaveisLocais(int escopoAtual);
// Funções de mapeamento




#endif