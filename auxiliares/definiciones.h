#ifndef definiciones_h
#define definiciones_h

typedef struct comp_lexico
{
    int componente_lexico;
    char* lexema;
} comp_lexico;


#define KW_CHAN 300
#define KW_FOR 301
#define KW_FUNC 302
#define KW_GO 303
#define KW_IMPORT 304
#define KW_PACKAGE 305
#define KW_RANGE 306
#define KW_VAR 307

#define ID 400
#define STRING 401
#define INTEGER 402
#define FLOAT 403
#define COMPLEX 404

#define TIPO_FLOAT32 450

#define SEPARADOR_PAR_IZQ 500
#define SEPARADOR_PAR_DER 501
#define SEPARADOR_COR_IZQ 502
#define SEPARADOR_COR_DER 503
#define SEPARADOR_LLAVE_IZQ 504
#define SEPARADOR_LLAVE_DER 505
#define SEPARADOR_COMA 506
#define SEPARADOR_PUNTO 507

#define OPERADOR_ASIGNACION 600
#define OPERADOR_INICIALIZACION 601
#define OPERADOR_MAS_IGUAL 602
#define OPERADOR_ENVIO 603
#define OPERADOR_SUMA 604
#define OPERADOR_RESTA 605
#define OPERADOR_MULT 606
#define OPERADOR_DIV 607
#define OPERADOR_RANGO 608
#define OPERADOR_MENOR 609

#define LIMITE_EOF 700


#endif