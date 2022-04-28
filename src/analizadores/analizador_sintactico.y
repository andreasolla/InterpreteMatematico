
/*-----INCLUDES-----*/

%code requires{
    #include "analizadores/analizador_sintactico.h"
    #include "analizadores/analizador_lexico.h"
    #include "auxiliares/tabla_simbolos.h"
    #include "auxiliares/errores.h"
    #include "auxiliares/funciones.h"

    #include <stdio.h>
    #include <math.h>
    #include <string.h>
    
    #define COLOR_VERDE   "\x1b[32m"
    #define COLOR_RESET   "\x1b[0m"
}

%union { numero num; char *ptr; }


/*-----SIMBOLOS TERMINALES-----*/

%token BLANCO

%token <num> INTEGER
%token <num> FLOAT

%token <ptr> ID
%token <ptr> FUNC
%token <ptr> ARCHIVO

%token LIMITE_EOF

%left OPERADOR_SUMA OPERADOR_RESTA
%left OPERADOR_MULT OPERADOR_DIV OPERADOR_MODULO

%token SEPARADOR_PAR_IZQ
%token SEPARADOR_PAR_DER
%token SEPARADOR_PUNTO_Y_COMA

%precedence NEG
%right OPERADOR_EXP

%token OPERADOR_IGUAL

%token KW_HELP
%token KW_QUIT
%token KW_LOAD
%token KW_WORKSPACE
%token KW_CLEAR
%token KW_IMPORT

/*-----SIMBOLOS NO TERMINALES-----*/

%type <num> expr;


/*-----FUNCIONES Y VARIABLES LOCALES-----*/

%code {
    void yyerror(char const *msg);
    float valorF(numero x);
    numero operar(numero a, numero b, char op);
    void finalizar();

    char *prompt = "(╮°-°)╮┳━━┳ >> ";
    char *prompt_archivo = "(⌐□_□)┬─┬ >> ";
    int hay_error=0;
    int leyendo_archivo=0;
}


/*-----GRAMÁTICA-----*/

%%

input: %empty
    | input line {if(leyendo_archivo) { printf("\n%s", prompt_archivo); } else { printf("\n%s", prompt); } liberar_direcciones(); }
    | input LIMITE_EOF { printf("FIN ARCHIVO\n"); leyendo_archivo=0; printf("\n%s", prompt); liberar_direcciones(); }
;

line: BLANCO
    | expr BLANCO { if(!hay_error) { if($1.tipo == 'f') {printf("%f\n", $1.valor.flotante);} else {printf("%d\n", $1.valor.entero);} } else { hay_error = 0; } }
    | expr SEPARADOR_PUNTO_Y_COMA BLANCO {}
    | op BLANCO
    | error BLANCO { yyclearin; yyerrok; }
;

op: KW_HELP { help(); }
    | KW_CLEAR { clear(NULL); }
    | KW_CLEAR SEPARADOR_PAR_IZQ ID SEPARADOR_PAR_DER  { if(id_definido($3)) { clear($3); } else {lanzar_error(ERROR_ID_NO_DEFINIDO); } }
    | KW_LOAD SEPARADOR_PAR_IZQ ARCHIVO SEPARADOR_PAR_DER { if(!leyendo_archivo) { leyendo_archivo=1; load($3); } else { lanzar_error(ERROR_2_ARCHIVO); } }
    | KW_WORKSPACE { workspace(); }
    | KW_QUIT { if(leyendo_archivo) {lanzar_error(ERROR_SALIR_ARCHIVO); } else { finalizar(); return 0; } }
    | KW_IMPORT SEPARADOR_PAR_IZQ ARCHIVO SEPARADOR_PAR_DER { import($3); }
    | KW_IMPORT SEPARADOR_PAR_IZQ ID SEPARADOR_PAR_DER { import($3); }
;

expr: INTEGER { $$.valor.entero = $1.valor.entero; $$.tipo = 'i'; }
    | FLOAT { $$.valor.flotante = $1.valor.flotante; $$.tipo = 'f'; }
    | ID { if(id_definido($1)) {$$ = obtener_valor($1);} else {lanzar_error(ERROR_ID_NO_DEFINIDO); hay_error=1; } }
    | ID OPERADOR_IGUAL expr { if(anadir_variable($1, $3)) { $$ = $3; } else {hay_error=1;} }
    | expr OPERADOR_SUMA expr { $$ = operar($1, $3, '+'); }
    | expr OPERADOR_RESTA expr { $$ = operar($1, $3, '-'); }
    | expr OPERADOR_MULT expr { $$ = operar($1, $3, '*'); }
    | expr OPERADOR_DIV expr { $$ = operar($1, $3, '/'); }
    | expr OPERADOR_EXP expr { $$ = operar($1, $3, '^'); }
    | expr OPERADOR_MODULO expr { $$ = operar($1, $3, '%'); }
    | OPERADOR_RESTA expr %prec NEG { if($2.tipo == 'f') {$$.valor.flotante = -$2.valor.flotante; $$.tipo = 'f'; } else {$$.valor.entero = -$2.valor.entero; $$.tipo = 'i'; } }
    | OPERADOR_SUMA expr { if($2.tipo == 'f') {$$.valor.flotante = $2.valor.flotante; $$.tipo = 'f'; } else {$$.valor.entero = $2.valor.entero; $$.tipo = 'i'; } }
    | SEPARADOR_PAR_IZQ expr SEPARADOR_PAR_DER { $$ = $2; }
    | FUNC SEPARADOR_PAR_IZQ expr SEPARADOR_PAR_DER { $$.valor.flotante = (*(obtener_funcion($1).cont.funcion))(valorF($3)); $$.tipo='f'; }
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
                if(valorF(b) == 0){
                    lanzar_error(ERROR_DIVISION_POR_CERO);
                    hay_error=1;
                } else {
                    resultado.valor.flotante = valorF(a) / valorF(b);
                }
                break;
            case '^':
                resultado.valor.flotante = pow(valorF(a), valorF(b));
                break;
            case '%':
                lanzar_error(ERROR_OPERACION_ENTERA);
                hay_error=1; //Para que no se ejecute el resto de la operacion
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
                if(valorF(b) == 0){
                    lanzar_error(ERROR_DIVISION_POR_CERO);
                    hay_error=1;
                } else {
                    resultado.valor.entero = a.valor.entero / b.valor.entero;
                }
                break;
            case '^':
                resultado.valor.entero = pow(a.valor.entero, b.valor.entero);
                break;
            case '%':
                resultado.valor.entero = a.valor.entero % b.valor.entero;
                break;
        }
    }
    
    return resultado;
}

void realizar_analisis() {
    printf("%s", prompt);
    inicializar_analizador_lexico();
    yyparse();
}

void finalizar() {
    //Finalizo el análisis en el analizador lexico (y la tabla de símbolos)
    finalizar_analisis();

    printf(COLOR_VERDE "\n  ╭──────────────────────────────────╮\n");
    printf("╭─┤FINALIZANDO...   ┳━━┳ノ( º _ ºノ) ├──────────\n");
    printf("│ ╰──────────────────────────────────╯\n");
    printf("│  Hasta pronto! \n");
    printf("╰───────────────────────────────────────────────\n\n" COLOR_RESET);
}

void yyerror(char const *msg) {
    lanzar_error(ERROR_SINTACTICO);
}