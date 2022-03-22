#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "sistema_entrada.h"
#include "analizadores/analizador_sintactico.h"
#include "auxiliares/tabla_simbolos.h"


int main(int argc, char *argv[])
{
    char caracterActual;


    crear_tabla();
    imprimir_tabla();

    realizar_analisis();

    imprimir_tabla();
    destruir_tabla();

    // caracterActual = siguiente_caracter();
    // printf("%c\n", caracterActual);

    // while(caracterActual != EOF) {
    //     printf("%c", caracterActual);
    //     //usleep(1000);
    //     caracterActual = siguiente_caracter();
    // }
    // printf("\n");

    // terminar();


    exit(EXIT_SUCCESS);
}