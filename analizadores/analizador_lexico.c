#include "analizador_lexico.h"

#include "../sistema_entrada.h"
#include "../auxiliares/tabla_simbolos.h"
#include "../auxiliares/errores.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char caracterActual;
int size;
comp_lexico componente;
int anade_punto_y_coma;
int numLinea;
int fin;


//---------------------------------- Funciones privadas

// Función que devuelve 1 si el caracter es un operador
int _isoperador(char caracter)
{
    return (caracter == '+') || (caracter == '-') || (caracter == '*') || (caracter == '/') || (caracter == '=') || (caracter == ':') || (caracter == '<');
}

// Autómatas para el procesado de lexemas

void _automata_alfanumerico()
{
    caracterActual = siguiente_caracter();
    componente.componente_lexico = ID;

    while (isalnum(caracterActual) ||
           caracterActual == '_')
    {
        caracterActual = siguiente_caracter();
        size++;
    }

    /*Este autómata reconoce cuando llega algún caracter que
    no forma parte de la cadena, por lo que hay que retroceder
    una posición al finalizar para no perder ese caracter*/
    retroceder();
}

void _automata_strings()
{
    int estado = 1;
    componente.componente_lexico = STRING;

    while (estado != 3)
    {
        caracterActual = siguiente_caracter();

        switch (estado)
        {
        case 1:
            if ((caracterActual != '"') && (caracterActual != '\\'))
            {
                estado = 1;
            }
            else if (caracterActual == '\\') // caracter de escape
            {
                estado = 2;
            }
            else
            {
                estado = 3;
            }
            break;
        case 2:
            if (caracterActual != '"')
            {
                error(ERROR_STRING_MAL_FORMADO, numLinea);
            }
            estado = 1;
            break;
        default:
            break;
        }

        size++;
    }
}

void _automata_operadores()
{
    int estado = 0;

    switch (caracterActual)
    {
    case '+':
        estado = 1;
        componente.componente_lexico = OPERADOR_SUMA;
        break;
    case '-':
        estado = 5;
        componente.componente_lexico = OPERADOR_RESTA;
        break;
    case '*':
        estado = 5;
        componente.componente_lexico = OPERADOR_MULT;
        break;
    case '/':
        estado = 5;
        componente.componente_lexico = OPERADOR_DIV;
        break;
    case '=':
        estado = 5;
        componente.componente_lexico = OPERADOR_ASIGNACION;
        break;
    case ':':
        estado = 3;
        componente.componente_lexico = OPERADOR_RANGO;
        break;
    case '<':
        estado = 4;
        componente.componente_lexico = OPERADOR_MENOR;
        break;
    default:
        break;
    }

    caracterActual = siguiente_caracter();

    switch (estado)
    {
    case 1:
        if (caracterActual == '=')
        {
            estado = 2;
            componente.componente_lexico = OPERADOR_MAS_IGUAL;
        }
        else
        {
            estado = 5;
        }
        break;
    case 3:
        if (caracterActual == '=')
        {
            estado = 2;
            componente.componente_lexico = OPERADOR_INICIALIZACION;
        }
        else
        {
            estado = 5;
        }
        break;
    case 4:
        if (caracterActual == '-')
        {
            estado = 2;
            componente.componente_lexico = OPERADOR_ENVIO;
        }
        else
        {
            estado = 5;
        }
        break;
    default:
        break;
    }

    if (estado == 5)
    {
        retroceder();
    }
    else
    {
        size++;
    }
}

void _automata_separadores()
{
    switch (caracterActual)
    {
    case '(':
        componente.componente_lexico = SEPARADOR_PAR_IZQ;
        anade_punto_y_coma = 0;
        break;
    case ')':
        componente.componente_lexico = SEPARADOR_PAR_DER;
        anade_punto_y_coma = 1;
        break;
    case '[':
        componente.componente_lexico = SEPARADOR_COR_IZQ;
        anade_punto_y_coma = 0;
        break;
    case ']':
        componente.componente_lexico = SEPARADOR_COR_DER;
        anade_punto_y_coma = 1;
        break;
    case '{':
        componente.componente_lexico = SEPARADOR_LLAVE_IZQ;
        anade_punto_y_coma = 0;
        break;
    case '}':
        componente.componente_lexico = SEPARADOR_LLAVE_DER;
        anade_punto_y_coma = 1;
        break;
    case ',':
        componente.componente_lexico = SEPARADOR_COMA;
        anade_punto_y_coma = 0;
        break;
    case '.':
        componente.componente_lexico = SEPARADOR_PUNTO;
        anade_punto_y_coma = 0;
        break;
    default:
        break;
    }
}

void _automata_numeros()
{
    int estado = 0, finalizado = 0, retroceso = 1;

    while (!finalizado)
    {
        switch (estado)
        {
        case 0:
            // el primer caracter es un numero del 0 al 9 o un .
            if (caracterActual == '0')
            {
                estado = 4;
            }
            else if (caracterActual == '.')
            {
                estado = 19;
            }
            else
            { // 1-9
                estado = 1;
            }
            break;
        case 1:
            if (isdigit(caracterActual))
            {
                estado = 1;
            }
            else if (caracterActual == '_')
            {
                estado = 2;
            }
            else if (caracterActual == '.')
            {
                estado = 14;
            }
            else if (caracterActual == 'i')
            {
                finalizado = 1;
                componente.componente_lexico = COMPLEX;
                retroceso = 0;
            }
            else if ((caracterActual == 'e') || (caracterActual == 'E'))
            {
                estado = 16;
            }
            else
            {
                finalizado = 1;
                componente.componente_lexico = INTEGER;
            }
            break;
        case 2:
            if (isdigit(caracterActual))
            {
                estado = 1;
            }
            else
            {
                error(ERROR_DECIMAL_MAL_FORMADO, numLinea);
                estado = 1;
            }
            break;
        case 4:
            if ((caracterActual == 'b') || (caracterActual == 'B'))
            {
                estado = 5;
            }
            else if ((isdigit(caracterActual) && caracterActual < '8') || (caracterActual == 'o') || (caracterActual == 'O'))
            {
                estado = 8;
            }
            else if (isdigit(caracterActual))
            { // num > 7
                estado = 8;
                error(ERROR_OCTAL_MAL_FORMADO, numLinea);
            }
            else if (caracterActual == '_')
            {
                estado = 9;
            }
            else if ((caracterActual == 'x') || (caracterActual == 'X'))
            {
                estado = 11;
            }
            else
            {
                finalizado = 1;
                componente.componente_lexico = INTEGER;
            }
            break;
        case 5:
            if ((caracterActual == '0') || (caracterActual == '1'))
            {
                estado = 5;
            }
            else if (caracterActual == '_')
            {
                estado = 6;
            }
            else if (isdigit(caracterActual))
            {
                estado = 5;
                error(ERROR_BINARIO_MAL_FORMADO, numLinea);
            }
            else if (caracterActual == 'i')
            {
                finalizado = 1;
                componente.componente_lexico = COMPLEX;
                retroceso = 0;
            }
            else
            {
                finalizado = 1;
                componente.componente_lexico = INTEGER;
            }
            break;
        case 6:
            if ((caracterActual == '0') || (caracterActual == '1'))
            {
                estado = 5;
            }
            else if (isdigit(caracterActual))
            {
                estado = 5;
                error(ERROR_BINARIO_MAL_FORMADO, numLinea);
            }
            else
            {
                error(ERROR_BINARIO_MAL_FORMADO, numLinea);
                estado = 5;
            }
            break;
        case 8:
            if (isdigit(caracterActual) && caracterActual < '8')
            {
                estado = 8;
            }
            else if (caracterActual == '_')
            {
                estado = 9;
            }
            else if (isdigit(caracterActual))
            {
                estado = 8;
                error(ERROR_OCTAL_MAL_FORMADO, numLinea);
            }
            else if (caracterActual == 'i')
            {
                finalizado = 1;
                componente.componente_lexico = COMPLEX;
                retroceso = 0;
            }
            else
            {
                finalizado = 1;
                componente.componente_lexico = INTEGER;
            }
            break;
        case 9:
            if (isdigit(caracterActual) && caracterActual < '8')
            {
                estado = 8;
            }
            else if (isdigit(caracterActual))
            {
                estado = 8;
                error(ERROR_OCTAL_MAL_FORMADO, numLinea);
            }
            else
            {
                error(ERROR_OCTAL_MAL_FORMADO, numLinea);
                estado = 8;
            }
            break;
        case 11:
            if (isxdigit(caracterActual))
            {
                estado = 20;
            }
            else if (caracterActual == '_')
            {
                estado = 12;
            }
            else if (caracterActual == '.')
            {
                estado = 23;
            }
            else
            {
                estado = 20;
                error(ERROR_HEX_MAL_FORMADO, numLinea);
            }
            break;
        case 12:
            if (isxdigit(caracterActual))
            {
                estado = 20;
            }
            else
            {
                error(ERROR_HEX_MAL_FORMADO, numLinea);
                estado = 20;
            }
            break;
        case 14:
            if (isdigit(caracterActual))
            {
                estado = 14;
            }
            else if (caracterActual == '_')
            {
                estado = 15;
            }
            else if ((caracterActual == 'e') || (caracterActual == 'E'))
            {
                estado = 16;
            }
            else
            {
                finalizado = 1;
                componente.componente_lexico = FLOAT;
            }
            break;
        case 15:
            if (isdigit(caracterActual))
            {
                estado = 14;
            }
            else
            {
                error(ERROR_DECIMAL_MAL_FORMADO, numLinea);
                estado = 14;
            }
            break;
        case 16:
            if (isdigit(caracterActual) || (caracterActual == '+') || (caracterActual == '-'))
            {
                estado = 17;
            }
            else
            {
                estado = 17;
                error(ERROR_EXP_ERRONEO, numLinea);
            }
            break;
        case 17:
            if (isdigit(caracterActual))
            {
                estado = 17;
            }
            else if (caracterActual == '_')
            {
                estado = 18;
            }
            else if (caracterActual == 'i')
            {
                finalizado = 1;
                componente.componente_lexico = COMPLEX;
                retroceso = 0;
            }
            else
            {
                finalizado = 1;
                componente.componente_lexico = FLOAT;
            }
            break;
        case 18:
            if (isdigit(caracterActual))
            {
                estado = 17;
            }
            else
            {
                error(ERROR_DECIMAL_MAL_FORMADO, numLinea);
                estado = 17;
            }
            break;
        case 19:
            if (isdigit(caracterActual))
            {
                estado = 14;
            }
            else if (caracterActual == '_')
            {
                estado = 15;
            }
            else
            {
                retroceder();
                retroceder();
                caracterActual = siguiente_caracter();
                _automata_separadores();
                finalizado = 1;
                retroceso = 0;
            }
            break;
        case 20:
            if (isxdigit(caracterActual) || (caracterActual == '+') || (caracterActual == '-'))
            {
                estado = 20;
            }
            else if (caracterActual == '_')
            {
                estado = 12;
            }
            else if (caracterActual == '.')
            {
                estado = 22;
            }
            else if ((caracterActual == 'p') || (caracterActual == 'P'))
            {
                estado = 25;
            }
            else if (caracterActual == 'i')
            {
                finalizado = 1;
                componente.componente_lexico = COMPLEX;
                retroceso = 0;
            }
            else if (isalpha(caracterActual))
            {
                estado = 20;
                error(ERROR_HEX_MAL_FORMADO, numLinea);
            }
            else
            {
                finalizado = 1;
                componente.componente_lexico = INTEGER;
            }
            break;
        case 22:
            if (isxdigit(caracterActual))
            {
                estado = 22;
            }
            else if (caracterActual == '_')
            {
                estado = 24;
            }
            else if ((caracterActual == 'p') || (caracterActual == 'P'))
            {
                estado = 25;
            }
            else
            {
                estado = 22;
                error(ERROR_HEX_MAL_FORMADO, numLinea);
            }
            break;
        case 23:
            if (isxdigit(caracterActual))
            {
                estado = 22;
            }
            else if (caracterActual == '_')
            {
                estado = 24;
            }
            else
            {
                estado = 22;
                error(ERROR_HEX_MAL_FORMADO, numLinea);
            }
            break;
        case 24:
            if (isxdigit(caracterActual))
            {
                estado = 22;
            }
            else
            {
                estado = 22;
                error(ERROR_HEX_MAL_FORMADO, numLinea);
            }
            break;
        case 25:
            if (isdigit(caracterActual) || (caracterActual == '+') || (caracterActual == '-'))
            {
                estado = 26;
            }
            else
            {
                estado = 26;
                error(ERROR_EXP_ERRONEO, numLinea);
            }
            break;
        case 26:
            if (isdigit(caracterActual))
            {
                estado = 26;
            }
            else if (caracterActual == '_')
            {
                estado = 27;
            }
            else if (caracterActual == 'i')
            {
                finalizado = 1;
                componente.componente_lexico = COMPLEX;
                retroceso = 0;
            }
            else
            {
                finalizado = 1;
                componente.componente_lexico = FLOAT;
            }
            break;
        case 27:
            if (isdigit(caracterActual))
            {
                estado = 26;
            }
            else
            {
                estado = 26;
                error(ERROR_HEX_MAL_FORMADO, numLinea);
            }
            break;
        default:
            break;
        }
        if (!finalizado)
        {
            caracterActual = siguiente_caracter();
            size++;
        }
    }

    if (retroceso)
    {
        retroceder();
    }
}

int _automata_comentarios()
{
    int aux = 1;
    int estado = 0;

    caracterActual = siguiente_caracter();

    if (caracterActual == '*')
    {
        estado = 1;
    }
    else if (caracterActual == '/')
    {
        estado = 4;
    }
    else
    {
        retroceder(); // Estamos en el caracter siguiente a la /, por lo que debemos restroceder y tratarlo como un operador
        retroceder();
        caracterActual = siguiente_caracter();
        _automata_operadores();
        estado = 3;
        aux = 0;
    }

    while (estado != 3)
    {
        switch (estado)
        {
        case 1:
            if (caracterActual != '*')
            {
                caracterActual = siguiente_caracter();
            }
            else
            {
                estado = 2;
                caracterActual = siguiente_caracter();
            }
            break;

        case 2:
            if (caracterActual != '/')
            {
                estado = 1;
                caracterActual = siguiente_caracter();
            }
            else
            {
                estado = 3;
            }
            break;
        case 4:
            if (caracterActual != '\n')
            {
                caracterActual = siguiente_caracter();
            }
            else
            {
                estado = 3;
                retroceder();
            }
        }
        /*Si es un comentario de línea, no aumento el contador ya que al retroceder 
        se va a procesar el salto de línea y ya se va a contar*/
        if((estado != 3) && (estado != 4) && (caracterActual == '\n')) 
        {
            numLinea++;
        }
        fin_lexema(); // Como no debe cumplir el tamaño máximo, no necesito pedir el lexema y no debe dar error de tamaño excedido
    }
    return aux;
}

// Función que, a partir del primer caracter del lexema, dirige el procesado del mismo hacia el autómata correspondiente
int _leer_lexema()
{
    size = 1;
    int pedir_lex = 1;

    if (isalpha(caracterActual) || caracterActual == '_')
    { // cadena alfanumerica
        _automata_alfanumerico();
        anade_punto_y_coma = 1; // Si se inserta un \n despues de una cadena alfanumerica se añade ;
    }
    else if (caracterActual == '\"')
    { // strings
        _automata_strings();
        anade_punto_y_coma = 1; // Si se inserta un \n despues de un string se añade ;
    }
    else if (isdigit(caracterActual) || caracterActual == '.')
    { // numeros
        _automata_numeros();
        anade_punto_y_coma = 1; // Si se inserta un \n despues de un número se añade ;
    }
    else if (caracterActual == '/')
    { // comentarios
        if (_automata_comentarios() == 1)
        {
            fin_lexema();
            caracterActual = siguiente_caracter();
            pedir_lex = _leer_lexema(); // paso al siguiente lexema, este no me interesa
        }
    }
    else if (_isoperador(caracterActual))
    { // operadores
        _automata_operadores();
        anade_punto_y_coma = 0; // Si se inserta un \n despues de un operador no se añade ;
    }
    else if (caracterActual == ' ')
    { // ignoro los espacios
        fin_lexema();
        caracterActual = siguiente_caracter();
        pedir_lex = _leer_lexema(); // paso al siguiente lexema, este no me interesa
    }
    else if (caracterActual == '\n')
    {
        numLinea++;
        
        if (anade_punto_y_coma)
        {
            anade_punto_y_coma = 0;
            componente.componente_lexico = SEPARADOR_PUNTOYCOMA;
            pedir_lex = 0;
        }
        else
        {
            fin_lexema();
            caracterActual = siguiente_caracter();
            _leer_lexema(); // paso al siguiente lexema, este no me interesa
        }
        // Despues de un \n no va otro ;
        anade_punto_y_coma = 0;
        
    }
    else if (caracterActual == ';')
    {
        componente.componente_lexico = SEPARADOR_PUNTOYCOMA;
        anade_punto_y_coma = 0; // Si se ha añadido un punto y coma, despues no va otro
    }
    else if (caracterActual == EOF)
    {
        componente.componente_lexico = LIMITE_EOF;
    }
    else
    {
        _automata_separadores(); // Depende del operador que despues vaya o no punto y coma
    }

    return pedir_lex;
}


//---------------------------------- Funciones públicas

// Función que se llama al inicio, para inicializar los elementos necesarios
void inicializar_analizador_lexico()
{
    iniciar_sistema_entrada();

    caracterActual = siguiente_caracter();

    anade_punto_y_coma = 0;
    numLinea = 1;
    fin = 0;
}

// Función que se llama una vez finalizado el análisis léxico, para cerrar y liberar lo necesario
void finalizar_analisis()
{
    terminar();
}

// Función que devuelve cada componente léxico
comp_lexico *sig_comp_lexico()
{
    if(fin_alcanzado()) {
        fin = 1;
    }

    int copiar = _leer_lexema();
    size++; // Añado una posicion para \0

    componente.lexema = malloc(size * sizeof(char));

    if (copiar)
    {
        char *aux = pedir_lexema();
        strcpy(componente.lexema, aux);
        liberar_lexema(aux);
    }
    else
    {

        strcpy(componente.lexema, ";");
    }

    if (componente.componente_lexico == ID)
    {
        componente.componente_lexico = buscar_lexema(componente.lexema);
    }

    fin_lexema();
    caracterActual = siguiente_caracter();

    
    return &componente;
}

// Función que se llama tras sig_comp_lexico(), para liberar la memoria reservada para el lexema
void destruir_comp_lexico(comp_lexico *cp)
{
    free(cp->lexema);
}

//Función que comprueba si se ha alcanzado el fin de fichero
int finalizar() {
    return fin;
}

int lineaActual() {
    return numLinea;
}