#include "scanner.h"

int modo_activo = 0;

FILE *abrir_archivo(char nombre[]) {
    FILE *archivo = fopen(nombre, "r");

    if (archivo == NULL) {
        printf("\nHubo un error al abrir el archivo\n");
        exit(-1);
    }
    return archivo;
}

int es_constante(char cadena[]){
    for (int i = 0; i < (int)strlen(cadena); i++){
        if (!isdigit((unsigned char)cadena[i])){
            return 0;
        }
    }
    return 1;
}

/* Helpers simples */
int es_separador_simple(int c) {
    return c==';' || c==',' || c=='*' || c=='/' || c=='+' || c=='-' || c=='(' || c==')';
}

int es_caracter_valido(int c) {
    if (isalpha(c) || isdigit(c)) return 1;
    if (es_separador_simple(c)) return 1;
    if (c==':' || c=='=' ) return 1;
    if (c==' ' || c=='\t' || c=='\n' || c=='\r') return 1;
    return 0;
}

void flush_token_ident_o_const(char token[]) {
    if (token[0] == '\0') return;
    if (es_constante(token))
        printf("Constante '%s'\n", token);
    else
        printf("Identificador '%s'\n", token);
    token[0] = '\0';
}

/* ---------------------------------------------------------
   SCANNER PRINCIPAL (versión final corregida)
   --------------------------------------------------------- */
void scanner(void) {

    FILE *entrada = abrir_archivo("../entrada.txt");

    char token[LONGITUD];
    token[0] = '\0';
    int ti = 0;

    int c;
    while ((c = fgetc(entrada)) != EOF) {

        /* ------------------ Ignorar espacios ------------------ */
        if (c == ' ' || c == '\t' || c == '\n' || c == '\r') {

            if (ti > 0) {
                token[ti] = '\0';

                /* Si es "inicio" lo activamos aunque venga donde venga */
                if (strcmp(token, "inicio") == 0) {
                    modo_activo = 1;
                    printf("Identificador 'inicio'\n");
                    ti = 0;
                    continue;
                }

                /* Si todavía no vimos "inicio", ignoramos cualquier token */
                if (!modo_activo) {
                    ti = 0;
                    continue;
                }

                /* Después de inicio procesamos normal */
                /* Nota: NO procesamos "fin" acá para evitar doble impresión */
                flush_token_ident_o_const(token);
                ti = 0;
            }
            continue;
        }

        /* -------------------- IDENTIFICADORES ------------------ */
        if (isalpha(c)) {

            token[0] = c;
            ti = 1;

            while ((c = fgetc(entrada)) != EOF && (isalpha(c) || isdigit(c))) {
                if (ti < LONGITUD - 1) token[ti++] = c;
            }
            token[ti] = '\0';

            /* Procesar inicio/fin SOLO aquí (token ya formado) */
            if (strcmp(token, "inicio") == 0) {
                modo_activo = 1;
                printf("Identificador 'inicio'\n");
                ti = 0;
                if (c != EOF) ungetc(c, entrada);
                continue;
            }

            if (strcmp(token, "fin") == 0 && modo_activo == 1) {
                printf("Identificador 'fin'\n");
                /* cerramos y salimos para no reimprimir fin en el flush final */
                fclose(entrada);
                return;
            }

            if (!modo_activo) {
                ti = 0;
                if (c != EOF) ungetc(c, entrada);
                continue;
            }

            /* Identificador normal */
            printf("Identificador '%s'\n", token);
            ti = 0;

            if (c != EOF) ungetc(c, entrada);
            continue;
        }

        /* --------------------- CONSTANTES ---------------------- */
        if (isdigit(c)) {
            token[0] = c; ti = 1;

            while ((c = fgetc(entrada)) != EOF && isdigit(c)) {
                if (ti < LONGITUD-1) token[ti++] = c;
            }
            token[ti] = '\0';

            /* antes de procesar, respetamos el modo_activo */
            if (!modo_activo) {
                ti = 0;
                if (c != EOF) ungetc(c, entrada);
                continue;
            }

            printf("Constante '%s'\n", token);
            ti = 0;

            if (c != EOF) ungetc(c, entrada);
            continue;
        }

        /* --------------- ASIGNACIÓN ':=' o errores ------------- */
        if (c == ':') {
            int nxt = fgetc(entrada);
            if (nxt == '=') {
                if (!modo_activo) { /* si no estamos activos ignoramos */
                    /* nada */
                } else {
                    printf("Asignación ':='\n");
                }
            }
            else {
                if (!modo_activo) {
                    /* ignorar ':' antes de inicio */
                } else {
                    printf("Error en asignación solo vino ':'\n");
                }
                if (nxt != EOF) ungetc(nxt, entrada);
            }
            continue;
        }

        if (c == '=') {
            if (modo_activo) printf("Error en asignación solo vino '='\n");
            continue;
        }

        /* ---------------- TOKENS DE UN CARACTER ---------------- */
        if (es_separador_simple(c)) {
            if (!modo_activo) continue; /* ignorar antes de inicio */

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

        /* -------------- ERROR GENERAL (secuencia inválida) ------------- */
        if (!es_caracter_valido(c)) {
            /* si no estamos activos, debemos seguir consumiendo hasta inicio pero no imprimir errores */
            if (!modo_activo) {
                /* consumir la secuencia inválida, pero no informar */
                while ((c = fgetc(entrada)) != EOF && !es_caracter_valido(c)) {}
                if (c != EOF) ungetc(c, entrada);
                continue;
            }

            char errbuf[LONGITUD*2];
            int ei = 0;
            errbuf[ei++] = (char)c;

            while ((c = fgetc(entrada)) != EOF && !es_caracter_valido(c)) {
                if (ei < (int)sizeof(errbuf)-1) errbuf[ei++] = (char)c;
            }
            errbuf[ei] = '\0';

            printf("Error general '%s'\n", errbuf);

            if (c != EOF) ungetc(c, entrada);
            continue;
        }

        /* Si llegamos acá es un caracter válido que no fue manejado; lo ignoramos */
    }

    /* Si llegamos al EOF y quedó token pendiente, lo procesamos solo si estamos en modo activo */
    if (ti > 0 && modo_activo) {
        token[ti] = '\0';
        /* evitar imprimir "fin" aquí porque ya lo hubiéramos procesado en el bloque de identificadores */
        if (strcmp(token, "fin") != 0) flush_token_ident_o_const(token);
    }

    fclose(entrada);
}
