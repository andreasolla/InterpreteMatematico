#include "auxiliares/tabla_simbolos.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <dlfcn.h>

#include "auxiliares/definiciones.h"


elemento_tabla *tabla_simbolos;

// Funciones del workspace -> keywords
char *kw_lexemas[6] = {"help", "quit", "load", "workspace", "clear", "import"};
int kw_comp_lexicos[6] = {KW_HELP, KW_QUIT, KW_LOAD, KW_WORKSPACE, KW_CLEAR, KW_IMPORT};


void anadir_libreria(char *nombre);
int buscar_funcion(char *nombre);

//---------------------------------- Funciones privadas

// Función para insertar elementos en la tabla
void _anadir(char *cadena, int comp_lexico)
{
    elemento_tabla *el;

    el = malloc(sizeof(elemento_tabla));
    el->id = malloc((strlen(cadena) + 1) * sizeof(char));

    el->componente_lexico = comp_lexico;
    strcpy(el->id, cadena);

    HASH_ADD_KEYPTR(hh, tabla_simbolos, el->id, strlen(el->id), el);
}

void _anadir_elemento(elemento_tabla *el)
{
    HASH_ADD_KEYPTR(hh, tabla_simbolos, el->id, strlen(el->id), el);
}

// Función para insertar un puntero a una funcion de una libreria importada
void _anadir_funcion(void *handle, char *funcion)
{
    double (*nombre_func)();
    *(double **)(&nombre_func) = dlsym(handle, funcion);

    elemento_tabla *el;
    el = malloc(sizeof(elemento_tabla));
    el->id = malloc((strlen(funcion) + 1) * sizeof(char));

    strcpy(el->id, funcion);
    el->componente_lexico = FUNC;
    el->cont.funcion = nombre_func;

    _anadir_elemento(el);
}

//Función para añadir las constantes
void _anadir_constantes() {
    elemento_tabla *el;

    el = malloc(sizeof(elemento_tabla));
    el->id = malloc((strlen("pi") + 1) * sizeof(char));
    strcpy(el->id, "pi");
    el->componente_lexico = ID;
    el->cont.valor.tipo = 'f';
    el->cont.valor.valor.flotante = M_PI;
    el->cont.valor.constante = 'c';
    _anadir_elemento(el);

    el = malloc(sizeof(elemento_tabla));
    el->id = malloc((strlen("e") + 1) * sizeof(char));
    strcpy(el->id, "e");
    el->componente_lexico = ID;
    el->cont.valor.tipo = 'f';
    el->cont.valor.valor.flotante = M_E;
    el->cont.valor.constante = 'c';
    _anadir_elemento(el);
}

// Función que devuelve NULL si el elemento no está, si no, devuelve el elemento
elemento_tabla *_buscar(char *cadena)
{
    elemento_tabla *el;

    HASH_FIND_STR(tabla_simbolos, cadena, el);

    return el;
}

//---------------------------------- Funciones publicas

// Función que crea la tabla y la inicializa con las keywords
void crear_tabla()
{
    tabla_simbolos = NULL;
    int i;

    // Introduzco las palabras clave (funciones del workspace)
    for (i = 0; i < 6; i++)
    {
        _anadir(kw_lexemas[i], kw_comp_lexicos[i]);
    }

    _anadir_constantes();
}

// Función que vacía la tabla y libera la memoria
void destruir_tabla()
{
    elemento_tabla *iterador, *tmp;

    HASH_ITER(hh, tabla_simbolos, iterador, tmp)
    {
        HASH_DEL(tabla_simbolos, iterador);
        free(iterador->id);
        free(iterador);
    }
}

// Función para imprimir la tabla
void imprimir_tabla()
{
    elemento_tabla *iterador, *tmp;

    printf("╔═══════════════════════════════════════════════╗\n");
    printf("║\t\tTABLA DE SIMBOLOS\t\t║\n");
    printf("╟───────────────────────┬───────────────────────╢\n");
    printf("║\tlexema\t\t│\tcomponente\t║\n");
    printf("╟───────────────────────┼───────────────────────╢\n");
    HASH_ITER(hh, tabla_simbolos, iterador, tmp)
    {
        printf("║\t%s\t\t│\t%d\t\t║\n", iterador->id, iterador->componente_lexico);
    }
    printf("╚═══════════════════════╧═══════════════════════╝\n");
}

// Función que imprime la tabla de variables
void imprimir_workspace()
{
    elemento_tabla *iterador, *tmp;

    printf("╔═══════════════════════════════════════════════╗\n");
    printf("║\t\t    WORKSPACE\t\t\t║\n");
    printf("╟───────────────────────┬───────────────────────╢\n");
    printf("║\tvariable\t│\tvalor\t\t║\n");
    printf("╟───────────────────────┼───────────────────────╢\n");
    HASH_ITER(hh, tabla_simbolos, iterador, tmp)
    {
        if (iterador->componente_lexico == ID)
        {
            if (iterador->cont.valor.tipo == 'f')
            {
                printf("║\t%s\t\t│\t%f\t║\n", iterador->id, iterador->cont.valor.valor.flotante);
            }
            else
            {
                printf("║\t%s\t\t│\t%d\t\t║\n", iterador->id, iterador->cont.valor.valor.entero);
            }
        }
    }
    printf("╚═══════════════════════╧═══════════════════════╝\n");
}

// Función para eliminar las variables almacenadas en la TS
void vaciar_workspace()
{
    elemento_tabla *iterador, *tmp;

    HASH_ITER(hh, tabla_simbolos, iterador, tmp)
    {
        if (iterador->componente_lexico == ID)
        {
            HASH_DEL(tabla_simbolos, iterador);
            free(iterador->id);
            free(iterador);
        }
    }
}

// Función para eliminar una variable concreta almacenada en la TS
void eliminar_variable(char *nombre)
{
    elemento_tabla *el = _buscar(nombre);

    if (el != NULL && el->componente_lexico == ID)
    {
        HASH_DEL(tabla_simbolos, el);
        free(el->id);
        free(el);
    }
}

// Función que devuelve el código del identificador o la palabra clave
int buscar_lexema(char *lexema)
{
    int codigo;
    elemento_tabla *el = _buscar(lexema);

    if (el != NULL)
    {
        codigo = el->componente_lexico;
    }
    else if (buscar_funcion(lexema))
    {
        codigo = FUNC;
    }
    else
    {
        // Si no esta en la tabla, es un identificador
        codigo = ID;
    }

    return codigo;
}

// Funcion para añadir una variable
int anadir_variable(char *nombre, numero num)
{
    elemento_tabla *el = _buscar(nombre);

    if (el == NULL)
    {
        el = malloc(sizeof(elemento_tabla));
        el->id = malloc((strlen(nombre) + 1) * sizeof(char));

        strcpy(el->id, nombre);
        el->componente_lexico = ID;
        el->cont.valor = num;

        _anadir_elemento(el);
    } else if(el->cont.valor.constante=='c') {
        lanzar_error(ERROR_MODIFICAR_CONSTANTE);
        return 0;
    } else if(el->componente_lexico==ID) {
        el->cont.valor = num;
    } else {
        lanzar_error(ERROR_NO_VARIABLE);
        return 0;
    }
    
    return 1;
}

// Funcion para añadir una libreria importada
void anadir_libreria(char *nombre)
{
    void *l = dlopen(nombre, RTLD_LAZY);

    if (l == NULL)
    {
        lanzar_error(ERROR_LIBRERIA);
    }
    else
    {
        elemento_tabla *el;
        el = malloc(sizeof(elemento_tabla));
        el->id = malloc((strlen(nombre) + 1) * sizeof(char));

        strcpy(el->id, nombre);
        el->componente_lexico = LIBR;
        el->cont.libreria = l;

        _anadir_elemento(el);
    }
}

// Función para comprobar si existe una función con ese nombre en alguna de las librerias importadas
int buscar_funcion(char *nombre)
{
    elemento_tabla *iterador, *tmp;

    HASH_ITER(hh, tabla_simbolos, iterador, tmp)
    {
        if (iterador->componente_lexico == LIBR)
        {
            if (dlsym(iterador->cont.libreria, nombre) != NULL)
            {
                _anadir_funcion(iterador->cont.libreria, nombre);
                return 1;
            }
        }
    }

    return 0;
}

numero obtener_valor(char *nombre)
{
    elemento_tabla *el = _buscar(nombre);

    return el->cont.valor;
}

elemento_tabla obtener_funcion(char *nombre)
{
    elemento_tabla *el = _buscar(nombre);

    return *el;
}

// Función que comprueba si hay alguna variable con ese nombre en la TS
int id_definido(char *nombre)
{
    elemento_tabla *el = _buscar(nombre);

    if (el != NULL && el->componente_lexico ==ID)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
