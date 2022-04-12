#include "auxiliares/errores.h"

#include <stdio.h>


void error(char *msg, int numLinea, int numCol){
    printf("\n  ╭─────╮\n");
    printf("╭─┤ERROR├───────────────────────────────────────\n");
    printf("│ ╰─────╯\n");
    printf("│  Línea: %d Columna: %d\n", numLinea, numCol);
    printf("│  %s\n", msg);
    printf("╰───────────────────────────────────────────────\n\n");
}