#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "AnalisadorLexico.h"
#include "AnalisadorSintatico.h"
#include "Funcaux.h"
//#include "TabelaSimbolos.h"

FILE *fd;
TOKEN tk;

int contLinha = 1;
char TABELA_PR[31][20];
bool mostraArvore;

char TABS[200] = "";

void TestaAnalisadorLexico() {
    if ((fd=fopen("entrada.txt", "r")) == NULL) {
        error("Arquivo de entrada da expressao nao encontrado!");
    }

    printf("LINHA %d: ", contLinha);

    while (1) { // laço infinito para processar a expressão até o fim de arquivo 
        tk = AnalisadorLexico(fd);
        switch (tk.cat) {

            case SN: switch (tk.codigo) {

                case PAR_ESQUERDO:
                    printf("<SN, PARENTESE_ESQUERDO ");
                    break;
                
                case PAR_DIREITO:
                    printf("<SN, PARENTESE_DIREITO ");
                    break;
                    
                case COL_ESQUERDO: 
                    printf("<SN, COLCHETE_ESQUERDO> ");
                    break;

                case COL_DIREITO: 
                    printf("<SN, COLCHETE_DIREITO> ");
                    break;

                case VIRGULA: 
                    printf("<SN, VIRGULA> ");
                    break;

                case ENDERECO: 
                    printf("<SN, ENDERECO> ");
                    break;

                case ATRIBUICAO: 
                    printf("<SN, ATRIBUICAO> ");
                    break;

                case ADICAO: 
                    printf("<SN, ADICAO> ");
                    break;

                case SUBTRACAO: 
                    printf("<SN, SUBTRACAO> ");
                    break;

                case MULTIPLICACAO: 
                    printf("<SN, MULTIPLICACAO> ");
                    break;

                case CHA_ESQUERDA: 
                    printf("<SN, CHAVE_ESQUERDA> ");
                    break;                    

                case CHA_DIREITA: 
                    printf("<SN, CHAVE_DIREITA> ");
                    break;

                case DIVISAO: 
                    printf("<SN, DIVISAO> ");
                    break;

                case IGUALDADE: 
                    printf("<SN, IGUALDADE> ");
                    break;

                case DIFERENTE:
                    printf("<SN, DIFERENTE> ");
                    break;

                case MENOR_IGUAL: 
                    printf("<SN, MENOR_IGUAL> ");
                    break;

                case MAIOR_IGUAL: 
                    printf("<SN, MAIOR_IGUAL> ");
                    break;

                case MENOR_QUE: 
                    printf("<SN, MENOR_QUE> ");
                    break;

                case MAIOR_QUE: 
                    printf("<SN, MAIOR_QUE> ");
                    break;

                case AND: 
                    printf("<SN, AND> ");
                    break;

                case OR: 
                    printf("<SN, OR> ");
                    break;

                case NOT:
                    printf("<SN, NOT> ");
                    break;
            }
            break;
            case PV_R: switch (tk.codigo){
                    case CONST:
                        printf("<PR, CONST> ");
                        break;
                    
                    case GETSTR:
                        printf("<PR, GETSTR> ");
                        break;

                    case INIT:
                        printf("<PR, INIT> ");
                        break;

                    case ENDP:
                        printf("<PR, ENDP> ");
                        break;

                    case CHAR:
                        printf("<PR, CHAR> ");
                        break;

                    case INT:
                        printf("<PR, INT> ");
                        break;

                    case REAL:
                        printf("<PR, REAL> ");
                        break;

                    case BOOL:
                        printf("<PR, BOOL> ");
                        break;

                    case DO:
                        printf("<PR, DO> ");
                        break;

                    case WHILE:
                        printf("<PR, WHILE> ");
                        break;

                    case ENDW:
                        printf("<PR, ENDW>");
                        break;

                    case VAR:
                        printf("<PR, VAR> ");
                        break;

                    case FROM:
                        printf("<PR, FROM> ");
                        break;

                    case TO:
                        printf("<PR, TO> ");
                        break;

                    case DT:
                        printf("<PR, DT> ");
                        break;

                    case BY:
                        printf("<PR, BY> ");
                        break;

                    case IF:
                        printf("<PR, IF> ");
                        break;

                    case ENDV:
                        printf("<PR, ENDV> ");
                        break;

                    case ELIF:
                        printf("<PR, ELIF> ");
                        break;

                    case ELSE:
                        printf("<PR, ELSE> ");
                        break;

                    case ENDI:
                        printf("<PR, ENDI> ");
                        break;

                    case GETOUT:
                        printf("<PR, GETOUT ");
                        break;

                    case GETINT:
                        printf("<PR, GETINT> ");
                        break;

                    case GETCHAR:
                        printf("<PR, GETCHAR> ");
                        break;

                    case GETREAL:
                        printf("<PR, GETREAL> ");
                        break;

                    case PUTINT:
                        printf("<PR, PUTINT> ");
                        break;

                    case PUTCHAR:
                        printf("<PR, PUTCHAR> ");
                        break;

                    case PUTREAL:
                        printf("<PR, PUTREAL> ");
                        break;

                    case PROT:
                        printf("<PR, PROT> ");
                        break;

                    case DEF:
                        printf("<PR, DEF> ");
                        break;
                    
                    case PUTSTR:
                        printf("<PR, PUTSTR> ");
                        break;
                        
                }
                break; 

            case ID: 
                printf("<ID, %s> ", tk.lexema);
                break;
            case CT_I:
                printf("<CT_I, %d> ", tk.valorInt);
                break;
            case CT_R:
                printf("<CT_R, %f> ", tk.valorReal);
                break;
            case CT_C:
                printf("<CT_C, %c> ", tk.c);
                break;
            case LT:
                printf("<LT, %s> ", tk.lexema);
                break;
            case ID_CONST:
                printf("<ID_CONST, %s> ", tk.lexema);
                break;
            case ID_PROC:
                printf("<ID_PROC, %s> ", tk.lexema);
                break;
            case FIM_ARQ: 
                printf(" <Fim do arquivo encontrado>\n");
                break;
            case FIM_EXPR:
                printf("<FIM_EXPR, %d>\n", 0);
                printf("LINHA %d: ", contLinha);
                break;
        }
        if (tk.cat == FIM_ARQ) break;
    }
    
    fclose(fd);
}

void TestaAnalisadorSintatico() {
    if ((fd=fopen("entrada.txt", "r")) == NULL) {
        error("Arquivo de entrada da expressao nao encontrado!");
    }

    

    while (1) { 
        tk = AnalisadorLexico(fd);
        if (tk.cat == FIM_ARQ) {
            printf("\nExpressão sintaticamente correta! \nFim do arquivo fonte encontrado!\n");
            
            break;
        }
        Prog();
        if (tk.cat==FIM_EXPR) {
            printf("\nLINHA %d: Expressão sintaticamente correta!\n\n", contLinha - 1);
            ConsomeFimExp();
        }
        else if (tk.cat != FIM_ARQ){
            error("Erro de sintaxe!");
        }
    }
    fclose(fd);
}


int main() {
    
    system("chcp 65001");  // Troca a página de códigos de caracteres 
                           // da console para UTF-8 na execução

    printf("\n\n[Análise Léxica -------------------]\n");
    TestaAnalisadorLexico();

    mostraArvore = false;
    
    printf("\n\n[Análise Sintática ----------------]\n");
    TestaAnalisadorSintatico();
}
