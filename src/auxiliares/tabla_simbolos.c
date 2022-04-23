#include "auxiliares/tabla_simbolos.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "auxiliares/definiciones.h"


elemento_tabla *tabla_simbolos;


//---------------------------------- Funciones privadas

//Función para insertar elementos en la tabla
void _anadir(char *cadena, int comp_lexico) {
    elemento_tabla *el;

    el = malloc(sizeof(elemento_tabla));
    el->id = malloc((strlen(cadena)+1)*sizeof(char));

    el->componente_lexico = comp_lexico;
    strcpy(el->id, cadena);

    HASH_ADD_KEYPTR(hh, tabla_simbolos, el->id, strlen(el->id), el);
}

//Función que devuelve NULL si el elemento no está, si no, devuelve el elemento
elemento_tabla *_buscar(char *cadena) {
    elemento_tabla *el;
    HASH_FIND_STR(tabla_simbolos, cadena, el);

    return el;
}


//---------------------------------- Funciones publicas

//Función que crea la tabla y la inicializa con las keywords
void crear_tabla()
{
    tabla_simbolos = NULL;
    int i;

    //Introduzco las palabras claves
    // for(i = 0; i < 8; i++) {
    //     _anadir(kw_lexemas[i], kw_comp_lexicos[i]); 
    // }
}

//Función que devuelve el código del identificador o la palabra clave
int buscar_lexema(char *lexema)
{
    int codigo;
    /*elemento_tabla *el = _buscar(lexema);

    if(el != NULL) {
        codigo = el->componente_lexico;
    } else{
        _anadir(lexema, ID); //Si no esta en la tabla, es un identificador
        codigo = ID;
    }*/

    return codigo;
}

void anadir_variable(char *nombre, float valor)
{
    elemento_tabla *el = _buscar(nombre);

    if(el == NULL) {
        _anadir(nombre, ID);
        el = _buscar(nombre);
    }

    el->cont.valor = valor;
}

float obtenerValor(char *nombre)
{
    elemento_tabla *el = _buscar(nombre);

    if(el == NULL) {
        return 0;
    }

    return el->cont.valor;
}


//Función que vacía la tabla y libera la memoria
void destruir_tabla()
{
    elemento_tabla *iterador, *tmp;

    HASH_ITER(hh, tabla_simbolos, iterador, tmp) {
        HASH_DEL(tabla_simbolos, iterador);
        free(iterador->id);
        free(iterador);
    }
}

//Función para imprimir la tabla
void imprimir_tabla()
{
    elemento_tabla *iterador, *tmp;

    printf("╔═══════════════════════════════════════════════╗\n");
    printf("║\t\tTABLA DE SIMBOLOS\t\t║\n");
    printf("╟───────────────────────┬───────────────────────╢\n");
    printf("║\tlexema\t\t│\tcomponente\t║\n");
    printf("╟───────────────────────┼───────────────────────╢\n");
    HASH_ITER(hh, tabla_simbolos, iterador, tmp) {
        printf("║\t%s\t\t│\t%d\t\t║\n", iterador->id, iterador->componente_lexico);
    }
    printf("╚═══════════════════════╧═══════════════════════╝\n");
}