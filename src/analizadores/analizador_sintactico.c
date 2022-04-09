#include <stdio.h>
#include <string.h>

#include "analizadores/analizador_sintactico.h"
#include "analizadores/analizador_lexico.h"



void realizar_analisis()
{
    inicializar_analizador_lexico();
    
    comp_lexico *actual; 
    
    
    //Empieza el análisis
    do {
        actual = sig_comp_lexico();
        imprimir_componente(*actual);
        //printf("<%d>\n", actual->componente_lexico); 
        //printf("<%d,%s>\n", actual->componente_lexico, actual->lexema); 
        destruir_comp_lexico(actual);
    } while (actual->componente_lexico != LIMITE_EOF);
    //Finaliza el análisis
    
    
    finalizar_analisis();
}