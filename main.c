#include <stdio.h>
#include "scanner.h"

int main(int argc, char *argv[]) {

    FILE *f = stdin;

    if (argc >= 2) {
        f = fopen(argv[1], "r");
        if (!f) {
            perror("No se puede abrir archivo");
            return 1;
        }
    }

    iniciarScanner(f);

    Token t;
    do {
        t = proxToken();
        imprimirToken(t);
    } while (t.tipo != TOK_FIN);

    if (f != stdin) fclose(f);

    return 0;
}
