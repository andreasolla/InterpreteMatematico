#ifndef tabla_simbolos_h
#define tabla_simbolos_h


#include "auxiliares/uthash.h"


#define LIBR 400

typedef struct numero{
        
        union{
            int entero;
            float flotante;
        }valor;
        char tipo; //'i' para integer y 'f' para float
        char constante; //'c' para constante y 'v' para variable

} numero;
typedef struct elemento_tabla
{
    char *id;
    int componente_lexico;
    union cont {
        numero valor;
        double (*funcion)();
        void *libreria;
    } cont;

    UT_hash_handle hh;
} elemento_tabla;


void crear_tabla();
void destruir_tabla();

void imprimir_tabla();
void imprimir_workspace();
void vaciar_workspace();
void eliminar_variable(char *nombre);

int buscar_lexema(char *lexema);
void anadir_variable(char *nombre, numero num);
void anadir_libreria(char *nombre);

numero obtener_valor(char *nombre);
elemento_tabla obtener_funcion(char *nombre);
int id_definido(char *nombre);

#endif