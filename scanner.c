#include "scanner.h"
#include <ctype.h>
#include <string.h>

static FILE *entrada = NULL;

// CLASES DE CARACTERES
enum {
    C_LETRA = 0,
    C_DIGITO,
    C_DOSP,
    C_IGUAL,
    C_PUNTOYCOMA,
    C_COMA,
    C_MULT,
    C_DIV,
    C_MAS,
    C_MENOS,
    C_PAR_ABRE,
    C_PAR_CIERRA,
    C_ESP,
    C_INV,
    C_EOF
};

static int claseCaracter(int c) {
    if (c == EOF) return C_EOF;
    if (isalpha(c)) return C_LETRA;
    if (isdigit(c)) return C_DIGITO;
    if (c == ':') return C_DOSP;
    if (c == '=') return C_IGUAL;
    if (c == ';') return C_PUNTOYCOMA;
    if (c == ',') return C_COMA;
    if (c == '*') return C_MULT;
    if (c == '/') return C_DIV;
    if (c == '+') return C_MAS;
    if (c == '-') return C_MENOS;
    if (c == '(') return C_PAR_ABRE;
    if (c == ')') return C_PAR_CIERRA;
    if (isspace(c)) return C_ESP;
    return C_INV;
}

// TABLA DE TRANSICIÓN
static int TT[4][15] = {
/*            L   D   :   =   ;   ,   *   /   +   -   (   )  ESP INV EOF */
/*0*/       {  1,  2,  3, 99, 14, 15, 16, 17, 18, 19, 20, 21,  0, 99, 22 },
/*1*/       {  1,  1, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11 },
/*2*/       { 99,  2, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 99, 12 },
/*3*/       { 99, 99, 99, 13, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99 }
};

// Mapeo estado → token
static TokenTipo tokenDeEstado(int est) {
    switch (est) {
        case 11: return TOK_IDENTIFIER;
        case 12: return TOK_CONSTANT;
        case 13: return TOK_ASIGN;
        case 14: return TOK_PUNTOYCOMA;
        case 15: return TOK_COMA;
        case 16: return TOK_MULT;
        case 17: return TOK_DIV;
        case 18: return TOK_SUMA;
        case 19: return TOK_MENOS;
        case 20: return TOK_LPAREN;
        case 21: return TOK_RPAREN;
        case 22: return TOK_FIN;
        default: return TOK_ERROR_GENERAL;
    }
}

// ============================================================================
static Token hacerToken(TokenTipo tipo, const char *lex) {
    Token t;
    t.tipo = tipo;
    strncpy(t.lexema, lex ? lex : "", 255);
    t.lexema[255] = '\0';
    return t;
}

void iniciarScanner(FILE *f) {
    entrada = f;
}

// ============================================================================
// SCANNER PRINCIPAL
// ============================================================================
Token proxToken() {

    char lex[256];
    int p = 0;
    int estado = 0;

    while (1) {

        int c = fgetc(entrada);
        int cl = claseCaracter(c);

        // EOF sin empezar token
        if (cl == C_EOF && p == 0)
            return hacerToken(TOK_FIN, "");

        // EOF cortando token
        if (cl == C_EOF && p > 0) {
            lex[p] = '\0';
            return hacerToken(tokenDeEstado(estado), lex);
        }

        // ignorar espacios antes
        if (cl == C_ESP && p == 0)
            continue;

        // espacio cortando token
        if (cl == C_ESP && p > 0) {
            lex[p] = '\0';
            return hacerToken(tokenDeEstado(estado), lex);
        }

        // inválido al inicio
        if (cl == C_INV && p == 0) {
            lex[0] = (char)c;
            lex[1] = '\0';
            return hacerToken(TOK_ERROR_GENERAL, lex);
        }

        int nuevo = TT[estado][cl];

        // estado de trabajo
        if (nuevo < 4) {
            lex[p++] = (char)c;
            estado = nuevo;
            continue;
        }

        // ===============================
        // Estados aceptores
        // ===============================

        // ASIGNACION :=
        if (nuevo == 13) {
            lex[p++] = '=';
            lex[p] = '\0';
            return hacerToken(TOK_ASIGN, lex);
        }

        // ID o NUM → ungetc
        if (nuevo == 11 || nuevo == 12) {
            ungetc(c, entrada);
            lex[p] = '\0';
            return hacerToken(tokenDeEstado(nuevo), lex);
        }

        // símbolos de un solo carácter
        lex[p++] = (char)c;
        lex[p] = '\0';
        return hacerToken(tokenDeEstado(nuevo), lex);
    }
}

// ============================================================================
void imprimirToken(Token t) {

    switch (t.tipo) {

        case TOK_IDENTIFIER:  printf("Identificador '%s'\n",   t.lexema); break;
        case TOK_CONSTANT:    printf("Constante '%s'\n",       t.lexema); break;
        case TOK_PUNTOYCOMA:  printf("Punto y coma ';'\n");    break;
        case TOK_COMA:        printf("Coma ','\n");            break;
        case TOK_LPAREN:      printf("Parentesis que abre '('\n"); break;
        case TOK_RPAREN:      printf("Parentesis que cierra ')'\n"); break;
        case TOK_SUMA:        printf("Suma '+'\n");             break;
        case TOK_MENOS:       printf("Menos '-'\n");            break;
        case TOK_MULT:        printf("Multiplicacion '*'\n");  break;
        case TOK_DIV:         printf("Division '/'\n");        break;
        case TOK_ASIGN:       printf("Asignacion ':='\n");     break;

        case TOK_ERROR_GENERAL:
            printf("Error '%s'\n", t.lexema);
            break;

        case TOK_FIN:
            break;
    }
}
