#include "errores.h"

#include "stdio.h"


void error(char *msg){
    printf("\n╭─┤ERROR├───────────────────────────────────────\n");
    printf("│  %s\n", msg);
    printf("╰───────────────────────────────────────────────\n\n");
}