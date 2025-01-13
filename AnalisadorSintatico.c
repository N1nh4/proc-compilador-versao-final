
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "AnalisadorLexico.h"
#include "AnalisadorSintatico.h"
#include "Funcaux.h"
#include "TabelaSimbolos.h"


TAB_SIMB tabela_idef;
REG_TS tb;
int escopoAtual = 0;
int procedimento = 0;

extern bool mostraArvore;

#define AVANCA 1
#define RETROCEDE -1
#define MANTEM 0
#define TAM_NUM_SINTATICO 100

int am = 1;
char cs;
int tamDS = 0;
char numSintatico[TAM_NUM_SINTATICO] = "";


void Prog() {
    FILE *arq = fopen("saida.txt", "w");

    if (arq == NULL) {
        printf("Erro na geração de codigo.\n");
        exit(1);
    }

    fprintf(arq, "INIP\n");
    
   

    Iniciar_tabela();
    
   

    while (tk.cat == PV_R && (tk.codigo == CONST || tk.codigo == INT || tk.codigo == CHAR || tk.codigo == REAL || tk.codigo == BOOL)) {
        
        DeclListVar();
        
    }

    while (tk.cat == PV_R && (tk.codigo == PROT || tk.codigo == DEF)) {
        DeclDefProc(); 
    }

    if (tk.cat != FIM_ARQ) error("Declaração ou definição de procedimento esperada!");
    
   
    
}

void DeclListVar() {
    
    am = 1;
    FILE *arq = fopen("saida.txt", "a");

    tb.eh_const = NAO_CONST;


    if (tk.cat == PV_R && tk.codigo == CONST) {
        tk = AnalisadorLexico(fd);
        ConsomeFimExp(); 
        tb.eh_const = EH_CONST;
       
    } 

    Tipo(); 

    DeclVar(); 

   

    while (tk.cat == SN && tk.codigo == VIRGULA) {
        am++;
        tk = AnalisadorLexico(fd);
        ConsomeFimExp(); 
        DeclVar();
    }

    

    fprintf(arq, "AMEM %d\n", am);
    
 
    am = 0;
    
}


void DeclDefProc() {
    FILE *arq = fopen("saida.txt", "a");
  
    if (tk.codigo == PROT) {

        tk = AnalisadorLexico(fd);
        ConsomeFimExp(); 

        if (tk.cat == ID) {
            char lexema[TAM_MAX_LEXEMA];
            strncpy(lexema, tk.lexema, TAM_MAX_LEXEMA - 1);
            lexema[TAM_MAX_LEXEMA - 1] = '\0';

            tb.tipo = NA_TIPO;
            tb.eh_const = NAO_CONST;
            tb.categoria = PROTOTIPO_PROCEDIMENTO;

            fprintf(arq, "LABEL %s\n", lexema);
       
        

            if (Inserir_tabela(lexema, escopoAtual, tb.tipo, tb.categoria, tb.zumbi, tb.eh_const, tb.valor_const, tb.array, tb.dim1, tb.dim2, tb.valorConst, tb.passagem) == -1) {
                
        }
           
            tk = AnalisadorLexico(fd);
            ConsomeFimExp(); 
        } else {
            error("Identificador de procedimento esperado após 'prot'!");
        }

        if (tk.codigo == PAR_ESQUERDO) {
          
            tk = AnalisadorLexico(fd);
            ConsomeFimExp(); 

            if (tk.codigo == PAR_ESQUERDO) {
                error("Definição de prototipo invalida!");
            }
          

        if (tk.cat == FIM_EXPR || tk.cat == FIM_ARQ) {
           
            error("Conteúdo esperado após '('!");
        }
            if (tk.codigo != PAR_DIREITO) { 

                

                do {
             
                    if (tk.cat == SN && tk.codigo == ENDERECO) {
                       
                        tk = AnalisadorLexico(fd);
                        ConsomeFimExp(); 
                      
                    }

                    Tipo(); 


                    while (tk.codigo == COL_ESQUERDO) {
                        tk = AnalisadorLexico(fd);
                        ConsomeFimExp(); 

                        if (tk.codigo == COL_DIREITO) {
                            tk = AnalisadorLexico(fd);
                            ConsomeFimExp(); 
                        } else {
                            error("Fechamento de colchetes esperado!");
                        }
                    }


                } while (tk.codigo == VIRGULA && (tk = AnalisadorLexico(fd), true)); 
            }

            if (tk.codigo == PAR_DIREITO) {
                
                tk = AnalisadorLexico(fd);
                ConsomeFimExp(); 
            } else {
                error("Definição de prototipo invalida!");
            }
        }

    } else if (tk.codigo == DEF) {
   
      
        tk = AnalisadorLexico(fd);
        ConsomeFimExp(); 
        
        escopoAtual++;
      
        if (tk.codigo == INIT) {
            char lexema[TAM_MAX_LEXEMA];
            strncpy(lexema, "init", TAM_MAX_LEXEMA - 1);
            lexema[TAM_MAX_LEXEMA - 1] = '\0';

            tb.tipo = NA_TIPO;
            tb.eh_const = NAO_CONST;
            tb.categoria = PROCEDIMENTO;

            fprintf(arq, "LABEL %s\n", lexema);
            
            

            if (Inserir_tabela(lexema, escopoAtual, tb.tipo, tb.categoria, tb.zumbi, tb.eh_const, tb.valor_const, tb.array, tb.dim1, tb.dim2, tb.valorConst, tb.passagem) == -1) {
                
            }

            tk = AnalisadorLexico(fd);
            ConsomeFimExp(); 
            
        } else if (tk.cat == ID) { 
            char lexema[TAM_MAX_LEXEMA];
            strncpy(lexema, tk.lexema, TAM_MAX_LEXEMA - 1);
            lexema[TAM_MAX_LEXEMA - 1] = '\0';

            tb.tipo = NA_TIPO;
            tb.categoria = PROCEDIMENTO;
            tb.eh_const = NAO_CONST;

            fprintf(arq, "LABEL %s\n", lexema);
          
           

            

            if (Inserir_tabela(lexema, escopoAtual, tb.tipo, tb.categoria, tb.zumbi, tb.eh_const, tb.valor_const, tb.array, tb.dim1, tb.dim2, tb.valorConst, tb.passagem) == -1) {
                printf("[ERRO] Não foi possível inserir o identificador '%s' na tabela de símbolos.\n", lexema);
            }
            
            

            

            tk = AnalisadorLexico(fd);
            ConsomeFimExp(); 
           
            if (tk.codigo == PAR_ESQUERDO) {

                tk = AnalisadorLexico(fd);
                ConsomeFimExp(); 
               
                
                if (tk.codigo != PAR_DIREITO) { 
                do {
                 
                    if (tk.cat == SN && tk.codigo == ENDERECO) {
                        tb.passagem = POR_REFERENCIA_ENDERECO;
                        tk = AnalisadorLexico(fd);
                        ConsomeFimExp(); 
                    } else if (tk.cat == SN && tk.codigo != ENDERECO) {
                        tb.passagem = POR_COPIA_VALOR;
                    }
                 
                    if (tk.cat == PV_R) { 
                        Tipo(); 
                    } else {
                        error("Tipo esperado após '&' ou diretamente!");
                    }

            
                    if (tk.cat == ID) {

                        char lexema[TAM_MAX_LEXEMA];
                        strncpy(lexema, tk.lexema, TAM_MAX_LEXEMA - 1);
                        lexema[TAM_MAX_LEXEMA - 1] = '\0';

                        tb.eh_const = NAO_CONST;
                        tb.categoria = PARAMETRO_PROCEDIMENTO;
                      

                        if (Inserir_tabela(lexema, escopoAtual, tb.tipo, tb.categoria, tb.zumbi, tb.eh_const, tb.valor_const, tb.array, tb.dim1, tb.dim2, tb.valorConst, tb.passagem) == -1) {
                            printf("[ERRO] Não foi possível inserir o identificador '%s' na tabela de símbolos.\n", lexema);
                        }    
                      
                        tk = AnalisadorLexico(fd);
                        ConsomeFimExp(); 

                    } else {
                        error("Identificador esperado após tipo!");
                    }

                    while (tk.cat == SN && tk.codigo == COL_ESQUERDO) {
                    tb.dim1 = 0;
                    
                    tk = AnalisadorLexico(fd);
                    ConsomeFimExp(); 
                    
                    if ((tk.cat == CT_I)) {
                        tb.dim1 = tk.valorInt;
                    
                    } else if (tk.cat == ID) {
                        // VOLTAR DEPOIS
                        // SE O ID FOR CONST, OU SEJA UMA VARIAVEL CONSTANTE
                        // aqui
                        int indice = Consultar_tabela(tk.lexema);
                      
                        if (indice != -1) {
                            if (tabela_idef.tabela_simb[indice].eh_const == EH_CONST &&
                                tabela_idef.tabela_simb[indice].tipo == NUM_INT) {
                                tb.dim1 = tabela_idef.tabela_simb[indice].valor_const.valor_int;
                            } else {
                                printf("Erro: '%s' não é uma constante inteira.\n", tk.lexema);
                                exit(1);
                            }
                        } else {
                            printf("Erro: Identificador '%s' não encontrado.\n", tk.lexema);
                            exit(1);
                        }
                        // até aqui
                    } else {
                        error("Constante inteira ou identificador esperado!");
                    }

                    tk = AnalisadorLexico(fd);
                    ConsomeFimExp(); // Consome '}'

                    if (tk.cat == SN && tk.codigo == COL_DIREITO) {
                        tk = AnalisadorLexico(fd);
                        ConsomeFimExp(); // Consumir "]"

                        if (tk.cat == SN && tk.codigo == COL_ESQUERDO) {
                            tk = AnalisadorLexico(fd);
                            ConsomeFimExp(); // Consome '}'
                            tb.array = ID_ARRAY_MATRIZ;

                            // int a[2][
                            if ((tk.cat == CT_I)) {
                                tb.dim2 = tk.valorInt;
                            } else if (tk.cat == ID) {
                                // voltar depois
                                // SE O ID FOR CONST, OU SEJA UMA VARIAVEL CONSTANTE
                                int indice = Consultar_tabela(tk.lexema);
                                if (indice != -1) {
                                    if (tabela_idef.tabela_simb[indice].eh_const == EH_CONST &&
                                        tabela_idef.tabela_simb[indice].tipo == NUM_INT) {
                                        tb.dim2 = tabela_idef.tabela_simb[indice].valor_const.valor_int;
                                    } else {
                                        printf("Erro: '%s' não é uma constante inteira.\n", tk.lexema);
                                        exit(1);
                                    }
                                } else {
                                    printf("Erro: Identificador '%s' não encontrado.\n", tk.lexema);
                                    exit(1);
                                }
                                // até aqui
                            } else {
                                error("Constante inteira ou identificador esperado!");
                            }

                            tk = AnalisadorLexico(fd);
                            ConsomeFimExp(); 

                            if (tk.cat == SN && tk.codigo == COL_DIREITO) {
                                tk = AnalisadorLexico(fd);
                                ConsomeFimExp();
                            } else {
                                error("Fechamento de colchetes esperado!");
                            }

                            if (tk.cat == SN && tk.codigo == COL_ESQUERDO) {
                                error("A linguagem só suporta o tipo estruturado array do tipo vetor e matriz.");
                            }
                        } else {
                            tb.array = ID_ARRAY_VETOR;
                           
                        }
                    } else {
                        error("Fechamento de colchetes esperado!");
                    }
                }

                    } while (tk.codigo == VIRGULA && (tk = AnalisadorLexico(fd), true)); 
                }

                if (tk.codigo == PAR_DIREITO) {
            
                    tk = AnalisadorLexico(fd);
                    ConsomeFimExp(); 
                } else {
                    error("Fechamento de parênteses esperado após parâmetros!");
                }
            }
        } else {
            error("Identificador de procedimento ou 'init' esperado após 'def'!");
        }

        while (tk.cat == PV_R && (tk.codigo == CONST || tk.codigo == INT || tk.codigo == CHAR || tk.codigo == REAL || tk.codigo == BOOL)) {
     
        DeclListVar(); 
        }
     
        if (tk.codigo != ENDP) {
            do {
                Cmd(); 
            } while (tk.codigo != ENDP && tk.codigo != EOF); 
        }

        
        if (tk.codigo == ENDP) {
            
           
            printf("Tabela de simbolos: \n");
            Imprimir_tabela();

            TrasformarZumbi(escopoAtual);
            RemoverVariaveisLocais(escopoAtual);

            printf("Após remoção de variáveis locais: \n");	
            Imprimir_tabela();
     
            tk = AnalisadorLexico(fd);
            ConsomeFimExp(); 
            escopoAtual--;
            
          
        } else {
            error("'endp' esperado ao final da definição de procedimento!");
        }
    } else {
        error("'prot' ou 'def' esperado no início da definição de procedimento!");
    }

    
}


void DeclVar() {
    
    FILE *arq = fopen("saida.txt", "a");
    char lexema[TAM_MAX_LEXEMA];
    strncpy(lexema, tk.lexema, TAM_MAX_LEXEMA - 1);
    lexema[TAM_MAX_LEXEMA - 1] = '\0';
    

    tb.array = VAR_SIMPLES;

    if (escopoAtual == 0) {
        tb.categoria = VAR_GLOBAL;
    } else if (escopoAtual == 1 && procedimento == 0) {
        tb.categoria = VAR_LOCAL;
    } else if (escopoAtual == 1 && procedimento == 1) {
        tb.categoria = PROCEDIMENTO;
    }
    
    if (tb.tipo == NA_TIPO) {
        Tipo();
    }

    if (Consultar_escopo(lexema, escopoAtual) != -1) {
        printf("[ERRO] Identificador '%s' já declarado no escopo atual.\n", lexema);
        error("Identificador já declarado no escopo atual.");
    }

    
    if (tk.cat == ID) {
        tk = AnalisadorLexico(fd);
        ConsomeFimExp(); 

        if (tb.eh_const == EH_CONST ) {
            if (tk.cat != SN && tk.codigo != ATRIBUICAO) {
                error("Declaração de constante invalida \n");
            }
        }
        
    } else {
        error("Esperado identificador na linha \n");
     
    }

   if (tk.codigo == COL_ESQUERDO) {
        tb.dim1 = 0;
        tk = AnalisadorLexico(fd);
        ConsomeFimExp(); 

        if ((tk.cat == CT_I)) {
            tb.dim1 = tk.valorInt;
        } else if (tk.cat == ID) {
            // VOLTAR DEPOIS
            // SE O ID FOR CONST, OU SEJA UMA VARIAVEL CONSTANTE
            //aqui
            int indice = Consultar_tabela(tk.lexema);
            if (indice != -1) {
                if (tabela_idef.tabela_simb[indice].eh_const == EH_CONST &&
                    tabela_idef.tabela_simb[indice].tipo == NUM_INT) {
                    tb.dim1 = tabela_idef.tabela_simb[indice].valor_const.valor_int;
                } else {
                    printf("Erro: '%s' não é uma constante inteira.\n", tk.lexema);
                    exit(1);
                }
            } else {
                printf("Erro: Identificador '%s' não encontrado.\n", tk.lexema);
                exit(1);
            }

            //ate aqui
            
        } else {
                
                error("Constante inteira ou identificador esperado!");
            } 

        tk = AnalisadorLexico(fd);
        ConsomeFimExp(); // Consome '}'

            if (tk.codigo == COL_DIREITO) {
                tk = AnalisadorLexico(fd);
                ConsomeFimExp(); // Consumir "]"
                if (tb.eh_const == EH_CONST ) {
                    if (tk.cat != SN && tk.codigo != ATRIBUICAO) {
                        error("Declaração de constante invalida \n");
                    }
                }
                if (tk.codigo == COL_ESQUERDO) {
                    tk = AnalisadorLexico(fd);
                    ConsomeFimExp(); // Consome '}'
                    tb.array = ID_ARRAY_MATRIZ;
                    
                    // int a[2][

                    if ((tk.cat == CT_I)) {
                        tb.dim2 = tk.valorInt; 
                    } else if (tk.cat == ID) {
                         //voltar depois
                         // SE O ID FOR CONST, OU SEJA UMA VARIAVEL CONSTANTE
                        int indice = Consultar_tabela(tk.lexema);
                        if (indice != -1) {
                            if (tabela_idef.tabela_simb[indice].eh_const == EH_CONST &&
                                tabela_idef.tabela_simb[indice].tipo == NUM_INT) {
                                tb.dim2 = tabela_idef.tabela_simb[indice].valor_const.valor_int;
                            } else {
                                printf("Erro: '%s' não é uma constante inteira.\n", tk.lexema);
                                exit(1);
                            }
                        } else {
                            printf("Erro: Identificador '%s' não encontrado.\n", tk.lexema);
                            exit(1);
                        }

                        //ate  aqui
                        
                    } else {
                        error("Constante inteira ou identificador esperado!");
                    }   

                    tk = AnalisadorLexico(fd);
                    ConsomeFimExp(); // Consome '}'
                        
                    if (tk.codigo == COL_DIREITO) {
                        tk = AnalisadorLexico(fd);
                        ConsomeFimExp(); // Consumir "]"
                        if (tb.eh_const == EH_CONST ) {
                            if (tk.cat != SN && tk.codigo != ATRIBUICAO) {
                                error("Declaração de constante invalida \n");
                            }
                        }
                    } else {
                        error("Fechamento de colchetes esperado!");
                    }

                    if (tk.cat == SN && tk.codigo == COL_ESQUERDO) {
                        error("A linguagem só suporta o tipo estruturado array do tipo vetor e matriz.");
                    }
                     
                } else {
                    tb.array = ID_ARRAY_VETOR;
                }
            } else {
                error("Fechamento de colchetes esperado!");
            }
        } 

    if (tk.codigo == ATRIBUICAO) {

        tk = AnalisadorLexico(fd);
        ConsomeFimExp();

        
        if (tk.cat == CT_I ){
            fprintf(arq, "PUSH %d\n", tk.valorInt);
           
          
            
            tb.valorConst = tk.codigo;
            tb.valor_const.valor_int = tk.valorInt;
            
            tk = AnalisadorLexico(fd);
            ConsomeFimExp(); 
            
        } else if (tk.cat == CT_R ){
            fprintf(arq, "PUSH %.2f\n", tk.valorReal);
            
            
            tb.valorConst = tk.codigo;
            tb.valor_const.valor_real = tk.valorReal;
          
            tk = AnalisadorLexico(fd);
            ConsomeFimExp(); 
           
        } else if (tk.cat == CT_C ){
            tb.valorConst = tk.codigo;
            tb.valor_const.valor_char = tk.c;
          
            tk = AnalisadorLexico(fd);
            ConsomeFimExp(); 
        } else if (tk.codigo == CHA_ESQUERDA) {
            tk = AnalisadorLexico(fd);
            ConsomeFimExp(); 
            
            if (tk.cat == CT_I || tk.cat == CT_R || tk.cat == CT_C) {
           
            tk = AnalisadorLexico(fd);
            ConsomeFimExp(); 
            } else {
                error("Erro: valor inválido para atribuição na linha \n");
               
            }
        int cont = 1;

        while (tk.cat == SN && tk.codigo == VIRGULA)
        {
            cont++;
            tk = AnalisadorLexico(fd);
            ConsomeFimExp(); // Consome a vírgula

            if (tk.cat == CT_I || tk.cat == CT_R || tk.cat == CT_C) {
            
            tk = AnalisadorLexico(fd);
            ConsomeFimExp(); // Consome o valor (intcon, realcon, etc.)
            } 
        }

        if (tb.array == ID_ARRAY_VETOR) {
            
            if (tb.dim1 < cont) {
                
                error("Erro: quantidade de valores incompatível com a declaração de array vetor\n");
                
            } 
        }

        if (tb.array == ID_ARRAY_MATRIZ) {
            
            int tamm = tb.dim1 * tb.dim2;
            if (tamm < cont) {
                
                error("Erro: quantidade de valores incompatível com a declaração de array matriz \n");
                
            } 
        }

        cont = 0;

        if (tk.codigo == CHA_DIREITA) {
            tk = AnalisadorLexico(fd);
            ConsomeFimExp(); // Consome '}'
        } else {
            error("Erro: esperado '}' para fechar a lista de identificadores na linha \n");
            
        }
            
        } else {
            error("Declaração de variável inválida\n");
        }

    //fprintf(arq, "STOR %d, %d\n", escopoAtual, tabela_idef.tamTab);

    fprintf(arq, "STOR %s\n", lexema);
    
  

   

    } 
    
    if (Inserir_tabela(lexema, escopoAtual, tb.tipo, tb.categoria, false, tb.eh_const, tb.valor_const, tb.array, tb.dim1, tb.dim2, tb.valorConst, tb.passagem) == -1) {
        printf("[ERRO] Não foi possível inserir o identificador '%s' na tabela de símbolos.\n", lexema);
    }
    
}


void Tipo() {

    if (tk.codigo == INT && tk.cat == PV_R) {
      
        tb.tipo = NUM_INT;
        tk = AnalisadorLexico(fd);
        ConsomeFimExp(); // Consome 'int'
    } 
    else if (tk.codigo == CHAR && tk.cat == PV_R) {
        
        tb.tipo = CARACTER_CHAR;
        tk = AnalisadorLexico(fd);
        ConsomeFimExp(); 
    }
    else if (tk.cat == PV_R && tk.codigo == REAL) {
        tb.tipo = NUM_REAL;
        tk = AnalisadorLexico(fd);
        ConsomeFimExp(); 

    } 
    else if (tk.codigo == BOOL) {

        tb.tipo = BOOLEANO;
        tk = AnalisadorLexico(fd);
        ConsomeFimExp(); 
    } 
    else {
        // Se não for nenhum tipo válido, gera um erro
        error("Tipo inválido \n");
       
    }

    
}

void Cmd() {
    

    if (tk.cat == PV_R && tk.codigo == DO) { // do idproc ( [ expr1 { , expr2 }] )
    
        tk = AnalisadorLexico(fd);
        ConsomeFimExp(); // Consumir "do"

        if (tk.cat == ID) {
            tk = AnalisadorLexico(fd);
            ConsomeFimExp(); // Consumir idproc

            if (tk.cat == SN && tk.codigo == PAR_ESQUERDO) {
                tk = AnalisadorLexico(fd);
                ConsomeFimExp(); // Consumir "("

                if (tk.codigo != PAR_DIREITO) {
                    Expr(); // Processar expr1
                    while (tk.codigo == VIRGULA) { // { , expr2 }
                        tk = AnalisadorLexico(fd);
                        ConsomeFimExp(); // Consumir ","
                        Expr(); // Processar expr2
                    }
                }

                if (tk.cat == SN && tk.codigo == PAR_DIREITO) {
                    tk = AnalisadorLexico(fd);
                    ConsomeFimExp(); // Consumir ")"
                } else {
                    error("')' esperado!");
                }
            } else {
                error("'(' esperado após idproc!");
            }
        } else {
            error("idproc esperado após 'do'!");
        }

    } else if (tk.cat == PV_R && tk.codigo == WHILE) { // while (expr) cmd endw
       
      
        tk = AnalisadorLexico(fd);
        ConsomeFimExp(); // Consumir "while"
       

        if (tk.cat == SN && tk.codigo == PAR_ESQUERDO) {
            tk = AnalisadorLexico(fd);
            ConsomeFimExp(); // Consumir "("
            Expr(); // Processar expr

            if (tk.cat == SN && tk.codigo == PAR_DIREITO) {
                tk = AnalisadorLexico(fd);
                ConsomeFimExp(); // Consumir ")"

                
                
                
                escopoAtual++;
            
                if (tk.codigo != ENDW ) { // Certifique-se de que há comandos para processar
                    do {
                        Cmd(); // Processar um comando
                    } while (tk.codigo != ENDW && tk.codigo != EOF); // Continuar enquanto não encontrar "endp" ou o fim do arquivo
                }

                escopoAtual--;

                if (tk.cat == PV_R && tk.codigo == ENDW) {
                   
                    tk = AnalisadorLexico(fd);
                    ConsomeFimExp(); // Consumir "endw"
                } else {
                    error("'endw' esperado após comando while!");
                }
            } else {
                error("')' esperado após expressão while!");
            }
        } else {
            error("'(' esperado após while!");
        }

    } else if (tk.cat == PV_R && tk.codigo == VAR) { // var id from expr1 (to | dt) expr2 [ by ( intcon | ID ) ] cmd endv
        tk = AnalisadorLexico(fd);
        ConsomeFimExp(); // Consumir "var"

        if (tk.cat == ID) {
            tk = AnalisadorLexico(fd);
            ConsomeFimExp(); // Consumir id

            if (tk.cat == PV_R && tk.codigo == FROM) {
                tk = AnalisadorLexico(fd);
                ConsomeFimExp(); // Consumir "from"
                Expr(); // Processar expr1

                if (tk.cat == PV_R && (tk.codigo == TO || tk.codigo == DT)) {
                    tk = AnalisadorLexico(fd);
                    ConsomeFimExp(); // Consumir "to" ou "dt"
                    Expr(); // Processar expr2

                    if (tk.cat == PV_R && tk.codigo == BY) {
                        tk = AnalisadorLexico(fd);
                        ConsomeFimExp(); // Consumir "by"

                        if (tk.cat == CT_I || tk.cat == ID) {
                            tk = AnalisadorLexico(fd);
                            ConsomeFimExp(); // Consumir constante ou ID
                        } else {
                            error("Constante ou ID esperado após 'by'!");
                        }
                    }
                    escopoAtual++;
                    Cmd(); // Processar cmd
                    escopoAtual--;
                    
                    if (tk.cat == PV_R && tk.codigo == ENDV) {
                        tk = AnalisadorLexico(fd);
                        ConsomeFimExp(); // Consumir "endv"
                    } else {
                        error("'endv' esperado após comando var!");
                    }
                } else {
                    error("'to' ou 'dt' esperado após expressão from!");
                }
            } else {
                error("'from' esperado após id!");
            }
        } else {
            error("id esperado após 'var'!");
        }

    } else if (tk.cat == PV_R && tk.codigo == IF) { // if ( expr ) cmd { elif ( expr ) cmd } [ else cmd ] endi
    
        tk = AnalisadorLexico(fd);
        ConsomeFimExp(); // Consumir "if"

        if (tk.cat == SN && tk.codigo == PAR_ESQUERDO) {
            
            tk = AnalisadorLexico(fd);
            ConsomeFimExp(); // Consumir "("
            Expr(); // Processar expr

            if (tk.cat == SN && tk.codigo == PAR_DIREITO) {
                
                tk = AnalisadorLexico(fd);
                ConsomeFimExp(); // Consumir ")"
              
                Cmd(); // Processar cmd
                

                while (tk.cat == PV_R && tk.codigo == ELIF) { // { elif ( expr ) cmd }
                    
                    tk = AnalisadorLexico(fd);
                    ConsomeFimExp(); // Consumir "elif"

                    if (tk.cat == SN && tk.codigo == PAR_ESQUERDO) {
                       
                        tk = AnalisadorLexico(fd);
                        ConsomeFimExp(); // Consumir "("
                        Expr(); // Processar expr

                        if (tk.cat == SN && tk.codigo == PAR_DIREITO) {
                           
                            tk = AnalisadorLexico(fd);
                            ConsomeFimExp(); // Consumir ")"
                            Cmd(); // Processar cmd
                        } else {
                            error("')' esperado após expressão elif!");
                        }
                    } else {
                        error("'(' esperado após elif!");
                    }
                }

                if (tk.cat == PV_R && tk.codigo == ELSE) { // [ else cmd ]
       
                    tk = AnalisadorLexico(fd);
                    ConsomeFimExp(); // Consumir "else"
                 
                    
                    if (tk.codigo != ENDI ) { // Certifique-se de que há comandos para processar
                    do {
                        Cmd(); // Processar um comando
                    } while (tk.codigo != ENDI && tk.codigo != EOF); // Continuar enquanto não encontrar "endp" ou o fim do arquivo
                    }

                    
                }

                if (tk.cat == PV_R && tk.codigo == ENDI) {
                    tk = AnalisadorLexico(fd);
                    ConsomeFimExp(); // Consumir "endi"
                } else {
                    error("'endi' esperado após comando if!");
                }
            } else {
                error("')' esperado após expressão if!");
            }
        } else {
            error("'(' esperado após if!");
        }

    } else if (tk.cat == ID) { // atrib
        Atrib();

    } else if (tk.cat == PV_R && tk.codigo == GETOUT) { // getout
        
        tk = AnalisadorLexico(fd);
        ConsomeFimExp(); // Consumir "getout"

    } else if (tk.cat == PV_R && tk.codigo == GETINT) { // getint id
        
        tk = AnalisadorLexico(fd);
        ConsomeFimExp(); // Consumir "getint"

        if (tk.cat == ID) {
          
            tk = AnalisadorLexico(fd);
            ConsomeFimExp(); // Consumir id
        } else {
            error("id esperado após 'getint'!");
        }

    } else if (tk.cat == PV_R && tk.codigo == GETREAL) { // getreal id
        tk = AnalisadorLexico(fd);
        ConsomeFimExp(); // Consumir "getreal"

        if (tk.cat == ID) {
            tk = AnalisadorLexico(fd);
            ConsomeFimExp(); // Consumir id
        } else {
            error("id esperado após 'getreal'!");
        }

    } else if (tk.cat == PV_R && tk.codigo == GETCHAR) { // getchar id
        tk = AnalisadorLexico(fd);
        ConsomeFimExp(); // Consumir "getchar"

        if (tk.cat == ID) {
            tk = AnalisadorLexico(fd);
            ConsomeFimExp(); // Consumir id
        } else {
            error("id esperado após 'getchar'!");
        }

    } else if (tk.cat == PV_R && tk.codigo == GETSTR) { // getstr id
        tk = AnalisadorLexico(fd);
        ConsomeFimExp(); // Consumir "getstr"

        if (tk.cat == ID) {
            tk = AnalisadorLexico(fd);
            ConsomeFimExp(); // Consumir id
        } else {
            error("id esperado após 'getstr'!");
        }

    } else if (tk.cat == PV_R && tk.codigo == PUTINT) { // putint (id | intcon)
        tk = AnalisadorLexico(fd);
        ConsomeFimExp(); // Consumir "putint"

        if (tk.cat == ID || tk.cat == CT_I) {
            tk = AnalisadorLexico(fd);
            ConsomeFimExp(); // Consumir id ou intcon
        } else {
            error("id ou constante inteira esperado após 'putint'!");
        }

    } else if (tk.cat == PV_R && tk.codigo == PUTREAL) { // putreal (id | realcon)
        tk = AnalisadorLexico(fd);
        ConsomeFimExp(); // Consumir "putreal"

        if (tk.cat == ID || tk.cat == CT_R) {
            tk = AnalisadorLexico(fd);
            ConsomeFimExp(); // Consumir id ou realcon
        } else {
            error("id ou constante real esperado após 'putreal'!");
        }

    } else if (tk.cat == PV_R && tk.codigo == PUTCHAR) { // putchar (id | charcon)
        tk = AnalisadorLexico(fd);
        ConsomeFimExp(); // Consumir "putchar"

        if (tk.cat == ID || tk.cat == CT_C) {
            tk = AnalisadorLexico(fd);
            ConsomeFimExp(); // Consumir id ou charcon
        } else {
            error("id ou constante caractere esperado após 'putchar'!");
        }

    } else if (tk.cat == PV_R && tk.codigo == PUTSTR) { // putstr (id | stringcon)
        tk = AnalisadorLexico(fd);
        ConsomeFimExp(); // Consumir "putstr"

        if (tk.cat == ID || tk.cat == LT) {
            tk = AnalisadorLexico(fd);
            ConsomeFimExp(); // Consumir id ou stringcon
        } else {
            error("id ou constante string esperado após 'putstr'!");
        }

    } else {
        error("Comando não reconhecido!");
    }

   
}


void Atrib() {
   

    // Verifica se o token atual é um identificador (id)
    if (tk.cat == ID) {
        tk = AnalisadorLexico(fd);
        ConsomeFimExp(); // Consome o identificador (id)
    } else {
        error("Erro: esperado identificador \n");
        
    }

    // Atribuição de um valor à variável
    

    // Se houver uma expressão opcional entre colchetes (caso haja repetição)
    while (tk.cat == SN &&tk.codigo == COL_ESQUERDO) {
        tk = AnalisadorLexico(fd);
        ConsomeFimExp(); // Consome o '['

        // Analisa a expressão dentro dos colchetes
        Expr();

        // Consome o colchete de fechamento, se existir
        if (tk.cat == SN && tk.codigo == COL_DIREITO) {
            tk = AnalisadorLexico(fd);
            ConsomeFimExp(); // Consome o ']'
        } else {
            error("Erro: esperado ']' para fechar a expressão entre colchetes \n");
           
        }
    }

    if (tk.cat == SN && tk.codigo == ATRIBUICAO) {
        tk = AnalisadorLexico(fd);
        ConsomeFimExp(); // Consome o '='

        // Processa a expressão que será atribuída
        Expr(); // Analisa a expressão à direita da atribuição
        
    } else {
        error("Erro: esperado '=' para atribuição \n");
        
    }

   
}


void Expr() {

    // Analisar a primeira parte da expressão (expr_simp)
    ExprSimp(); 

    // Verifica se existe uma operação relacional (op_rel)
    if (tk.cat == SN && (tk.codigo == IGUALDADE || tk.codigo == DIFERENTE || tk.codigo == MENOR_QUE || tk.codigo == MAIOR_QUE || tk.codigo == MENOR_IGUAL || tk.codigo == MAIOR_IGUAL )) {

        // Se existir, consome o operador relacional
        OpRel(); // Analisa a operação relacional

        // Analisar a segunda parte da expressão (expr_simp)
        ExprSimp(); 
    }

}


void ExprSimp() {
    
    // Verificar se existe um sinal de '+' ou '-' no início
    if (tk.cat == SN && (tk.codigo == ADICAO || tk.codigo == SUBTRACAO)) {
       
        tk = AnalisadorLexico(fd);
        ConsomeFimExp(); // Consome o operador
    }

    // Processa o primeiro termo da expressão
    Termo(); // Chama a função que analisa um termo

    // Verifica se há um operador de adição (+), subtração (-) ou união (||)
    while (tk.cat == SN && (tk.codigo == ADICAO || tk.codigo == SUBTRACAO || tk.codigo == OR)) {
      
        tk = AnalisadorLexico(fd);
        ConsomeFimExp(); // Consome o operador

        // Processa o próximo termo após o operador
        Termo(); // Chama a função que analisa o próximo termo
    }

   
}


void Termo() {
   

    // Analisando o primeiro fator
    Fator(); 

    // Verifica se existe multiplicação, divisão ou operação lógica "&&"
    while ( tk.cat == SN && (tk.codigo == MULTIPLICACAO || tk.codigo == DIVISAO || tk.codigo == AND)) {
       
        tk = AnalisadorLexico(fd);
        ConsomeFimExp(); // Consome o operador

        // Processa o próximo fator
        Fator(); 
    }

   
}


void Fator() {
    
    // Verifica se o fator é um identificador
    if (tk.cat == ID) {
        
        tk = AnalisadorLexico(fd);
        ConsomeFimExp(); // Consome o identificador (id)

        while (tk.cat == SN && (tk.codigo == COL_ESQUERDO))
        {
            
            tk = AnalisadorLexico(fd);
            ConsomeFimExp();

            Expr();

            if(tk.cat == SN && (tk.codigo == COL_DIREITO)){
                
                tk = AnalisadorLexico(fd);
                ConsomeFimExp();
            }
        }
    } 

    // Verifica se o fator é uma constante inteira
    else if (tk.cat == CT_I) {
        
        tk = AnalisadorLexico(fd);
        ConsomeFimExp(); 
    } 
    // Verifica se o fator é uma constante real
    else if (tk.cat == CT_R) {
        
        tk = AnalisadorLexico(fd);
        ConsomeFimExp(); 
    } 
    // Verifica se o fator é uma constante char
    else if (tk.cat == CT_C) {
        
        tk = AnalisadorLexico(fd);
        ConsomeFimExp(); 
    } 
   
    else if (tk.codigo == PAR_ESQUERDO) {
        tk = AnalisadorLexico(fd);
        ConsomeFimExp(); 
        Expr(); 
        
        if (tk.codigo == PAR_DIREITO) {
            tk = AnalisadorLexico(fd);
            ConsomeFimExp(); 
        } else {
            error("Erro: esperado ')' na linha \n");
           
        }
    } 
    
    else if (tk.codigo == NOT) {
       
        tk = AnalisadorLexico(fd);
        ConsomeFimExp(); 
        Fator(); 
    } else {
        error("Definição de fator nao reconhecida!");
    }
}


void OpRel() {
  
    if (tk.cat == SN && tk.codigo == IGUALDADE) {
        
        tk = AnalisadorLexico(fd);
        ConsomeFimExp(); 
    } 
    else if (tk.cat == SN && tk.codigo == DIFERENTE) {
       
        tk = AnalisadorLexico(fd);
        ConsomeFimExp(); 
    } 
    else if (tk.cat == SN && tk.codigo == MENOR_QUE) {
       
        tk = AnalisadorLexico(fd);
        ConsomeFimExp(); 
    } 
    else if (tk.cat == SN && tk.codigo == MAIOR_QUE) {
        
        tk = AnalisadorLexico(fd);
        ConsomeFimExp(); 
    } 
    else if (tk.cat == SN && tk.codigo == MENOR_IGUAL) {
        
        tk = AnalisadorLexico(fd);
        ConsomeFimExp(); 
    } 
    else if (tk.cat == SN && tk.codigo == MAIOR_IGUAL) {
       
        tk = AnalisadorLexico(fd);
        ConsomeFimExp();
    } 
    else {
        error("Erro: operador relacional esperado na linha \n");
       
    }
}
