#include "scanner.h"

int main() {
    FILE *entrada = abrir_archivo("../entrada.txt");
    int modo_activo = 0;
    char *token = NULL;

    do {
        token = scanner(entrada);
        if (strcmp(token, "inicio") == 0) {modo_activo = 1;}
        if (modo_activo) {mostrar_identificador(token, es_constante(token));}
        }while (strcmp(token, "fin") != 0);

    fclose(entrada);
    return 0;
    }
