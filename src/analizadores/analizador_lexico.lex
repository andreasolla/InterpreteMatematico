/*-----OPCIONES-----*/

/* fuerzo que se se genere un analizador de 8 bits */
%option 8bit

/*para que no intente leer otro fichero una vez llega a eof*/
%option noyywrap

/*Para contar el número de línea*/
%option yylineno


/*-----DEFINICIONES-----*/

LETTER          [a-zA-Z_]
DECIMAL_DIGIT   [0-9]
BINARY_DIGIT    [0-1]
OCTAL_DIGIT     [0-7]
HEX_DIGIT       [0-9a-fA-F]
ESPACIO         [[:blank:]]  

COMENTARIOS_BLOQUE     \/\*(.|\n)*\*\/
COMENTARIOS_LINEA       \/\/.*\n


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
COMPLEX         ({DECIMAL_NUM}|{INT}|{FLOAT})i

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

P_IZQUIERDO "("
P_DERECHO ")"
C_IZQUIERDO "["
C_DERECHO "]"
L_IZQUIERDA "{"
L_DERECHA "}"
COMA ","
PUNTO "."
PUNTO_Y_COMA ";"


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

    int anade_punto_y_coma = 0;

%}


/*-----REGLAS-----*/

%%


{ID} {
    anade_punto_y_coma = 1;

    componente.lexema = strdup(yytext); /*malloc + strcpy*/
    componente.componente_lexico = buscar_lexema(componente.lexema);
    return &componente;
}

{STRING} {
    anade_punto_y_coma = 1;

    componente.lexema = strdup(yytext); /*malloc + strcpy*/
    componente.componente_lexico = STRING;
    return &componente;
}


 /*OPERADORES*/

{IGUAL} {
    anade_punto_y_coma = 0;

    componente.componente_lexico = OPERADOR_ASIGNACION;
    return &componente;
}

{DOS_PUNTOS}{IGUAL} {
    anade_punto_y_coma = 0;

    componente.componente_lexico = OPERADOR_INICIALIZACION;    
    return &componente;
}

{SUMA}{IGUAL} {
    anade_punto_y_coma = 0;

    componente.componente_lexico = OPERADOR_MAS_IGUAL;
    return &componente;
}

{MENOR}{RESTA} {
    anade_punto_y_coma = 0;

    componente.componente_lexico = OPERADOR_ENVIO;
    return &componente;
}

{SUMA} {
    anade_punto_y_coma = 0;

    componente.componente_lexico = OPERADOR_SUMA;
    return &componente;
}

{RESTA} {
    anade_punto_y_coma = 0;

    componente.componente_lexico = OPERADOR_RESTA;
    return &componente;
}

{MULT} {
    anade_punto_y_coma = 0;

    componente.componente_lexico = OPERADOR_MULT;
    return &componente;
}

{DIV} {
    anade_punto_y_coma = 0;

    componente.componente_lexico = OPERADOR_DIV;
    return &componente;
}

{DOS_PUNTOS} {
    anade_punto_y_coma = 0;

    componente.componente_lexico = OPERADOR_RANGO;
    return &componente;
}

{MENOR} {
    anade_punto_y_coma = 0;

    componente.componente_lexico = OPERADOR_MENOR;
    return &componente;
}


 /*SEPARADORES*/

{P_IZQUIERDO} {
    anade_punto_y_coma = 0;

    componente.componente_lexico = SEPARADOR_PAR_IZQ;
    return &componente;
}

{P_DERECHO} {
    anade_punto_y_coma = 1;

    componente.componente_lexico = SEPARADOR_PAR_DER;
    return &componente;
}

{C_IZQUIERDO} {
    anade_punto_y_coma = 0;

    componente.componente_lexico = SEPARADOR_COR_IZQ;
    return &componente;
}

{C_DERECHO} {
    anade_punto_y_coma = 1;

    componente.componente_lexico = SEPARADOR_COR_DER;
    return &componente;
}

{L_IZQUIERDA} {
    anade_punto_y_coma = 0;

    componente.componente_lexico = SEPARADOR_LLAVE_IZQ;
    return &componente;
}

{L_DERECHA} {
    anade_punto_y_coma = 1;

    componente.componente_lexico = SEPARADOR_LLAVE_DER;
    return &componente;
}

{COMA} {
    anade_punto_y_coma = 0;

    componente.componente_lexico = SEPARADOR_COMA;
    return &componente;
}

{PUNTO} {
    anade_punto_y_coma = 0;

    componente.componente_lexico = SEPARADOR_PUNTO;
    return &componente;
}

{PUNTO_Y_COMA} {
    anade_punto_y_coma = 0;

    componente.componente_lexico = SEPARADOR_PUNTOYCOMA;
    return &componente;
}

 /*NUMEROS*/

{INT} {
    anade_punto_y_coma = 1;

    componente.lexema = strdup(yytext); 
    componente.componente_lexico = INTEGER;
    return &componente;
}

{FLOAT} {
    anade_punto_y_coma = 1;

    componente.lexema = strdup(yytext); 
    componente.componente_lexico = FLOAT;
    return &componente;
}

{COMPLEX} {
    anade_punto_y_coma = 1;

    componente.lexema = strdup(yytext); 
    componente.componente_lexico = COMPLEX;
    return &componente;
}

 /*IGNORO LOS COMENTARIOS DE BLOQUE*/

{COMENTARIOS_BLOQUE} 

 /*los comentarios de linea se interpretan como \n*/

{COMENTARIOS_LINEA} | 
\n {
    num_columnas = 0;

    if(anade_punto_y_coma) {
        anade_punto_y_coma = 0;

        componente.componente_lexico = SEPARADOR_PUNTOYCOMA;
        return &componente;
    }
}

{ESPACIO} /*ignoro los espacios*/


<<EOF>> {
    componente.componente_lexico = LIMITE_EOF;
    return &componente;
}


%%

//Procedimiento para iniciar el análisis léxico
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

//Procedimientos de liberación de memoria para la finalización del análisis léxico
void finalizar_analisis() {
    fclose(yyin); //Cierro el archivo
    yylex_destroy(); //Libero  la memria de yylex
}

void destruir_comp_lexico(comp_lexico *cp) {
    if(cp->lexema != NULL) {
        free(cp->lexema);
    }
    cp->lexema = NULL;
}

/*Funciṕn para poder imprimir los lexemas 
constantes asociados a componentes léxicas.
Esto se hace para evitar copiar el lexema cada 
vez que se leen componentes cuyo lexema no cambia*/
char *obtener_constante(int comp) {
    switch(comp) {
        case SEPARADOR_PAR_IZQ:
            return "(";
            break;
        case SEPARADOR_PAR_DER:
            return ")";
            break;
        case SEPARADOR_COR_IZQ:
            return "[";
            break;
        case SEPARADOR_COR_DER:
            return "]";
            break;
        case SEPARADOR_LLAVE_IZQ:
            return "{";
            break;
        case SEPARADOR_LLAVE_DER:
            return "}";
            break;
        case SEPARADOR_COMA:
            return ",";
            break;
        case SEPARADOR_PUNTO:
            return ".";
            break;
        case SEPARADOR_PUNTOYCOMA:
            return ";";
            break;
        case OPERADOR_ASIGNACION:
            return "=";
            break;
        case OPERADOR_INICIALIZACION:
            return ":=";
            break;
        case OPERADOR_MAS_IGUAL:
            return "+=";
            break;
        case OPERADOR_ENVIO:
            return "<-";
            break;
        case OPERADOR_SUMA:
            return "+";
            break;
        case OPERADOR_RESTA:
            return "-";
            break;
        case OPERADOR_MULT:
            return "*";
            break;
        case OPERADOR_DIV:
            return "/";
            break;
        case OPERADOR_RANGO:
            return ":";
            break;
        case OPERADOR_MENOR:
            return "<";
            break;
        default:
            return "";
    }
}

//Función para imprimir el componente según sea de lexema constante o variable
void imprimir_componente(comp_lexico cp) {
    if(cp.lexema != NULL) {
        printf("<%d,%s>\n", cp.componente_lexico, cp.lexema);
    } else {
        printf("<%d,%s>\n", cp.componente_lexico, obtener_constante(cp.componente_lexico));
    }
}

