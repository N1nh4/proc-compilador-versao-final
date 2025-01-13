#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "AnalisadorLexico.h"
#include "Funcaux.h"

#define TAM_LEXEMA 50
#define TAM_NUM 20

char TABELA_PR[31][20]={
    "const",        
    "getstr",                
    "init",              
    "endp",              
    "char",              
    "int",              
    "real",              
    "bool",              
    "do",                
    "while",            
    "endw",              
    "var",              
    "from",              
    "to",                
    "dt",                
    "by",                
    "if",                
    "endv",              
    "elif",              
    "else",              
    "endi",              
    "getout",            
    "getint",            
    "getchar",          
    "getreal",          
    "putint",            
    "putchar",          
    "putreal",
    "prot",
    "def",
    "putstr"
};

TOKEN AnalisadorLexico(FILE *fd) {

    
    int estado = 0;
    char c;
    int tamL = 0;
    int tamD = 0;
    char lexema[TAM_LEXEMA] = "";
    char digitos[TAM_NUM] = "";


    while (1) {
        c = fgetc(fd);
        
        switch (estado) {
            case 0:  // Estado inicial
                if (c == ' ' || c == '\t' ) {
                    estado = 0;  // Ignora espaços e tabulações
                } 
                else if (isalpha(c)) {
                    estado = 1;  // Inicio de ID começando com letra
                    lexema[tamL] = c;
                    lexema[++tamL] = '\0';
                } 
                else if (c == '_') {
                    estado = 2; // Inicio de ID começando com _
                    lexema[tamL] = c;
                    lexema[++tamL] = '\0';
                }
                else if (isdigit(c)) {
                    estado = 3;  // Inicio de intcon ou realcon
                    digitos[tamD] = c;
                    digitos[++tamD] = '\0';
                } 
                else if (c == '\'') {
                    estado = 6;  // Inicio de charcon 
                }
                else if (c == '"') {
                    estado = 10; // Inicio de stringcon
                }
                else if (c == '<') { 
                    estado = 25; // Inicio de operador menor que ou menor igual
                }
                else if (c == '>') {
                    estado = 28; // Inicio de operador maior ou maior que ou maior igual
                }
                else if (c == '!') {
                    estado = 34; // Inicio de operador de negação ou not
                }
                else if (c == '=') {
                    estado = 22; // Inicio operador de atribuição ou igualdade 
                } 
                else if (c == '|') {
                    estado = 40; // Inicio de operador or
                }
                else if (c == ',') {
                    estado = 46; // Inicio de virgula
                    tk.cat = SN;
                    tk.codigo = VIRGULA;
                    return tk; // Retorno do token virgula
                }
                else if (c == '(') {
                    estado = 45; // Inicio de parentese esquerdo 
                    tk.cat = SN;
                    tk.codigo =  PAR_ESQUERDO;
                    return tk; // Retorno do token parentese esquerdo
                }
                else if (c == ')') {
                    estado = 44; // Inicio de parentese direito
                    tk.cat = SN;
                    tk.codigo =  PAR_DIREITO;
                    return tk; // Retorno do token parentese direito
                }
                else if (c == '[') {
                    estado = 42; // Inicio de colchetes esquerdo
                    tk.cat = SN;
                    tk.codigo = COL_ESQUERDO;
                    return tk; // Retorno do token colchetes esquerdo
                }
                else if (c == ']') {
                    estado = 43; // Inicio de colchetes direito
                    tk.cat = SN;
                    tk.codigo = COL_DIREITO;
                    return tk; // Retorno do token colchetes direito
                }
                else if (c == '{') {
                    estado = 43; // Inicio de colchetes direito
                    tk.cat = SN;
                    tk.codigo = CHA_ESQUERDA;
                    return tk; // Retorno do token colchetes direito
                }
                else if (c == '}') {
                    estado = 43; // Inicio de colchetes direito
                    tk.cat = SN;
                    tk.codigo = CHA_DIREITA;
                    return tk; // Retorno do token colchetes direito
                }
                else if (c == '&') {
                    estado = 37; // Inico de endereco ou and
                }
                else if (c == '+') {
                    estado = 21; // Inicio do operador de adicao
                    tk.cat = SN;
                    tk.codigo = ADICAO;
                    return tk; // Retorno do token de adicao
                }
                else if (c == '-') {
                    estado = 32; // Inicio do operador de subtracao
                    tk.cat = SN;
                    tk.codigo = SUBTRACAO;
                    return tk; // Retorno do token de subtracao
                }
                else if (c == '*') {
                    estado = 33; // Inicio do operador de multiplicacao
                    tk.cat = SN;
                    tk.codigo = MULTIPLICACAO;
                    return tk; // Retorno do token de multiplicacao
                }
                else if (c == '/') {
                    estado = 12;
                }
                else if(c == EOF){
                    tk.cat = FIM_ARQ; // Fim do arquivo
                    return tk; 
                }
                else if(c == '\n'){
                    estado = 0;
                    tk.cat = FIM_EXPR;
                    contLinha++;
                    return tk;
                }
                else {
                    error("Caracter invalido na expressao!");
                }
                break;

            // ID (começando com letra)
            case 1:
                if (isalpha(c) || isdigit(c) || c == '_') {
                    estado = 1; 
                    lexema[tamL] = c;
                    lexema[++tamL] = '\0';
                } 
                // Verificação de palavras reservadas
                else { 
                    int pr = identificarPR(lexema);
                    if (pr) {
                        tk.cat = PV_R;
                        tk.codigo = pr;
                    } else {
                        tk.cat = ID;
                        strcpy(tk.lexema, lexema);
                    }
				ungetc(c, fd);
				estado = 15;
				return (tk);
			}
			break ;

            // ID (começando com _)
            case 2:
                if (c == '_') {
                    estado = 2; 
                    lexema[tamL] = c;
                    lexema[++tamL] = '\0';
                } else if (isalpha(c)) {
                    estado = 1;
                    lexema[tamL] = c;
                    lexema[++tamL] = '\0';
                } else {
                    error("Erro na construcao do id");
                }
                break;


            // Intcon
            case 3:
                if (isdigit(c)) {
                    estado = 3;  
                    digitos[tamD] = c;
                    digitos[++tamD] = '\0';
                } else if (c == '.') {
                    estado = 4;  
                    digitos[tamD] = c;
                    digitos[++tamD] = '\0';
                } else {
                    ungetc(c, fd); 
                    tk.cat = CT_I;
                    tk.valorInt = atoi(digitos);
                    estado = 17;
                    return tk;
                }
                break;

            // Realcon
            case 4:
                if (isdigit(c)) {
                    estado = 5;  
                    digitos[tamD] = c;
                    digitos[++tamD] = '\0';
                } else {
                    error("Erro na construcao do realcon");
                }
                break;


            case 5:
                if(isdigit(c)) {
                    estado = 5;
                    digitos[tamD] = c;
                    digitos[++tamD] = '\0';
                } else {
                    ungetc(c, fd);
                    tk.cat = CT_R;
                    tk.valorReal = atof(digitos);
                    return tk;
                }
                break;

            // Charcon
            case 6:
                if (c == '\\')
                {
                    estado = 8;
                }
                else if(isprint(c))
                {
                    estado = 7;
                    tk.c = c;
                }
                else {
                    error("Erro na construcao do charcon");
                }
                break;
            
            case 8:
                if (c == '0'){
                    tk.c = '\0';
                    estado = 20;
                }
                else if (c == 'n'){
                    tk.c = '\n';
                    estado = 14;
                } 
                else {
                    error("Erro na construcao do charcon");
                }
                break;
 
            case 7:
                if (c == '\'')
                {
                    estado = 18;
                    tk.cat = CT_C;
                    return tk;
                }
                else {
                    error("Erro na construcao do charcon");
                }
                break;
            
            case 14:
                if (c == '\'')
                {
                    estado = 9;
                    tk.cat = CT_C;
                    tk.c = 10;
                    return tk;
                }
                else {
                    error("Erro na construcao do charcon");
                }
                break;

            case 20:
                if (c == '\'')
                {
                    estado = 48;
                    tk.cat = CT_C;
                    tk.c = 0;
                    return tk;
                }
                else {
                    error("Erro na construcao do charcon");
                }
                break;

            case 12: 
                if(c == '/'){
                    estado = 13;   
                } 
                else {
                    ungetc(c, fd);
                    estado = 31;
                    tk.cat = SN;
                    tk.codigo = DIVISAO;
                    return tk;
                }
                break;
            
            case 13:
                if (c == '\n' || c == EOF) {
                    estado = 0;
                    ungetc(c,fd);
                }
                break;

            case 22:
                if (c == '='){
                    estado = 24;
                    tk.cat = SN;
                    tk.codigo = IGUALDADE;
                    return tk;
                }
                else {
                    estado = 23;
                    ungetc(c, fd);
                    tk.cat = SN;
                    tk.codigo = ATRIBUICAO;
                    return tk;
                }
                break;

            case 25: 
                if (c == '='){
                    estado = 27;
                    tk.cat = SN;
                    tk.codigo = MENOR_IGUAL;
                    return tk;
                }
                else {
                    estado = 26;
                    ungetc(c, fd);
                    tk.cat = SN;
                    tk.codigo = MENOR_QUE;
                    return tk;
                }
                break;

            case 28:
                if (c == '='){
                    estado = 30;
                    tk.cat = SN;
                    tk.codigo = MAIOR_IGUAL;
                    return tk;
                }
                else {
                    estado = 29;
                    ungetc(c, fd);
                    tk.cat = SN;
                    tk.codigo = MAIOR_QUE;
                    return tk;
                }
                break;

            case 34:
                if (c == '='){
                    estado = 35;
                    tk.cat = SN;
                    tk.codigo = DIFERENTE;
                    return tk;
                }
                else {
                    estado = 36;
                    ungetc(c, fd);
                    tk.cat = SN;
                    tk.codigo = NOT;
                    return tk;
                }
                break;
            
            case 37:
                if (c == '&'){
                    estado = 38;
                    tk.cat = SN;
                    tk.codigo = AND;
                    return tk;
                }
                else {
                    estado = 39;
                    ungetc(c, fd);
                    tk.cat = SN;
                    tk.codigo = ENDERECO;
                    return tk;
                }
                break;

            case 40:
                if (c == '|'){
                    estado = 41;
                    tk.cat = SN;
                    tk.codigo = OR;
                    return tk;
                }
                else {
                    error("Erro na construcao do operador or!");
                }
                break;

            // Stringcon
            case 10:
                if (isprint(c) && c != '"') { 
                    estado = 47;
                    lexema[tamL] = c;
                    lexema[++tamL] = '\0';  
                } else {
                    error("Erro na construcao da string");
                }
                break;

            case 47:
                if (isprint(c) && c != '"'){
                    estado = 47;
                    lexema[tamL] = c;
                    lexema[++tamL] = '\0';  
                }
                else if(c == '"') {
                    estado = 11;
                }
                else {
                    error("Erro na construcao da stringcon");
                }
                break;

            case 11:
                estado = 19;
                ungetc(c, fd);
                tk.cat = LT;
                strcpy(tk.lexema, lexema);
                return tk;
                break;

    
        }
    }
    tk.codigo = 0; // Código que representa um token inválido, caso aplicável
    strcpy(tk.lexema, ""); // Lexema vazio
    return tk;
}

