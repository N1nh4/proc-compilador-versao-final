#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "AnalisadorLexico.h"
#include "Funcaux.h"

void error(char msg[]) {
    printf("%s\n", msg);
    exit(1);
}

int identificarPR(const char *lexema) {
    
    for(int i = 0; i < 31; i++) {
        if (strcmp(lexema, TABELA_PR[i]) == 0) {
            return i + 1;
        }
    }

    return 0;
}

void PrintNodo(char info[], int movim) {
 
    if (movim == AVANCA) { printf("%s%s\n", TABS, info); strcat(TABS, "\t"); }
    else if (movim == MANTEM) { printf("%s%s\n", TABS, info); }
    else if (movim == RETROCEDE) { TABS[strlen(TABS)-1] = '\0'; }
}

void PrintNodoInt(int val, int movim) {
 
    if (movim == AVANCA) { printf("%s%d\n", TABS, val); strcat(TABS, "\t"); }
    else if (movim == MANTEM) { printf("%s%d\n", TABS, val); }
    else if (movim == RETROCEDE) { TABS[strlen(TABS)-1] = '\0'; }
}

void ConsomeFimExp(){
    while(tk.cat == FIM_EXPR){ tk = AnalisadorLexico(fd); }
}

extern char TABELA_PR[31][20];