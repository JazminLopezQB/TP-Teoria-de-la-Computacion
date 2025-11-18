#include "scanner.h"

int main() {
    FILE *f = abrir_archivo("../entrada.txt");
    Token t = scanner(f);
    int modo_activo = 0; // Asegurarse de ignorar todo lo que venga antes de 'inicio'
    
    do { // Mientras no se reciba 'fin' el scanner analizará la entrada y se mostrará el resultado
        if (strcmp(t.lexema, "inicio") == 0) modo_activo = 1;
        if (modo_activo) imprimirToken(t);
        t = scanner(f);
        } while (t.tipo != FIN);

    fclose(f);

    return 0;
}
