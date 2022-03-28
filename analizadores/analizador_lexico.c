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
int _anade_punto_y_coma = 0;

int _isoperador(char caracter)
{
    return (caracter == '+') || (caracter == '-') || (caracter == '*') || (caracter == '/') || (caracter == '=') || (caracter == ':') || (caracter == '<');
}

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
            else if (caracterActual == '\\')
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
                // error de string mal formado
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
        _anade_punto_y_coma = 0;
        break;
    case ')':
        componente.componente_lexico = SEPARADOR_PAR_DER;
        _anade_punto_y_coma = 1;
        break;
    case '[':
        componente.componente_lexico = SEPARADOR_COR_IZQ;
        _anade_punto_y_coma = 0;
        break;
    case ']':
        componente.componente_lexico = SEPARADOR_COR_DER;
        _anade_punto_y_coma = 1;
        break;
    case '{':
        componente.componente_lexico = SEPARADOR_LLAVE_IZQ;
        _anade_punto_y_coma = 0;
        break;
    case '}':
        componente.componente_lexico = SEPARADOR_LLAVE_DER;
        _anade_punto_y_coma = 1;
        break;
    case ',':
        componente.componente_lexico = SEPARADOR_COMA;
        _anade_punto_y_coma = 0;
        break;
    case '.':
        componente.componente_lexico = SEPARADOR_PUNTO;
        _anade_punto_y_coma = 0;
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
                error(ERROR_DECIMAL_MAL_FORMADO);
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
                error(ERROR_OCTAL_MAL_FORMADO);
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
                error(ERROR_BINARIO_MAL_FORMADO);
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
                error(ERROR_BINARIO_MAL_FORMADO);
            }
            else
            {
                error(ERROR_BINARIO_MAL_FORMADO);
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
                error(ERROR_OCTAL_MAL_FORMADO);
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
                error(ERROR_OCTAL_MAL_FORMADO);
            }
            else
            {
                error(ERROR_OCTAL_MAL_FORMADO);
                estado = 8;
            }
            break;
        case 11:
            if (isdigit(caracterActual) || (('a' <= caracterActual) && (caracterActual <= 'f')) || (('A' <= caracterActual) && (caracterActual <= 'F')))
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
                error(ERROR_HEX_MAL_FORMADO);
            }
            break;
        case 12:
            if (isdigit(caracterActual) || (('a' <= caracterActual) && (caracterActual <= 'f')) || (('A' <= caracterActual) && (caracterActual <= 'F')))
            {
                estado = 20;
            }
            else
            {
                error(ERROR_HEX_MAL_FORMADO);
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
                error(ERROR_DECIMAL_MAL_FORMADO);
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
                error(ERROR_EXP_ERRONEO);
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
                error(ERROR_DECIMAL_MAL_FORMADO);
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
            if (isdigit(caracterActual) || (('a' <= caracterActual) && (caracterActual <= 'f')) || (('A' <= caracterActual) && (caracterActual <= 'F')) || (caracterActual == '+') || (caracterActual == '-'))
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
            else
            {
                finalizado = 1;
                componente.componente_lexico = INTEGER;
            }
            break;
        case 22:
            if (isdigit(caracterActual) || (('a' <= caracterActual) && (caracterActual <= 'f')) || (('A' <= caracterActual) && (caracterActual <= 'F')))
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
                error(ERROR_HEX_MAL_FORMADO);
            }
            break;
        case 23:
            if (isdigit(caracterActual) || (('a' <= caracterActual) && (caracterActual <= 'f')) || (('A' <= caracterActual) && (caracterActual <= 'F')))
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
                error(ERROR_HEX_MAL_FORMADO);
            }
            break;
        case 24:
            if (isdigit(caracterActual) || (('a' <= caracterActual) && (caracterActual <= 'f')) || (('A' <= caracterActual) && (caracterActual <= 'F')))
            {
                estado = 22;
            }
            else
            {
                estado = 22;
                error(ERROR_HEX_MAL_FORMADO);
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
                error(ERROR_EXP_ERRONEO);
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
                error(ERROR_HEX_MAL_FORMADO);
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
                // caracterActual = siguiente_caracter();
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
                //caracterActual = siguiente_caracter();
            }
        }
        fin_lexema(); // Como no debe cumplir el tamaño máximo, no necesito pedir el lexema y no debe dar error de tamaño excedido
    }
    return aux;
}

int _leer_lexema()
{
    size = 1;
    int pedir_lex = 1;

    if (isalpha(caracterActual) || caracterActual == '_')
    { // cadena alfanumerica
        _automata_alfanumerico();
        _anade_punto_y_coma = 1; // Si se inserta un \n despues de una cadena alfanumerica se añade ;
    }
    else if (caracterActual == '\"')
    { // strings
        _automata_strings();
        _anade_punto_y_coma = 1; // Si se inserta un \n despues de un string se añade ;
    }
    else if (isdigit(caracterActual) || caracterActual == '.')
    { // numeros
        _automata_numeros();
        _anade_punto_y_coma = 1; // Si se inserta un \n despues de un número se añade ;
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
        _anade_punto_y_coma = 0; // Si se inserta un \n despues de un operador no se añade ;
    }
    else if (caracterActual == ' ')
    { // ignoro los espacios
        fin_lexema();
        caracterActual = siguiente_caracter();
        pedir_lex = _leer_lexema(); // paso al siguiente lexema, este no me interesa
    }
    else if (caracterActual == '\n')
    {
        if (_anade_punto_y_coma)
        {
            _anade_punto_y_coma = 0;
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
        _anade_punto_y_coma = 0;
    }
    else if (caracterActual == ';')
    {
        componente.componente_lexico = SEPARADOR_PUNTOYCOMA;
        _anade_punto_y_coma = 0; // Si se ha añadido un punto y coma, despues no va otro
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

// Funciones públicas

void inicializar_analizador_lexico()
{
    iniciar_sistema_entrada();

    caracterActual = siguiente_caracter();
}

void finalizar_analisis()
{
    terminar();
}

comp_lexico *sig_comp_lexico()
{
    if(!fin_alcanzado()) {
        int copiar = _leer_lexema();
        size++; //Añado una posicion para \0

        componente.lexema = malloc(size*sizeof(char));

        if (copiar)
        {
            char *aux = pedir_lexema();
            strcpy(componente.lexema, aux);
            liberar_lexema(aux);
            //printf("no llego\n");
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
        
    }
    else {
        componente.componente_lexico = LIMITE_EOF;
        componente.lexema = "eof";
    }
    return &componente;
}

void destruir_comp_lexico(comp_lexico *cp) {
    free(cp->lexema);
}