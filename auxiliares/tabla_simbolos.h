#ifndef tabla_simbolos_h
#define tabla_simbolos_h


#include "uthash.h"


typedef struct elemento_tabla
{
    char *id;
    int componente_lexico;
    UT_hash_handle hh;
} elemento_tabla;


void crear_tabla();
int buscar_lexema(char *lexema); //devuelve el código del identificador o la palabra clave
void destruir_tabla();
void imprimir_tabla();

#endif