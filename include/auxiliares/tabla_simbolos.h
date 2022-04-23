#ifndef tabla_simbolos_h
#define tabla_simbolos_h


#include "auxiliares/uthash.h"

typedef struct numero{
        
        union{
            int entero;
            float flotante;
        }valor;
        char tipo; //'i' para integer y 'f' para float

} numero;
typedef struct elemento_tabla
{
    char *id;
    int componente_lexico;
    union cont {
        numero valor;
        double (*funcion)();
    } cont;

    UT_hash_handle hh;
} elemento_tabla;


void crear_tabla();
int buscar_lexema(char *lexema);
void destruir_tabla();
void imprimir_tabla();
void anadir_variable(char *nombre, numero num);
numero obtener_valor(char *nombre);
elemento_tabla obtener_funcion(char *nombre);
int id_definido(char *nombre);

#endif