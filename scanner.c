#include "scanner.h"
#include <stdio.h>
#include <string.h>

void scanner() {

    // Abrir archivo de entrada y validar que no haya errores
    FILE *entrada = fopen("../entrada.txt", "r");
    
    if (entrada == NULL) {
        printf("\nHubo un error al abrir el archivo\n");
        exit(-1);
        }

    // Definir variables de análisis
    
    char token[LONGITUD]; // Se van guardando lo que se lee del archivo
    char separadores[] = " ;*/+-,=:()\n"; // Cosas que pueden provocar que se corte un identificador
    char errores[] = "@!^$"; // Caracteres que producen errores
    char caracter; // Entrada del archivo
    int i = 0; // Indice para escribir caracter en token

    while ((caracter = fgetc(entrada)) != EOF) { //TODO: Revisar por que strcmp(token, "fin\0") != 0 no anda
        // TODO: También habria que poner un condicional para que descarte cualquier cosa que vena antes de "inicio"
        if (strchr(separadores, caracter) != NULL) { // Si encuentra un separador
            if (i > 0) { // Primero termina de procesar lo que habia antes
                token[i] = '\0';
                printf("Token: %s\n", token);
                i = 0; // Reinicio el indice
                }

            if (caracter != '\n' && caracter != ' ') { // Los espacios y saltos no deberian mostrarse
                printf("Separador: %c\n", caracter);
                }
            }
        else { // Si hay espacio se copia caracter, sino se empieza un analisis nuevo
            if (i < LONGITUD - 1) {
                token[i] = caracter;
                i++;
                }
            else {
                token[i] = '\0';
                printf("Token: %s\n", token);
                i = 0;
                }
            }
        }
        
    
    fclose(entrada);
    }