#ifndef errores_h
#define errores_h


//Códigos de error
#define ERROR_TAM_LEXEMA_EXCEDIDO "Se ha excedido el tamaño máximo de lexema"
#define ERROR_STRING_MAL_FORMADO "String sin cerrar"
#define ERROR_BINARIO_MAL_FORMADO "Número binario mal formado"
#define ERROR_OCTAL_MAL_FORMADO "Número octal mal formado"
#define ERROR_DECIMAL_MAL_FORMADO "Número decimal mal formado"
#define ERROR_HEX_MAL_FORMADO "Número hexadecimal mal formado"
#define ERROR_EXP_ERRONEO "Exponente inválido"



//Función para la impresión de errores
void error(char *msg, int numLinea, int numCol);


#endif