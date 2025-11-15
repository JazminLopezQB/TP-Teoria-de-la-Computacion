#include "scanner.h"
#include <ctype.h>
#include <stdio.h>

/* ---------------------------------------------------------
    FUNCIONES AUXILIARES
   --------------------------------------------------------- */

FILE *abrir_archivo(char nombre[]) {
    FILE *archivo = fopen(nombre, "r");

    if (archivo == NULL) {
        printf("\nHubo un error al abrir el archivo\n");
        exit(-1);
        }
    return archivo;
    }

int es_constante(char cadena[]){
    for (int i = 0; i < strlen(cadena); i++){
        if (!isdigit(cadena[i])){
            return 0;
            }
        }
    return 1;
    }

int es_separador_simple(int c) {
    return c==';' || c==',' || c=='*' || c=='/' || c=='+' || c=='-' || c=='(' || c==')';
    }

int es_separador_especial(int c){
    return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\0';
    }

int es_caracter_valido(int c) {
    if (es_separador_simple(c) || es_separador_especial(c) || c==':' || c=='=') return 1;
    return 0;
    }

void mostrar_identificador(char token[], int *ti, int tipo) {
    if (token[0] == '\0') return;
    if (tipo == 1)
        printf("Constante '%s'\n", token);
    else {
        if(!isalpha(token[0]))
            printf("Error, identificador invalido '%s'\n", token);
        else
            printf("Identificador '%s'\n", token);
        }
    token[0] = '\0';
    *ti = 0;
    }

/* ---------------------------------------------------------
    SCANNER PRINCIPAL
   --------------------------------------------------------- */
void scanner() {

    FILE *entrada = abrir_archivo("../entrada.txt");

    char token[LONGITUD];
    token[0] = '\0';
    int ti = 0;
    int modo_activo = 0;

    int c;
    while ((c = fgetc(entrada)) != EOF) {

        if (!modo_activo){

            if (!isalpha(c)){continue;}
            
            token[0] = c;
            ti++;
            while ((c = fgetc(entrada)) != EOF && !es_caracter_valido(c)) {
                if (ti < LONGITUD - 1) token[ti++] = c;
                }
                token[ti] = '\0';
            
            if (strcmp(token, "inicio") == 0) {
                modo_activo = 1;
                mostrar_identificador(token, &ti, 0);
                continue;
                }
            ti = 0;
            if (c != EOF) ungetc(c, entrada);
            }
        else if (!es_separador_especial(c)){
            if (isalnum(c)) {
                token[0] = c;
                ti = 1;
                while ((c = fgetc(entrada)) != EOF && isalnum(c)) {
                    if (ti < LONGITUD - 1) token[ti++] = c;
                    }
                token[ti] = '\0';

                if (strcmp(token, "fin") == 0) {
                    mostrar_identificador(token, &ti, 0);
                    fclose(entrada);
                    return;
                    }
                mostrar_identificador(token, &ti, es_constante(token));

                if (c != EOF) ungetc(c, entrada);
                continue;
                }
            else if (c == '=') {
                printf("Error en asignación solo vino '='\n");
                continue;
                }
            else if (c == ':') {
                int c = fgetc(entrada);
                if (c == '=') { printf("Asignación ':='\n"); }
                else {
                    printf("Error en asignación solo vino ':'\n");
                    if (c != EOF) ungetc(c, entrada);
                    }
                continue;
                }
            else if (es_separador_simple(c)) {
                switch (c) {
                    case ';': printf("Punto y coma ';'\n"); break;
                    case ',': printf("Coma ','\n"); break;
                    case '*': printf("Multiplicación '*'\n"); break;
                    case '/': printf("División '/'\n"); break;
                    case '+': printf("Más '+'\n"); break;
                    case '-': printf("Menos '-'\n"); break;
                    case '(': printf("Paréntesis que abre '('\n"); break;
                    case ')': printf("Paréntesis que cierra ')'\n"); break;
                    }
                continue; 
                }
            else if (!es_caracter_valido(c) && !isalnum(c)){
                char errbuf[LONGITUD*2];
                int ei = 0;
                errbuf[ei++] = (char)c;

                while ((c = fgetc(entrada)) != EOF && !es_caracter_valido(c) && !isalnum(c)) {
                    if (ei < (int)sizeof(errbuf)-1) errbuf[ei++] = (char)c;
                    }
                errbuf[ei] = '\0';

                printf("Error general '%s'\n", errbuf);

                if (c != EOF) ungetc(c, entrada);
                continue;
                }
            }
        }

    printf("Error, no hay un identificador 'fin'.\n");
    fclose(entrada);
    }
