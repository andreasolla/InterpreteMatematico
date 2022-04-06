/*-----OPCIONES-----*/

/* fuerzo que se se genere un analizador de 8 bits */
%option 8bit

/*para que no intente leer otro fichero una vez llega a eof*/
%option noyywrap

/*Para contar el número de líneas*/
%option yylineno


/*-----DEFINICIONES-----*/

LETTER          [a-zA-Z_]
DECIMAL_DIGIT   [0-9]
BINARY_DIGIT    [0-1]
OCTAL_DIGIT     [0-7]
HEX_DIGIT       [0-9a-fA-F]

COMENTARIOS_BLOQUE     \/\*(.|\n)*\*\/
COMENTARIOS_LINEA       \/\/.*

/*NUMEROS*/

DECIMAL_NUM     [1-9](_?{DECIMAL_DIGIT})*
BINARY_NUM      0[bB](_?{BINARY_DIGIT})*
OCTAL_NUM       0[oO]?(_?{OCTAL_DIGIT})*
HEX_NUM         0[xX](_?{HEX_DIGIT})*

EXP_DECIMAL     [eE][+-]?{DECIMAL_DIGIT}(_?{DECIMAL_DIGIT})*
EXP_HEX         [pP][+-]?{DECIMAL_DIGIT}(_?{DECIMAL_DIGIT})*

/*TIPOS DE DATOS*/

INT             {DECIMAL_NUM}|{BINARY_NUM}|{OCTAL_NUM}|{HEX_NUM}
FLOAT           {DECIMAL_NUM}\.{DECIMAL_NUM}?{EXP_DECIMAL}?|{DECIMAL_NUM}{EXP_DECIMAL}|\.{DECIMAL_NUM}{EXP_DECIMAL}?|{HEX_NUM}(\.{HEX_DIGIT}+)?{EXP_HEX}|\.{HEX_DIGIT}+{EXP_HEX}
COMPLEX         {DECIMAL_NUM}|{INT}|{FLOAT}

STRING          \".*\"
ID              {LETTER}({LETTER}|{DECIMAL_DIGIT})*


/*OPERADORES*/

IGUAL "="
SUMA "+"
DOS_PUNTOS ":"
RESTA "-"
MULT "*"
DIV "/"
MENOR "<"


/*SEPARADORES*/



/*-----CABECERA DEL .c-----*/
%{
    #include "analizadores/analizador_lexico.h"
    #include "auxiliares/definiciones.h"
    #include "auxiliares/tabla_simbolos.h"

    /*Redefino yylex para que devuelva mi estructura*/
    #define YY_DECL comp_lexico *sig_comp_lexico()

    /*Variable a devolver*/
    comp_lexico componente; 


    /*Reescribo el tamaño de buffer de flex a 4096 bytes*/
    #define YY_BUF_SIZE 4096


    /*Cuento las columnas para especificar donde hay errores*/
    int num_columnas = 0;

    #define YY_USER_ACTION { num_columnas += yyleng; }

%}



/*-----REGLAS-----*/

%%

\n {
    num_columnas = 0;
}

{ID} {
    componente.lexema = strdup(yytext); /*malloc + strcpy*/
    componente.componente_lexico = buscar_lexema(componente.lexema);
    return &componente;
}

{STRING} {
    componente.lexema = strdup(yytext); /*malloc + strcpy*/
    componente.componente_lexico = STRING;
    return &componente;
}

 /*OPERADORES*/

{IGUAL} {
    componente.componente_lexico = OPERADOR_ASIGNACION;
    return &componente;
}

{DOS_PUNTOS}{IGUAL} {
    componente.componente_lexico = OPERADOR_INICIALIZACION;
    return &componente;
}



{MENOR}{IGUAL} {
    componente.componente_lexico = OPERADOR_ENVIO;
    return &componente;
}

{RESTA} {
    componente.componente_lexico = OPERADOR_RESTA;
    return &componente;
}

{SUMA}{IGUAL} {
    componente.componente_lexico = OPERADOR_MAS_IGUAL;
    return &componente;
}

{MULT} {
    componente.componente_lexico = OPERADOR_MULT;
    return &componente;
}

{DIV} {
    componente.componente_lexico = OPERADOR_DIV;
    return &componente;
}

{DOS_PUNTOS} {
    componente.componente_lexico = OPERADOR_RANGO;
    return &componente;
}

{MENOR} {
    componente.componente_lexico = OPERADOR_MENOR;
    return &componente;
}


"(" {
    componente.componente_lexico = SEPARADOR_PAR_IZQ;
    return &componente;
}

")" {
    componente.componente_lexico = SEPARADOR_PAR_DER;
    return &componente;
}

"[" {
    componente.componente_lexico = SEPARADOR_COR_IZQ;
    return &componente;
}

"]" {
    componente.componente_lexico = SEPARADOR_COR_DER;
    return &componente;
}

"{" {
    componente.componente_lexico = SEPARADOR_LLAVE_IZQ;
    return &componente;
}

"}" {
    componente.componente_lexico = SEPARADOR_LLAVE_DER;
    return &componente;
}

"," {
    componente.componente_lexico = SEPARADOR_COMA;
    return &componente;
}

"\." {
    componente.componente_lexico = SEPARADOR_PUNTO;
    return &componente;
}

";" {
    componente.componente_lexico = SEPARADOR_PUNTOYCOMA;
    return &componente;
}

{INT} {
    componente.lexema = strdup(yytext); 
    componente.componente_lexico = INTEGER;
    return &componente;
}

{FLOAT} {
    componente.lexema = strdup(yytext); 
    componente.componente_lexico = FLOAT;
    return &componente;
}

{COMPLEX} {
    componente.lexema = strdup(yytext); 
    componente.componente_lexico = COMPLEX;
    return &componente;
}

{COMENTARIOS_BLOQUE} | {COMENTARIOS_LINEA} /*IGNORO LOS COMENTARIOS*/

<<EOF>> {
    componente.componente_lexico = LIMITE_EOF;
    return &componente;
}


%%

void inicializar_analizador_lexico()
{
    
    yyin = fopen("concurrentSum.go", "r"); // Abro el codigo

    if (yyin == NULL)
    { // comprobación de errores de apertura
        perror("Error al abrir el fichero\n");
        exit(EXIT_FAILURE);
    }

    componente.lexema = NULL;
}

void finalizar_analisis() {
    fclose(yyin);
}

void destruir_comp_lexico(comp_lexico *cp) {
    if(cp->lexema != NULL) {
        free(cp->lexema);
    }
    cp->lexema = NULL;
}
