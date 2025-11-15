#ifndef TP_FINAL_SCANNER_H
#define TP_FINAL_SCANNER_H

// Bibliotecas

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definiciones

#define LONGITUD 33 // Los identificadores tienen como longitud máxima 32 bits

FILE *abrir_archivo(char nombre[]);
int es_constante(char cadena[]);
int es_separador_simple(int c);
int es_separador_especial(int c);
int es_caracter_valido(int c);
void mostrar_identificador(char token[], int *ti, int tipo);
void scanner();

#endif // TP_FINAL_SCANNER_H