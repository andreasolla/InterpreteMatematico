#include "sistema_entrada.h"

#include "auxiliares/errores.h"
#include "analizadores/analizador_lexico.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *codigo;

char *bufferA;
char *bufferB;
char *buffer_actual;
char *buffer_siguiente;
char *delantero, *inicio;
int siguiente_ya_cargado;
int error_ya_lanzado;
int fin_de_fichero;


//---------------------------------- Funciones privadas

void _cambiar_buffer()
{
    char *aux;

    // Intercambio los buffers
    aux = buffer_actual;
    buffer_actual = buffer_siguiente;
    buffer_siguiente = aux;
}

void _cargar_buffer()
{
    int elem_leidos;

    // Actualizo el buffer actual a buffer siguiente
    _cambiar_buffer();

    if (!siguiente_ya_cargado)
    { // compruebo si no se ha cargado ya por temas de retroceso
        elem_leidos = fread(buffer_actual, sizeof(char), TAM_BUFFER, codigo);

        // Si el fragmento leido no da para rellenar el buffer, se ha llegado al final del fichero
        if (elem_leidos != TAM_BUFFER)
        {
            buffer_actual[elem_leidos] = EOF;
        }
    }

    siguiente_ya_cargado = 0;
}

void _abrir_fichero()
{
    codigo = fopen("concurrentSum.go", "r"); // Abro el codigo

    if (codigo == NULL)
    { // comprobación de errores de apertura
        perror("Error al abrir el fichero\n");
        exit(EXIT_FAILURE);
    }
}

void _iniciar_buffer()
{
    bufferA = malloc((TAM_BUFFER + 1) * sizeof(char)); // añado 1 posicion para el centinela
    bufferB = malloc((TAM_BUFFER + 1) * sizeof(char));

    // Añado el centinela
    bufferA[TAM_BUFFER] = EOF;
    bufferB[TAM_BUFFER] = EOF;

    // El primer buffer a cargar será el A
    buffer_actual = bufferB;
    buffer_siguiente = bufferA;
    _cargar_buffer();

    // Inicializo los punteros de incio y delantero
    inicio = buffer_actual;
    delantero = buffer_actual;
    siguiente_ya_cargado = 0;
    error_ya_lanzado = 0;
    fin_de_fichero = 0;
}

void _incrementar_delantero()
{
    delantero++;

    if (*delantero == EOF)
    {

        // O bien final de buffer
        if (delantero == (buffer_actual + TAM_BUFFER))
        {
            _cargar_buffer(); // cargo el siguiente buffer
            delantero = buffer_actual;
        }
        else
        {
            fin_de_fichero = 1;
        }
    }
}

void _incrementar_inicio()
{
    inicio++;

    if (*inicio == EOF)
    {
        // Final de buffer
        inicio = buffer_actual;
    }
}

int _apuntan_distinto_buffer()
{
    if ((buffer_actual <= inicio) && (inicio <= (buffer_actual + TAM_BUFFER)))
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

int _tam_lexema()
{
    int size;

    if (_apuntan_distinto_buffer())
    {
        size = ((buffer_siguiente + TAM_BUFFER) - inicio) + (delantero - buffer_actual);
    }
    else
    {
        size = delantero - inicio;
    }

    return (size / sizeof(char));
}

int _tam_excedido()
{
    // Si la distancia entre inicio y delantero es mayor que el tamaño de un buffer, se excede el tamaño máximo de lexema
    if (_apuntan_distinto_buffer() && ((inicio - buffer_siguiente) <= (delantero - buffer_actual)))
    {
        return 1;
    }
    return 0;
}

//---------------------------------- Funciones públicas

void iniciar_sistema_entrada()
{
    _abrir_fichero();

    // Inicializo los buffers, cargo el primero e inicio los punteros a la posición inicial
    _iniciar_buffer();
}

char siguiente_caracter()
{
    char aux = *delantero;

    _incrementar_delantero();

    if (_tam_excedido())
    {
        _incrementar_inicio(); // avanzo inicio una posición
        // error de tamaño maximo
        if (!error_ya_lanzado)
        {
            error(ERROR_TAM_LEXEMA_EXCEDIDO, lineaActual());
            error_ya_lanzado = 1; // Marco el error para que no se vuelva a dar en el mismo lexema
        }
    }

    return aux;
}

char *pedir_lexema()
{
    int aux, tamano = _tam_lexema();
    char *cadena = malloc((tamano + 1) * sizeof(char));

    if (_apuntan_distinto_buffer())
    {
        aux = (buffer_siguiente + TAM_BUFFER) - inicio;
        memcpy(cadena, inicio, aux);
        memcpy((cadena + aux), buffer_actual, (delantero - buffer_actual));
    }
    else
    {
        memcpy(cadena, inicio, tamano * sizeof(char));
    }
    cadena[tamano] = '\0'; // añado el fin de cadena ya que memcpy no lo introduce

    return cadena;
}

void liberar_lexema(char *cadena)
{
    free(cadena);
}

void fin_lexema()
{
    inicio = delantero;
    error_ya_lanzado = 0;
}

void retroceder()
{
    // si no está en la primera posición del buffer actual, solo retrocedo
    if (delantero != buffer_actual)
    {
        delantero--;
    }
    else
    {                                               // si está en la primera posición, vuelvo a la última del anterior buffer
        _cambiar_buffer();                          // vuelvo al anterior buffer
        delantero = buffer_actual + TAM_BUFFER - 1; // sitúo delantero en la última posición
        siguiente_ya_cargado = 1;                   // activo la flag para no volver a cargar el siguiente buffer al avanzar
    }
}

void terminar()
{
    free(bufferA);
    free(bufferB);

    fclose(codigo);
}

int fin_alcanzado() {
    return fin_de_fichero;
}