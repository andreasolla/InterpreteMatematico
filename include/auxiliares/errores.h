#ifndef errores_h
#define errores_h


//Códigos de error
#define ERROR_ID_NO_DEFINIDO "El identificador no está definido"
#define ERROR_ARCHIVO "Ha habido un error al abrir el archivo"
#define ERROR_2_ARCHIVO "Ya se esta leyendo un archivo"
#define ERROR_SINTACTICO "Expresión no reconocida"
#define ERROR_LIBRERIA "Error al importar la librería"
#define ERROR_MODIFICAR_CONSTANTE "No se puede modificar, es una constante"
#define ERROR_NO_VARIABLE "No es una variable"
#define ERROR_OPERACION_ENTERA "Operación para enteros"
#define ERROR_SALIR_ARCHIVO "No se puede salir desde un archivo"
#define ERROR_DIVISION_POR_CERO "División por cero"


//Función para la impresión de errores
void lanzar_error(char *msg);


#endif