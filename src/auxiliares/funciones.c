#include "auxiliares/funciones.h"

#include "analizadores/analizador_sintactico.h"
#include "analizadores/analizador_lexico.h"
#include "auxiliares/tabla_simbolos.h"

#include <dlfcn.h>

void help() {
    printf("\n        ╭──────────────────────────────────╮\n");
    printf("        │ Bienvenid@ a la ayuda\t\t   │\n");
    printf("        └──────────────────────────────────╯\n");
    printf("(╮°-°)╮╱\n\n");

    printf("\n\nINSTRUCCIONES DE USO\n"
            "\n"
            "FUNCIONES\n"
            "Las funciones propias del intérprete son las siguientes:\n"
            "- help\n"
            "    imprime la ayuda \n"
            "\n"
            "- workspace\n"
            "    muestra las variables almacenadas en el intérprete\n"
            "\n"
            "- clear\n"
            "    elimina todas las variables, si se añade (<nombre_variable>), elimina la variable indicada \n"
            "\n"
            "- quit\n"
            "    sale del programa\n"
            "\n"
            "- load(\"<archivo>\") \n"
            "    carga el archivo indicado y lo ejecuta línea a línea\n"
            "\n"
            "- import(\"<nombre>\") \n"
            "    importa la libreria indicada para tener disponibles sus funciones\n"
            "\n"
            "\n"
            "CONSIDERACIONES\n"
            "- Tras cada instrucción se muestra el resultado de la misma. Para evitar que se muestre el resultado, se debe añadir ';' al final de la línea.\n"
            "\n"
            "- Las operaciones básicas con números enteros realizan el cálculo entero (para obtener el resultado flotante, se debe introducir alguno de los números flotante)\n"
            "    p.e.:\n"
            "        4/3 = 1\n"
            "        4/3.0 = 1.333333\n"
            "        4.0/3 = 1.333333\n"
            "\n"
            "operaciones básicas -> suma (+), resta (-), multiplicación (*), división (/), potencia (^) y módulo (%).\n"
            "\n"
            "Las constantes almacenadas son e y pi\n"
            "\n"
            "\n"
            "EJEMPLO DE USO \n"
            "\n"
            "Para importar las funciones de la librería matemática:\n"
            "\n"
            "    (╮°-°)╮┳━━┳ >> import(\"libm.so.6\")   \n"
            "\n"
            "    (╮°-°)╮┳━━┳ >> sin(pi/2)\n"
            "    1.000000\n\n\n"
        );
}

void workspace() {
    imprimir_workspace();
}

void clear(char *var) {
    if(var==NULL) {
        vaciar_workspace();
    } else {
        eliminar_variable(var);
    }
}

void load(char *nombre) {
    leer_archivo(nombre);
}

void import(char *libreria) {
    anadir_libreria(libreria);
}