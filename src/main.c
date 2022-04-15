#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "analizadores/analizador_sintactico.h"
#include "auxiliares/tabla_simbolos.h"


int main(int argc, char *argv[])
{
    //crear_tabla();
    //imprimir_tabla();

    realizar_analisis();

    //imprimir_tabla();
    //destruir_tabla();


    exit(EXIT_SUCCESS);
}