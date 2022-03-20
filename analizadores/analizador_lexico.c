#include "analizador_lexico.h"

#include "../sistema_entrada.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char *cadena_prueba = "package";
char caracterActual;
int size;
comp_lexico componente;

int isoperador(char caracter)
{
    return (caracter == '+') || (caracter == '-') || (caracter == '*') || (caracter == '/') || (caracter == '=') || (caracter == ':') || (caracter == '<');
}

void inicializar_analizador_lexico()
{
    iniciar_sistema_entrada();

    caracterActual = siguiente_caracter();
    printf("%c\n", caracterActual);
}

void finalizar_analisis()
{
    terminar();
}

void automata_alfanumerico()
{
    caracterActual = siguiente_caracter();
    componente.componente_lexico = ID;

    while (isalnum(caracterActual) ||
           caracterActual == '_')
    {
        // printf("%c", caracterActual);
        caracterActual = siguiente_caracter();
        size++;
    }

    retroceder();
}

void automata_strings()
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
            if(caracterActual != '"') {
                //error de string mal formado
            }
            estado = 1;
            break;
        default:
            break;
        }

        size++;
    }
}

void automata_numeros()
{
    int estado = 0, finalizado=0, retroceso = 1;

    while (!finalizado) {
        switch (estado)
        {
        case 0:
            if(caracterActual=='0') {
                estado = 4;
            } else if(caracterActual == '.'){ 
                estado = 14;
            } else { //el primer caracter es un numero del 0 al 9 o un .
                estado = 1;
            }
            break;
        case 1:
            if (isdigit(caracterActual))
            {
                estado = 1;
            } else if(caracterActual == '_') {
                estado = 2;
            } else if( caracterActual == '.') {
                estado = 14;
            } else if (caracterActual == 'i') {
                finalizado = 1;
                componente.componente_lexico = COMPLEX;
                retroceso = 0;
            } else if((caracterActual == 'e') || (caracterActual == 'E')) {
                estado = 16;
            } else {
                finalizado = 1;
                componente.componente_lexico = INTEGER;
            }
            break;
        case 2:
            if(isdigit(caracterActual)) {
                estado = 1;
            } else {
                //avisar de error
                estado = 1;
            }
            break;
        case 4:
            if((caracterActual == 'b') || (caracterActual == 'B')) {
                estado = 5;
            } else if((isdigit(caracterActual) && caracterActual < '8') || (caracterActual == 'o') || (caracterActual == 'O')) {
                estado = 8;
            } else if(caracterActual == '_') {
                estado = 9;
            } else if((caracterActual == 'x') || (caracterActual == 'X')) {
                estado = 11;
            } else {
                finalizado = 1;
                componente.componente_lexico = INTEGER;
            }
            break;
        case 5:
            if ((caracterActual == '0') || (caracterActual == '1'))
            {
                estado = 5;
            } else if(caracterActual == '_') {
                estado = 6;
            } else if(isdigit(caracterActual)) {
                estado = 5;
                //error binario mal formado
            } else if (caracterActual == 'i') {
                finalizado = 1;
                componente.componente_lexico = COMPLEX;
                retroceso = 0;
            } else {
                finalizado = 1;
                componente.componente_lexico = INTEGER;
            }
            break;
        case 6:
            if ((caracterActual == '0') || (caracterActual == '1'))
            {
                estado = 5;
            } else if(isdigit(caracterActual)) {
                estado = 5;
                //error binario mal formado
            } else {
                //avisar de error
                estado = 5;
            }
            break;
        case 8:
            if (isdigit(caracterActual) && caracterActual < '8') {
                estado = 8;
            } else if(caracterActual == '_') {
                estado = 9;
            } else if(isdigit(caracterActual)) {
                estado = 8;
                //error octal mal formado
            } else if (caracterActual == 'i') {
                finalizado = 1;
                componente.componente_lexico = COMPLEX;
                retroceso = 0;
            } else {
                finalizado=1;
                componente.componente_lexico = INTEGER;
            }
            break;
        case 9:
            if (isdigit(caracterActual) && caracterActual < '8')
            {
                estado = 8;
            } else if(isdigit(caracterActual)) {
                estado = 8;
                //error octal mal formado
            } else {
                //avisar de error
                estado = 8;
            }
            break;
        case 11:
            if(isdigit(caracterActual) || (('a' <= caracterActual) && (caracterActual <= 'f')) || (('A' <= caracterActual) && (caracterActual <= 'F'))) {
                estado = 20;
            } else if (caracterActual == '_') {
                estado = 12;
            } else if (caracterActual == '.'){
                estado = 23;
            }else {
                estado = 20;
                //error hex mal formado
            }
            break;
        case 12:
            if(isdigit(caracterActual) || (('a' <= caracterActual) && (caracterActual <= 'f')) || (('A' <= caracterActual) && (caracterActual <= 'F')))
            {
                estado = 20;
            } else {
                //avisar de error
                estado = 20;
            }
            break;
        case 14:
            if(isdigit(caracterActual)) {
                estado = 14;
            } else if (caracterActual == '_') {
                estado = 15;
            } else if((caracterActual == 'e') || (caracterActual == 'E')) {
                estado = 16;
            } else {
                finalizado = 1;
                componente.componente_lexico = FLOAT;
            }
            break;
        case 15:
            if(isdigit(caracterActual)) {
                estado = 14;
            } else {
                //avisar de error
                estado = 14;
            }
            break;
        case 16:
            if(isdigit(caracterActual) || (caracterActual == '+') || (caracterActual == '-')) {
                estado = 17;
            } else {
                estado = 17;
                //error
            }
            break;
        case 17:
            if(isdigit(caracterActual)) {
                estado = 17;
            } else if(caracterActual == '_') {
                estado = 18;
            } else if (caracterActual == 'i') {
                finalizado = 1;
                componente.componente_lexico = COMPLEX;
                retroceso = 0;
            } else {
                finalizado = 1;
                componente.componente_lexico = FLOAT; 
            }
            break;
        case 18:
            if(isdigit(caracterActual)) {
                estado = 17;
            } else {
                //avisar de error
                estado = 17;
            }
            break;
        case 20:
            if(isdigit(caracterActual) || (('a' <= caracterActual) && (caracterActual <= 'f')) || (('A' <= caracterActual) && (caracterActual <= 'F')) || (caracterActual == '+') || (caracterActual == '-')) {
                estado = 20;
            } else if (caracterActual == '_') {
                estado = 12;
            } else if (caracterActual == '.'){
                estado = 22;
            } else if ((caracterActual == 'p') || (caracterActual == 'P')){
                estado = 25;
            } else if (caracterActual == 'i') {
                finalizado = 1;
                componente.componente_lexico = COMPLEX;
                retroceso = 0;
            } else {
                finalizado = 1;
                componente.componente_lexico = INTEGER;
            }
            break;
        case 22:
            if(isdigit(caracterActual) || (('a' <= caracterActual) && (caracterActual <= 'f')) || (('A' <= caracterActual) && (caracterActual <= 'F'))) {
                estado = 22;
            } else if (caracterActual == '_') {
                estado = 24;
            } else if ((caracterActual == 'p') || (caracterActual == 'P')){
                estado = 25;
            } else {
                estado = 22;
                //error hex mal formado
            }
            break;
        case 23:
            if(isdigit(caracterActual) || (('a' <= caracterActual) && (caracterActual <= 'f')) || (('A' <= caracterActual) && (caracterActual <= 'F'))) {
                estado = 22;
            } else if (caracterActual == '_') {
                estado = 24;
            } else {
                estado = 22;
                //error hex mal formado
            }
            break;
        case 24:
            if(isdigit(caracterActual) || (('a' <= caracterActual) && (caracterActual <= 'f')) || (('A' <= caracterActual) && (caracterActual <= 'F'))) {
                estado = 22;
            } else {
                estado = 22;
                //error hex mal formado
            }
            break;
        case 25:
            if(isdigit(caracterActual) || (caracterActual == '+') || (caracterActual == '-')) {
                estado = 26;
            } else {
                estado = 26;
                //error exp mal formado
            }
            break;
        case 26:
            if(isdigit(caracterActual)) {
                estado = 26;
            } else if (caracterActual == '_') {
                estado = 27;
            } else if (caracterActual == 'i') {
                finalizado = 1;
                componente.componente_lexico = COMPLEX;
                retroceso = 0;
            } else {
                finalizado = 1;
                componente.componente_lexico = FLOAT;
            }
            break;
        case 27:
            if(isdigit(caracterActual)) {
                estado = 26;
            } else {
                estado = 26;
                //error hex mal formado
            }
            break;
        default:
            break;
        }
        if(!finalizado){
            caracterActual = siguiente_caracter();
        }
    }

    if(retroceso) {
        retroceder();
    }

}

void automata_operadores()
{
    int estado = 0;

    switch (caracterActual)
    {
    case '+':
        estado = 1;
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
            size++;
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
            size++;
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
            size++;
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
}

int automata_comentarios()
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
        automata_operadores();
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
            }
        }
    }
    return aux;
}

void automata_separadores()
{
    switch (caracterActual)
    {
    case '(':
        componente.componente_lexico = SEPARADOR_PAR_IZQ;
        break;
    case ')':
        componente.componente_lexico = SEPARADOR_PAR_DER;
        break;
    case '[':
        componente.componente_lexico = SEPARADOR_COR_IZQ;
        break;
    case ']':
        componente.componente_lexico = SEPARADOR_COR_DER;
        break;
    case '{':
        componente.componente_lexico = SEPARADOR_LLAVE_IZQ;
        break;
    case '}':
        componente.componente_lexico = SEPARADOR_LLAVE_DER;
        break;
    case ',':
        componente.componente_lexico = SEPARADOR_COMA;
        break;
    case '.':
        componente.componente_lexico = SEPARADOR_PUNTO;
        break;
    default:
        break;
    }
}

void leer_lexema()
{
    size = 1;
    // printf("%c\n", caracterActual);
    if (isalpha(caracterActual) || caracterActual == '_')
    { // cadena alfanumerica
        automata_alfanumerico();
    }
    else if (caracterActual == '\"')
    { // strings
        automata_strings();
    }
    else if (isdigit(caracterActual) || caracterActual == '.')
    { // numeros
        automata_numeros();
    }
    else if (caracterActual == '/')
    { // comentarios
        if (automata_comentarios() == 1)
        {
            fin_lexema();
            caracterActual = siguiente_caracter();
            leer_lexema(); // paso al siguiente lexema, este no me interesa
        }
    }
    else if (isoperador(caracterActual))
    { // operadores
        automata_operadores();
    }
    else if ((caracterActual == ' ') || (caracterActual == '\n'))
    { //ignoro los espacios y saltos de linea
        fin_lexema();
        caracterActual = siguiente_caracter();
        leer_lexema(); // paso al siguiente lexema, este no me interesa
    }
    else if (caracterActual == EOF)
    {
        printf("eof alcanzado\n");
        componente.componente_lexico = LIMITE_EOF;
    }
    else
    {
        automata_separadores();
    }
}

comp_lexico sig_comp_lexico()
{
    leer_lexema();

    componente.lexema = malloc(size);
    strcpy(componente.lexema, pedir_lexema());
    // componente.componente_lexico = 300; //Aqui tengo que ir a la tabla de simbolos o lo que sea
    // printf("%s\n", componente.lexema);

    if (caracterActual == EOF)
    {
        strcpy(componente.lexema, "EOF");
        printf("añadiendo eof\n");
    }

    fin_lexema();

    caracterActual = siguiente_caracter();

    return componente;
}
