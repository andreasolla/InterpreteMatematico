#include "errores.h"

#include "stdio.h"


void error(char *msg, int numLinea){
    printf("\n  ╭─────╮\n");
    printf("╭─┤ERROR├───────────────────────────────────────\n");
    printf("│ ╰─────╯\n");
    printf("│  Línea: %d\n", numLinea);
    printf("│  %s\n", msg);
    printf("╰───────────────────────────────────────────────\n\n");
}