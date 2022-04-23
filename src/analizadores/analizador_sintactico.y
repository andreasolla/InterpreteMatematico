
/*-----INCLUDES-----*/

%code requires{
    #include "analizadores/analizador_sintactico.h"
    #include "auxiliares/tabla_simbolos.h"
    #include "auxiliares/errores.h"

    #include <stdio.h>
    #include <math.h>
    #include <string.h>
    

}

%union { numero num; char *ptr; }


/*-----SIMBOLOS TERMINALES-----*/

%token BLANCO

%token <num> INTEGER
%token <num> FLOAT

%token <ptr> ID
%token <ptr> FUNC

%left OPERADOR_SUMA OPERADOR_RESTA
%left OPERADOR_MULT OPERADOR_DIV

%token SEPARADOR_PAR_IZQ
%token SEPARADOR_PAR_DER

%precedence NEG
%right OPERADOR_EXP

%token OPERADOR_IGUAL


/*-----SIMBOLOS NO TERMINALES-----*/

%type <num> expr;


/*-----FUNCIONES Y VARIABLES LOCALES-----*/

%code {
    void yyerror(char const *msg);
    float valorF(numero x);
    numero operar(numero a, numero b, char op);

    char *prompt = "(╮°-°)╮┳━━┳ >> ";
    int hay_error=0;
}


/*-----GRAMÁTICA-----*/

%%

input: %empty
    | input line {printf("\n%s", prompt);}
;

line: BLANCO
    | expr BLANCO { if(!hay_error) { if($1.tipo == 'f') {printf("%f\n", $1.valor.flotante);} else {printf("%d\n", $1.valor.entero);} hay_error = 0; } }
;

expr: INTEGER { $$.valor.entero = $1.valor.entero; $$.tipo = 'i'; }
    | FLOAT { $$.valor.flotante = $1.valor.flotante; $$.tipo = 'f'; }
    | ID { if(id_definido($1) != NULL) {$$ = obtener_valor($1);} else {error(ERROR_ID_NO_DEFINIDO); hay_error=1; } }
    | ID OPERADOR_IGUAL expr { anadir_variable($1, $3); $$ = $3; }
    | expr OPERADOR_SUMA expr { $$ = operar($1, $3, '+'); }
    | expr OPERADOR_RESTA expr { $$ = operar($1, $3, '-'); }
    | expr OPERADOR_MULT expr { $$ = operar($1, $3, '*'); }
    | expr OPERADOR_DIV expr { $$ = operar($1, $3, '/'); }
    | expr OPERADOR_EXP expr { $$ = operar($1, $3, '^'); }
    | OPERADOR_RESTA expr %prec NEG { if($2.tipo == 'f') {$$.valor.flotante = -$2.valor.flotante; $$.tipo = 'f'; } else {$$.valor.entero = -$2.valor.entero; $$.tipo = 'i'; } }
    | SEPARADOR_PAR_IZQ expr SEPARADOR_PAR_DER { $$ = $2; }
    | FUNC SEPARADOR_PAR_IZQ expr SEPARADOR_PAR_DER { $$.valor.flotante = (*(obtener_funcion($1).cont.funcion))(valorF($3)); $$.tipo='f'; printf("%p: %p\n", obtener_funcion($1).cont.funcion, sin); }
;


%%

/*OPERACIONES*/

float valorF(numero x){
    if(x.tipo == 'f'){
        return x.valor.flotante;
    }else{
        return x.valor.entero;
    }
}

numero operar(numero a, numero b, char op){
    numero resultado;
    if(a.tipo == 'f' || b.tipo == 'f'){
        resultado.tipo = 'f';

        switch(op){
            case '+':
                resultado.valor.flotante = valorF(a) + valorF(b);
                break;
            case '-':
                resultado.valor.flotante = valorF(a) - valorF(b);
                break;
            case '*':
                resultado.valor.flotante = valorF(a) * valorF(b);
                break;
            case '/':
                resultado.valor.flotante = valorF(a) / valorF(b);
                break;
            case '^':
                resultado.valor.flotante = pow(valorF(a), valorF(b));
                break;
        }
    } else {
        resultado.tipo = 'i';

        switch(op){
            case '+':
                resultado.valor.entero = a.valor.entero + b.valor.entero;
                break;
            case '-':
                resultado.valor.entero = a.valor.entero - b.valor.entero;
                break;
            case '*':
                resultado.valor.entero = a.valor.entero * b.valor.entero;
                break;
            case '/':
                resultado.valor.entero = a.valor.entero / b.valor.entero;
                break;
            case '^':
                resultado.valor.entero = pow(a.valor.entero, b.valor.entero);
                break;
        }
    }
    
    return resultado;
}



void realizar_analisis() {
    error(ERROR_STRING_MAL_FORMADO);
    printf("%s", prompt);
    yyparse();
}

void yyerror(char const *msg) {
    printf("Error analizador sintactico-semantico: %s\n", msg);
}