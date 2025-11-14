#include "scanner.h"

FILE *abrir_arcivo(char nombre[]) {
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

void scanner() {

    FILE *entrada = abrir_arcivo("../entrada.txt");

    // Definir variables de análisis
    
    char token[LONGITUD]; // Se van guardando lo que se lee del archivo
    char separadores[] = " ;*/+-,=:()\n\t"; // Cosas que pueden provocar que se corte un identificador
    char errores[] = "@!^$"; // Caracteres que producen errores
    char caracter; // Entrada del archivo
    int i = 0; // Indice para escribir caracter en token

    while ((caracter = fgetc(entrada)) != EOF) { //TODO: Revisar por que strcmp(token, "fin\0") != 0 no anda
        // TODO: También habria que poner un condicional para que descarte cualquier cosa que venga antes de "inicio"
        if (strchr(separadores, caracter) != NULL || i >= LONGITUD - 1) { // Si encuentra un separador o no hay espacio
            if (i > 0){
                token[i] = '\0';
                if (es_constante(token))
                    printf("Constante: '%s'\n", token);
                else
                    printf("Identificador: '%s'\n", token);
                i = 0; // Reinicio el indice
            }
            }
        else {
            token[i] = caracter;
            i++;
            }
        switch (caracter) {
            case ';':
                printf("Punto y coma: '%c'\n", caracter);
                break;
            case ',':
                printf("Coma: '%c'\n", caracter);
                break;
            case '*':
                printf("Multiplicacion: '%c'\n", caracter);
                break;
            case '/':
                printf("Division: '%c'\n", caracter);
                break;
            case '+':
                printf("Suma: '%c'\n", caracter);
                break;
            case '-':
                printf("Resta: '%c'\n", caracter);
                break;
            case '(':
                printf("Parentesis que abre: '%c'\n", caracter);
                break;
            case ')':
                printf("Parentesis que cierra: '%c'\n", caracter);
                break;
            default:
                continue;
                break;
            }
        }

    fclose(entrada);
    }