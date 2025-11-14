#ifndef TP_FINAL_SCANNER_H
#define TP_FINAL_SCANNER_H

// Bibliotecas
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definiciones
#define LONGITUD 33 // Los identificadores tienen como longitud máxima 32 chars

FILE *abrir_archivo(char nombre[]);
int es_constante(char *cadena);
void scanner(void);

#endif // TP_FINAL_SCANNER_H
