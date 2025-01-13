#include "TabelaSimbolos.h"
#include <string.h>

/* Inicializa a tabela de símbolos */
void Iniciar_tabela() {
    tabela_idef.tamTab = -1; // A tabela começa vazia
}

int indRotulo = 1;



const char* MapearTipo(enum TIPO tipo) {
    switch (tipo) {
        case NUM_INT: return "INT";
        case NUM_REAL: return "REAL";
        case CARACTER_CHAR: return "CHAR";
        case BOOLEANO: return "BOOL";
        case NA_TIPO: return "N/A";
        default: return "DESC";
    }
}

const char* MapearEscopo(enum ESCOPO escopo) {
    switch (escopo) {
        case GLOBAL: return "GLOBAL";
        case LOCAL: return "LOCAL";
        default: return "DESC";
    }
}

const char* MapearCategoria(enum CATEGORIA categoria) {
    switch (categoria) {
        case VAR_GLOBAL: return "VAR_GLOBAL";
        case VAR_LOCAL: return "VAR_LOCAL";
        case PROCEDIMENTO: return "PROCEDIMENTO";
        case PARAMETRO_PROCEDIMENTO: return "PARAMETRO";
        case PROTOTIPO_PROCEDIMENTO: return "PROTOTIPO";
        default: return "DESC";
    }
}

const char* MapearPassagem(enum PASSAGEM passagem) {
    switch (passagem) {
        case POR_COPIA_VALOR: return "COPIA";
        case POR_REFERENCIA_ENDERECO: return "REFER";
        case NA_PASSAGEM: return "N/A";
        default: return "DESC";
    }
}

const char* MapearZumbi(enum ZUMBI zumbi) {
    switch (zumbi) {
        case VIVO: return "VIVO";
        case ZUMBI: return "ZUMBI";
        case NA: return "N/A";
        default: return "DESC";
    }
}

const char* MapearArray(enum ARRAY array) {
    switch (array) {
        case VAR_SIMPLES: return "NAO";
        case ID_ARRAY_VETOR: return "VETOR";
        case ID_ARRAY_MATRIZ: return "MATRIZ";
        default: return "DESC";
    }
}

const char* MapearConstStatus(enum CONST_STATUS status) {
    return status == EH_CONST ? "SIM" : "NAO";
}



/* Imprime o conteúdo da tabela de símbolos */
void Imprimir_tabela() {
    //printf("Tabela de Símbolos:\n");
    printf("------------------------------------------------------------------------------------------------------------------------------------------------\n");
    printf("| Lexema       | Escopo    | Tipo    | Categoria      | Endereço | Const | Valor Const   | Array    | Dim1 | Dim2 | Zumbi  | Passagem | Rotulo |\n");
    printf("------------------------------------------------------------------------------------------------------------------------------------------------\n");
    for (int i = 0; i <= tabela_idef.tamTab; i++) {
         // Buffer para armazenar o valor constante como string.
        char valor_const_str[20];
        // Determina como imprimir o valor constante com base no tipo.
        if (tabela_idef.tabela_simb[i].eh_const == EH_CONST) {
            if (tabela_idef.tabela_simb[i].array == ID_ARRAY_VETOR) {
                // Se for vetor, exibe [dim1]
                sprintf(valor_const_str, "N/A");
            } else if (tabela_idef.tabela_simb[i].array == ID_ARRAY_MATRIZ) {
                // Se for matriz, exibe [dim1][dim2]
                sprintf(valor_const_str, "N/A");
            } else {
                switch (tabela_idef.tabela_simb[i].tipo) {
                    case NUM_INT:
                        sprintf(valor_const_str, "%d", tabela_idef.tabela_simb[i].valor_const.valor_int);
                        break;
                    case NUM_REAL:
                        sprintf(valor_const_str, "%.2f", tabela_idef.tabela_simb[i].valor_const.valor_real);
                        break;
                    case CARACTER_CHAR:
                        sprintf(valor_const_str, "'%c'", tabela_idef.tabela_simb[i].valor_const.valor_char);
                        break;
                    case BOOLEANO:
                        sprintf(valor_const_str, "%s", tabela_idef.tabela_simb[i].valor_const.valor_bool ? "true" : "false");
                        break;
                    default:
                        strcpy(valor_const_str, "N/A");
                        break;
                }
         }
    } else {
        strcpy(valor_const_str, "N/A");
    }



        // Imprime a linha da tabela.
        printf("| %-12s | %-9s | %-7s | %-14s | %-8d | %-5s | %-13s | %-8s | %-4d | %-4d | %-6s | %-8s | %-6d |\n",
            tabela_idef.tabela_simb[i].lexema,
            MapearEscopo(tabela_idef.tabela_simb[i].escopo),
            MapearTipo(tabela_idef.tabela_simb[i].tipo),
            MapearCategoria(tabela_idef.tabela_simb[i].categoria),
            tabela_idef.tabela_simb[i].endereco,
            MapearConstStatus(tabela_idef.tabela_simb[i].eh_const),
            valor_const_str,
            MapearArray(tabela_idef.tabela_simb[i].array),
            tabela_idef.tabela_simb[i].dim1,
            tabela_idef.tabela_simb[i].dim2,
            MapearZumbi(tabela_idef.tabela_simb[i].zumbi),
            MapearPassagem(tabela_idef.tabela_simb[i].passagem),
            tabela_idef.tabela_simb[i].rotulo);
    }
    printf("------------------------------------------------------------------------------------------------------------------------------------------------\n");

}


int Consultar_tabela(const char lexema[]) {
    for (int i = 0; i <= tabela_idef.tamTab; i++) {
        if (strcmp(tabela_idef.tabela_simb[i].lexema, lexema) == 0) {
            return i; 
        }
    }
    return -1; 
}


int Consultar_escopo(char lexema[], int escopo) {
    for (int i = 0; i <= tabela_idef.tamTab; i++) {
        if (strcmp(tabela_idef.tabela_simb[i].lexema, lexema) == 0 && tabela_idef.tabela_simb[i].escopo == escopo) {
            return i;
        }
    }
    return -1; 
}

int Inserir_tabela(char lexema[], int escopo, int tipo, int categoria, int zombie, int eh_const, union VALOR_CONST valor_const , int array, int dim1,int dim2, int valorConst, int passagem) {
    if (tabela_idef.tamTab + 1 >= MAX_TAM) {
        printf("Erro: Tabela de símbolos cheia.\n");
        return -1;
    }

    if (Consultar_escopo(lexema, escopo) != -1) {
        printf("Erro: Identificador '%s' já declarado no escopo %d.\n", lexema, escopo);
        return -1;
    }

    tabela_idef.tamTab++;
    REG_TS *novo = &tabela_idef.tabela_simb[tabela_idef.tamTab];

    strcpy(novo->lexema, lexema);
    novo->escopo = escopo;
    novo->tipo = tipo;
    novo->categoria = categoria;

    /*if (escopo == 0) {
        novo->endereco = indEnderecoGlobal++;
    } else if (escopo == 1) {
       
        novo->endereco = indEnderecoLocal++;
    }*/

    if (tb.categoria == PARAMETRO_PROCEDIMENTO) {
        novo->passagem = passagem;
    } else {
        novo->passagem = NA_PASSAGEM;
    }

    if (tb.categoria == PARAMETRO_PROCEDIMENTO) {
        novo->zumbi = VIVO; // Parâmetros são vivos
    } else {
        novo->zumbi = NA;
    }

    novo->endereco = tabela_idef.tamTab; // Endereço fictício baseado na posição
    novo->eh_const = eh_const; // Valor padrão
    

    // Inicialização do campo valor_const
    if ( eh_const == EH_CONST) {
        if (tipo == NUM_INT) {
            novo->valor_const.valor_int = valor_const.valor_int;
        } else if (tipo == NUM_REAL) {
            novo->valor_const.valor_real = valor_const.valor_real;
        } else if (tipo == CARACTER_CHAR) {
            novo->valor_const.valor_char = valor_const.valor_char;
        } else if (tipo == BOOLEANO) {
            novo->valor_const.valor_bool = valor_const.valor_bool;
        } else if (tipo == NA_TIPO) {
            novo->valor_const.valor_na = valor_const.valor_na;
        } else {
            printf("Erro: Tipo inválido para valor_const.\n");
            exit(1);
        }
    } 

    novo->array = array;
   
   

    if (tb.array == ID_ARRAY_VETOR) {
        novo->dim1 = dim1;
        novo->dim2 = 0; // Vetores não possuem segunda dimensão
    } else if (tb.array == ID_ARRAY_MATRIZ) {
        novo->dim1 = dim1;
        novo->dim2 = dim2;
    } else {
        novo->dim1 = 0; // Caso não seja array, dimensões são zero
        novo->dim2 = 0;
    }

    if (tb.categoria == PROCEDIMENTO) {
        novo->rotulo = indRotulo++;
    }

    return tabela_idef.tamTab;
}

/* Remove o último elemento da tabela */
int Remover_ultimo() {
    if (tabela_idef.tamTab < 0) {
        printf("Erro: Tabela de símbolos está vazia.\n");
        return -1;
    }
    tabela_idef.tamTab--;
    return 0; // Sucesso
}


void TrasformarZumbi(int escopoAtual) {
    for (int i = 0; i <= tabela_idef.tamTab; i++) {
        if (tabela_idef.tabela_simb[i].escopo == escopoAtual && 
            tabela_idef.tabela_simb[i].categoria == PARAMETRO_PROCEDIMENTO) {
            tabela_idef.tabela_simb[i].zumbi = ZUMBI; // Marca como zumbi
        }
    }
}


void RemoverVariaveisLocais(int escopoAtual) {
    if (tabela_idef.tamTab < 0) {
        printf("Erro: Tabela de símbolos está vazia.\n");
        return;
    }

    int i = tabela_idef.tamTab;

    // Percorre a tabela de trás para frente para remover as variáveis locais do escopo atual.
    while (i >= 0) {
        if (tabela_idef.tabela_simb[i].escopo == escopoAtual &&
            tabela_idef.tabela_simb[i].categoria == VAR_LOCAL) {
            
            // "Remove" a entrada reduzindo o tamanho da tabela.
            tabela_idef.tamTab--;
        } else {
            break; // Para quando sair do escopo local
        }
        i--;
    }
}



