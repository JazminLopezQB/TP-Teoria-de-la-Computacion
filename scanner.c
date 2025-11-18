#include "scanner.h"

// ============================================================================
// Definición de Tabla y Caracteres
// ============================================================================

static int TT[4][15] = { // Tabla de Transición
/*                      L      D        :   =        ;         ,       *   /          +    -      (   )  ESP INV EOF */
/*0*/       {  1,  2,  3, 100, 14, 15, 16, 17, 18, 19, 20, 21,  0, 99, 22 },
/*1*/       {  1,  1, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11 },
/*2*/       { 99,  2, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 99, 12 },
/*3*/       { 100, 100, 100, 13, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100 }
};

enum { // Posibles caracteres
    C_LETRA = 0, // Letras
    C_DIGITO = 1, // Digitos
    C_DOSP = 2, // :
    C_IGUAL = 3, // =
    C_PUNTOYCOMA = 4, // ;
    C_COMA = 5, // ,
    C_MULT = 6, // *
    C_DIV = 7, // /
    C_MAS = 8, // +
    C_MENOS = 9, // -
    C_PAR_ABRE = 10, // (
    C_PAR_CIERRA = 11, // )
    C_ESP = 12, // Espacios y saltos de linea
    C_INV = 13, // Caracter Inválido
    C_EOF =14 // Fin de archivo
    };

/* ---------------------------------------------------------
    Funciones Auxiliares
   --------------------------------------------------------- */

FILE *abrir_archivo(char nombre[]) {
    FILE *archivo = fopen(nombre, "r");

    if (archivo == NULL) {
        printf("\nHubo un error al abrir el archivo\n");
        exit(-1);
        }
    return archivo;
    }

static int tipoCaracter(char c) {
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
    return C_INV; // Si no es ninguno de los anteriores, es un caracter inválido
    }

static TokenTipo tokensFinales(int est) {
    switch (est) {
        case 11: return IDENTIFICADOR; // Fin de Identificador
        case 12: return CONSTANTE; // Fin de Constante
        case 13: return ASIGNAR; // Asignación bien formada
        case 14: return PUNTOYCOMA; // ;
        case 15: return COMA; // ,
        case 16: return MULTIPLICAR; // *
        case 17: return DIV; // /
        case 18: return SUMA; // +
        case 19: return MENOS; // -
        case 20: return PAREN_ABR; // (
        case 21: return PAREN_CERR; // )
        case 22: return FIN; // EOF/FDT
        case 100: return ERROR_ASIGNACION;
        default: return ERROR_GENERAL; // Error
        }
    }

static Token crearToken(TokenTipo tipo, const char *lex) {
    Token t;
    t.tipo = tipo;
    strcpy(t.lexema, lex);
    return t;
    }

void imprimirToken(Token t) { // Mostrar los lexemas o errores según corresponda
    switch (t.tipo) {
        case FIN: // Hay que mostrar el identificador 'fin'
        case IDENTIFICADOR:  printf("Identificador '%s'\n",   t.lexema); break;
        case CONSTANTE:    printf("Constante '%s'\n",       t.lexema); break;
        case PUNTOYCOMA:  printf("Punto y coma ';'\n");    break;
        case COMA:        printf("Coma ','\n");            break;
        case PAREN_ABR:      printf("Paréntesis que abre '('\n"); break;
        case PAREN_CERR:      printf("Paréntesis que cierra ')'\n"); break;
        case SUMA:        printf("Más '+'\n");             break;
        case MENOS:       printf("Menos '-'\n");            break;
        case MULTIPLICAR:        printf("Multiplicación '*'\n");  break;
        case DIV:         printf("División '/'\n");        break;
        case ASIGNAR:       printf("Asignación ':='\n");     break;
        case ERROR_ASIGNACION: printf("Error en asignación, solo vino '%s'\n", t.lexema); break;
        case ERROR_GENERAL: printf("Error general '%s'\n", t.lexema); break;
        }
    }

// ============================================================================
// Scanner Principal
// ============================================================================
Token scanner(FILE *entrada) {

    char lex[LONGITUD]; // Lexema
    int p = 0; // Punto en el lexema
    int estado = 0; // Estado actual, empieza en el estado inicial 0
    int nuevo = 0; // Siguiente estado, si bien también empieza en 0 será el siguiente de estado
    int c, cl; // Caracter de entrada y su tipo
    
    while (1) {

        // Se lee la entrada y se determina que clase de caractér se recibió
        c = fgetc(entrada);
        cl = tipoCaracter(c);
        nuevo = TT[estado][cl]; // Obtener el estado al que se pasa según la tabla

        // Errores iniciales
        if (c == EOF && p == 0) { return crearToken(FIN, "Error, archivo vacio");}
        if (nuevo == 0 && estado == 0) continue; // Ignorar espacios antes de 'inicio'
        
        if ((nuevo == 11 || nuevo == 12 || (nuevo == 100 && estado == 3)) && c != EOF) { ungetc(c, entrada);} // Devolver caracter procesado de más
        else if (p < LONGITUD -1) lex[p++] = c; // Evitar agregar caracteres erroneos o consumir el error solo '='
        
        // Los valores de nuevo son discretos, si es > 4 es por que es un estado aceptor o error
        if (4 < nuevo || p == LONGITUD - 1 ) {
            lex[p] = '\0'; // Finalizar el lexema
            if (p == LONGITUD - 1) nuevo = (nuevo == 1) ? 11 : (nuevo == 2 ? 12 : 99);
            return crearToken(tokensFinales(nuevo), lex);
            }

        // Si nuevo siguen en un estado de trabajo/transición hay que continuar
        estado = nuevo;
        }
    }

