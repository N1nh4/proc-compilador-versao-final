#ifndef _FUNC_AUX_
#define _FUNC_AUX_

#include <stdbool.h>

enum MOVIMENTO {AVANCA, MANTEM, RETROCEDE};

extern bool mostraArvore;
extern char TABS[200];

void error(char msg[]);

int identificarPR(const char *lexema);
void PrintNodo(char info[], int movim);
void PrintNodoInt(int val, int movim);
void PrintNodoFloat(float val, int movim);

void ConsomeFimExp();

#endif