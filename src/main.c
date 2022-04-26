#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "analizadores/analizador_sintactico.h"
#include "auxiliares/tabla_simbolos.h"


int main(int argc, char *argv[])
{
    crear_tabla();
    printf("Bienvenido...\n");
    printf("Para ayuda escribe 'help'\n");

    printf("\n        ╭──────────────────────────────────╮\n");
    //printf("╭─┤FINALIZANDO...   ┳━━┳ノ( º _ ºノ) │ \n");
    printf("        │ Bienvenido...\t\t\t   │\n");
    printf("        │ Si tienes dudas escribe 'help'   │\n");
    printf("        └──────────────────────────────────╯\n");
    printf("(╮°-°)╮╱\n\n");

    realizar_analisis();


    destruir_tabla();


    exit(EXIT_SUCCESS);
}