#ifndef SCANNER_H
#define SCANNER_H

#include <stdio.h>

typedef enum {
    TOK_IDENTIFIER = 1,
    TOK_CONSTANT,
    TOK_ASIGN,
    TOK_PUNTOYCOMA,
    TOK_COMA,
    TOK_MULT,
    TOK_DIV,
    TOK_SUMA,
    TOK_MENOS,
    TOK_LPAREN,
    TOK_RPAREN,

    TOK_ERROR_GENERAL,
    TOK_FIN
} TokenTipo;

typedef struct {
    TokenTipo tipo;
    char lexema[256];
} Token;

void iniciarScanner(FILE *f);
Token proxToken();
void imprimirToken(Token t);

#endif