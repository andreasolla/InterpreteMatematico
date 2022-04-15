
/*-----INCLUDES-----*/

%code requires{
    #include "analizadores/analizador_sintactico.h"

    #include <stdio.h>
    #include <math.h>
    
}

%union { double valor; }


/*-----SIMBOLOS TERMINALES-----*/

%token BLANCO

%token <valor> INTEGER
%token <valor> FLOAT


%left OPERADOR_SUMA OPERADOR_RESTA
%left OPERADOR_MULT OPERADOR_DIV

%token SEPARADOR_PAR_IZQ
%token SEPARADOR_PAR_DER

%precedence NEG
%right OPERADOR_EXP


/*-----SIMBOLOS NO TERMINALES-----*/

%type <valor> expr;


/*-----FUNCIONES Y VARIABLES LOCALES-----*/

%code {
    void yyerror(char const *msg);
}



/*-----GRAMÁTICA-----*/

%%

input: %empty
    | input line
;

line: BLANCO
    | expr BLANCO { printf("%f\n", $1); }
;

expr: INTEGER { $$ = $1; }
    | FLOAT { $$ = $1; }
    | expr OPERADOR_SUMA expr { $$ = $1 + $3; printf("hola\n"); }
    | expr OPERADOR_RESTA expr { $$ = $1 - $3; }
    | expr OPERADOR_MULT expr { $$ = $1 * $3; }
    | expr OPERADOR_DIV expr { $$ = $1 / $3; }
    | expr OPERADOR_EXP expr { $$ = pow( $1,  $3); }
    | OPERADOR_RESTA expr %prec NEG { $$ = -$2; }
    | SEPARADOR_PAR_IZQ expr SEPARADOR_PAR_DER { $$ = $2; }
;


%%

void realizar_analisis() {
    yyparse();
}

void yyerror(char const *msg) {
    printf("Error analizador sintactico-semantico: %s\n", msg);
}