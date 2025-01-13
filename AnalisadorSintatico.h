#ifndef ANALISADORSINTATICO_H
#define ANALISADORSINTATICO_H

#include <stdio.h>
#include <stdbool.h>
#include "AnalisadorLexico.h"
#include "TabelaSimbolos.h"


/* Variaveis globais */
extern TOKEN tk;
extern REG_TS tb;
extern FILE *fd;
extern int contLinha;
extern int escopoAtual;

extern bool mostraArvore;
extern char TABS[200];

/* Assinaturas das funcoes */
void Prog();
void DeclListVar();
void DeclDefProc();
void DeclVar();
void Tipo();
void Cmd();
void Atrib();
void Expr();
void ExprSimp();
void Termo();
void Fator();
void OpRel();

#endif // ANASINT



