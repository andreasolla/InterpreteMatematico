#ifndef errores_h
#define errores_h


//Códigos de error
#define ERROR_ID_NO_DEFINIDO "El identificador no está definido"
#define ERROR_ARCHIVO "Ha habido un error al abrir el archivo"
#define ERROR_2_ARCHIVO "No se puede cargar un archivo dentro de otro"
#define ERROR_SINTACTICO "Expresión no reconocida"
#define ERROR_LIBRERIA "Error al importar la librería"
#define ERROR_MODIFICAR_CONSTANTE "No se puede modificar, es una constante"
#define ERROR_NO_VARIABLE "No es una variable"
#define ERROR_OPERACION_ENTERA "No se puede realizar la operación con flotantes"
#define ERROR_SALIR_ARCHIVO "No se puede salir desde un archivo"

#define ERROR_TAM_LEXEMA_EXCEDIDO "Se ha excedido el tamaño máximo de lexema"
#define ERROR_STRING_MAL_FORMADO "String sin cerrar"
#define ERROR_BINARIO_MAL_FORMADO "Número binario mal formado"
#define ERROR_OCTAL_MAL_FORMADO "Número octal mal formado"
#define ERROR_DECIMAL_MAL_FORMADO "Número decimal mal formado"
#define ERROR_HEX_MAL_FORMADO "Número hexadecimal mal formado"
#define ERROR_EXP_ERRONEO "Exponente inválido"


//Función para la impresión de errores
void lanzar_error(char *msg);


#endif