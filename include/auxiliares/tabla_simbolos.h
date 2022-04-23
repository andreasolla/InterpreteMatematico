#ifndef tabla_simbolos_h
#define tabla_simbolos_h


#include "auxiliares/uthash.h"


typedef struct elemento_tabla
{
    char *id;
    int componente_lexico;
    union cont {
        double valor;
        double (*funcion)();
    } cont;

    UT_hash_handle hh;
} elemento_tabla;


void crear_tabla();
int buscar_lexema(char *lexema);
void destruir_tabla();
void imprimir_tabla();
void anadir_variable(char *nombre, float valor);
float obtenerValor(char *nombre);

#endif