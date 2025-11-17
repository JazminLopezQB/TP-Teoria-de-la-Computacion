#ifndef SCANNER_H
#define SCANNER_H

// Bibliotecas
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define LONGITUD 33 // La longitud máxima de los lexemas es de 32 caracteres

typedef enum { // Tipos de tokens posibles
    // Generales
    IDENTIFICADOR,
    CONSTANTE,
    ASIGNAR,
    // Errores
    ERROR_GENERAL,
    ERROR_ASIGNACION,
    // Separadores
    PUNTOYCOMA,
    COMA,
    PAREN_ABR,
    PAREN_CERR,
    // Operadores
    MULTIPLICAR,
    DIV,
    SUMA,
    MENOS,
    // Final
    FIN
    } TokenTipo;

typedef struct { // Estructura que devuelve el scanner, un token más el lexema
    TokenTipo tipo;
    char lexema[LONGITUD];
    } Token;

static int tipoCaracter(char c); // Devuelve que tipo de caracter es el recibido
static TokenTipo tokenDeEstado(int est); // Según el estado aceptor devolver un token
static Token crearToken(TokenTipo tipo, const char *lex); // Crear la estructura que devolverá el scanner
FILE *abrir_archivo(char nombre[]); // Gestiona la apertura de un archivo en caso de usarlo
Token scanner(FILE *entrada); // Analiza la entrada y navega la tabla de transición
void imprimirToken(Token t); // Muestra los lexemas

#endif