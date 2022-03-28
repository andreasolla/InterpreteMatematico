#include <stdio.h>
#include <string.h>

#include "analizador_sintactico.h"
#include "analizador_lexico.h"


void realizar_analisis()
{
    inicializar_analizador_lexico();

    comp_lexico *actual = sig_comp_lexico();

    while (actual->componente_lexico != LIMITE_EOF)
    {
        printf("<%d,%s>\n", actual->componente_lexico, actual->lexema);
        destruir_comp_lexico(actual);
        actual = sig_comp_lexico();
    }

    printf("<%d,%s>\n", actual->componente_lexico, actual->lexema);
    
    
    finalizar_analisis();
}

