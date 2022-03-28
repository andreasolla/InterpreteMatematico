#include "tabla_simbolos.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "definiciones.h"


elemento_tabla *tabla_simbolos;


void anadir(char *cadena, int comp_lexico) {
    elemento_tabla *el;

    el = malloc(sizeof(elemento_tabla));
    el->id = malloc((strlen(cadena)+1)*sizeof(char));

    el->componente_lexico = comp_lexico;
    strcpy(el->id, cadena);

    HASH_ADD_KEYPTR(hh, tabla_simbolos, el->id, strlen(el->id), el);
}

//Si devuelve NULL el elemento no está, si no, devuelve el elemento
elemento_tabla *buscar(char *cadena) {
    elemento_tabla *el;
    HASH_FIND_STR(tabla_simbolos, cadena, el);

    return el;
}


//Funciones publicas

void crear_tabla()
{
    tabla_simbolos = NULL;
    int i;

    //Introduzco las palabras claves
    for(i = 0; i < 8; i++) {
        anadir(kw_lexemas[i], kw_comp_lexicos[i]); 
    }
}

// devuelve el código del identificador o la palabra clave
int buscar_lexema(char *lexema)
{
    int codigo;
    elemento_tabla *el = buscar(lexema);

    if(el != NULL) {
        codigo = el->componente_lexico;
    } else{
        anadir(lexema, ID); //Si no esta en la tabla, es un identificador
        codigo = ID;
    }

    return codigo;
}

void destruir_tabla()
{
    elemento_tabla *iterador, *tmp;

    HASH_ITER(hh, tabla_simbolos, iterador, tmp) {
        HASH_DEL(tabla_simbolos, iterador);
        free(iterador->id);
        free(iterador);
    }
}

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