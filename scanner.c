#include "scanner.h"

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

void mostrar_identificador(char token[], int tipo) {
    if (token[0] == '\0') return;
    if (tipo)
        printf("Constante '%s'\n", token);
    else {
        if (strlen(token) == 1){
            switch (token[0]) {
                case ';': printf("Punto y coma ';'\n"); return;
                case ',': printf("Coma ','\n"); return;
                case '*': printf("Multiplicación '*'\n"); return;
                case '/': printf("División '/'\n"); return;
                case '+': printf("Más '+'\n"); return;
                case '-': printf("Menos '-'\n"); return;
                case '(': printf("Paréntesis que abre '('\n"); return;
                case ')': printf("Paréntesis que cierra ')'\n"); return;
                case '=':
                case ':': printf("Error en asignación solo vino '%s'\n", token); return;
                default: break;
                }
            }
        else if (strcmp(token, ":=") == 0){
            printf("Asignación '%s'\n", token);
            return;
            }
        else if(!isalpha(token[0]) && !es_caracter_valido(token[0])){
            printf("Error general '%s'\n", token);
            return;
            }
        else if (strcmp(token, "EOF") == 0){
            printf("Error, no hay un programa valido.\n");
            return;
            }
        else
            printf("Identificador '%s'\n", token);
        }
    }

/* ---------------------------------------------------------
    SCANNER PRINCIPAL
   --------------------------------------------------------- */
char * scanner(FILE *entrada) {

    static char token[LONGITUD];
    token[0] = '\0';
    int ti = 0;
    int c;

    while ((c = fgetc(entrada)) != EOF) {
        if (!es_separador_especial(c)){
            if (isalnum(c) || !es_caracter_valido(c)) {
                token[ti++] = c;
                if(isalnum(c)){
                    while ((c = fgetc(entrada)) != EOF && isalnum(c)) {
                        if (ti < LONGITUD - 1) token[ti++] = c;
                        }
                    }
                else if (!es_caracter_valido(c)){
                    while ((c = fgetc(entrada)) != EOF && !es_caracter_valido(c) && !isalnum(c)) {
                        if (ti < (int)sizeof(token)-1) token[ti++] = c;
                        }
                    }
                token[ti] = '\0';
                if (c != EOF) ungetc(c, entrada);
                return token;
                }
            else if (es_separador_simple(c) || c == '=') {
                token[0] = c;
                token[1] = '\0';
                return  token;
                }
            else if (c == ':') {
                int c = fgetc(entrada);
                if (c == '=') {
                    token[0] = ':';
                    token[1] = '=';
                    token[2] = '\0';
                    }
                else {
                    token[0] = ':';
                    token[1] = '\0';
                    if (c != EOF) ungetc(c, entrada);
                    }
                return token;
                }
            }
        }
    return "EOF";
    }
