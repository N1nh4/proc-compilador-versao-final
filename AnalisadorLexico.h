#ifndef ANALISADORLEXICO_H
#define ANALISADORLEXICO_H

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define TAM_MAX_LEXEMA 31


enum TOKEN_CATEGORIA 
{
    ID = 1,            // Identificador 
    CT_I,              // Constante númerica Inteira
    CT_R,              // Constante númerica Real
    CT_C,              // Constante Caractere
    LT,                // Literal cadeia de caracter
    FIM_ARQ,           // Fim do arquivo
    FIM_EXPR,          // Fim da expressao
    SN,                // Sinal
    PV_R,              // Palavra Reservada
    ID_CONST,
    ID_PROC
    
};

enum PALAVRA_RESERVADA 
{
    CONST = 1,         // 1
    GETSTR,            // 2
    INIT,              // 3
    ENDP,              // 4
    CHAR,              // 5
    INT,               // 6
    REAL,              // 7
    BOOL,              // 8
    DO,                // 9
    WHILE,             // 10
    ENDW,              // 11
    VAR,               // 12
    FROM,              // 13
    TO,                // 14
    DT,                // 15
    BY,                // 16
    IF,                // 17
    ENDV,              // 18
    ELIF,              // 19
    ELSE,              // 20
    ENDI,              // 21
    GETOUT,            // 22
    GETINT,            // 23
    GETCHAR,           // 24
    GETREAL,           // 25
    PUTINT,            // 26
    PUTCHAR,           // 27
    PUTREAL,           // 28
    PROT,              // 29
    DEF,               // 30
    PUTSTR             // 31
};

enum SINAIS
{     
    PAR_ESQUERDO = 1,  // (
    PAR_DIREITO,       // )
    COL_ESQUERDO,      // [
    COL_DIREITO,       // ]
    VIRGULA,           // ,
    ENDERECO,          // &
    ATRIBUICAO,        // =
    ADICAO,            // +
    SUBTRACAO,         // -
    MULTIPLICACAO,     // *
    DIVISAO,           // /
    IGUALDADE,         // ==
    DIFERENTE,         // !=
    MENOR_IGUAL,       // <=
    MAIOR_IGUAL,       // >=
    MENOR_QUE,         // <
    MAIOR_QUE,         // >
    AND,               // &&
    OR,                // ||
    NOT,               // !
    CHA_ESQUERDA,      // {
    CHA_DIREITA        // }
};


typedef 
    struct {
        enum TOKEN_CATEGORIA cat; // deve receber uma das constantes de enum TOKEN_CATEGORIA
        union {                
            int codigo;                  
            int indice;              
            char lexema[TAM_MAX_LEXEMA]; 
            int valorInt;                 
            float valorReal;            
            char c; 
                         
    };
} TOKEN; // Tipo TOKEN


TOKEN AnalisadorLexico(FILE *fd);

extern TOKEN tk;
extern FILE *fd;
extern int contLinha; 
extern char TABELA_PR[31][20];

#endif

