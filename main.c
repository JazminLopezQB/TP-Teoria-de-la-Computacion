#include "scanner.h"

int main() {
    FILE *f = abrir_archivo("../entrada.txt");
    Token t = scanner(f);
    
    do { // Mientras no se reciba 'fin' el scanner analizará la entrada y se mostrará el resultado
        imprimirToken(t);
        t = scanner(f);
        } while (t.tipo != FIN);

    fclose(f);

    return 0;
}
