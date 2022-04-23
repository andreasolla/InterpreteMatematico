/*-----OPCIONES-----*/

/* fuerzo que se se genere un analizador de 8 bits */
%option 8bit

/*para que no intente leer otro fichero una vez llega a eof*/
%option noyywrap


/*-----DEFINICIONES-----*/

LETTER          [a-zA-Z_]
DECIMAL_DIGIT   [0-9]
ESPACIO         [[:blank:]]  


/*NUMEROS*/

NUM     {DECIMAL_DIGIT}+

EXP_DECIMAL     [eE][+-]?{DECIMAL_DIGIT}+


/*TIPOS DE DATOS*/

INT             {NUM}
FLOAT           {NUM}\.{NUM}{EXP_DECIMAL}?|{NUM}{EXP_DECIMAL}|\.{NUM}{EXP_DECIMAL}?

ID              {LETTER}({LETTER}|{DECIMAL_DIGIT})*


/*OPERADORES*/

IGUAL "="
SUMA "+"
RESTA "-"
MULT "*"
DIV "/"
EXP "^"
MODULO "%"


/*SEPARADORES*/

P_IZQUIERDO "("
P_DERECHO ")"


/*-----CABECERA DEL .c-----*/
%{
    #include "analizadores/analizador_lexico.h"
    #include "auxiliares/definiciones.h"
    #include "auxiliares/tabla_simbolos.h"
    #include "auxiliares/errores.h"

    #include <stdlib.h>
    #include <stdio.h>


    /*Reescribo el tamaño de buffer de flex a 4096 bytes*/
    #define YY_BUF_SIZE 4096
%}


/*-----REGLAS-----*/

%%


\n {
    return BLANCO; }


 /*OPERADORES*/

{SUMA} {
    return OPERADOR_SUMA;
}

{RESTA} {
    return OPERADOR_RESTA;
}

{MULT} {
    return OPERADOR_MULT; 
}

{DIV} {
    return OPERADOR_DIV;
}

{EXP} {
    return OPERADOR_EXP;   
}

 /*{MODULO} {
    return OPERADOR_MODULO;
}*/

{IGUAL} {
    return OPERADOR_IGUAL;
}

 /*SEPARADORES*/

{P_IZQUIERDO} {
    return SEPARADOR_PAR_IZQ;
}

{P_DERECHO} {
    return SEPARADOR_PAR_DER;   
}


 /*NUMEROS*/

{INT} {
    yylval.num.valor.entero = atoi(yytext);
    yylval.num.tipo = 'i';
    return INTEGER;
}

{FLOAT} {
    yylval.num.valor.flotante = atof(yytext); 
    yylval.num.tipo = 'f';
    return FLOAT;
    
}

 /*VARIABLES*/
{ID} {
    yylval.ptr = strdup(yytext);
    return buscar_elemento(yylval.ptr);
}


{ESPACIO} /*ignoro los espacios*/


%%

//Procedimiento para iniciar el análisis léxico
void inicializar_analizador_lexico()
{
    yyin = stdin;
}

//Procedimientos de liberación de memoria para la finalización del análisis léxico
void finalizar_analisis() {
    fclose(yyin); //Cierro el archivo
    yylex_destroy(); //Libero  la memria de yylex
}



