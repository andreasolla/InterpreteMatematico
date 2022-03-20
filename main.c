#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "sistema_entrada.h"
#include "analizadores/analizador_sintactico.h"


int main(int argc, char *argv[])
{
    char caracterActual;


    printf("Inicio \n");
    //preparar_cosillas();

    realizar_analisis();


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