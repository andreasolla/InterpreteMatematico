#include "auxiliares/funciones.h"

#include "analizadores/analizador_sintactico.h"
#include "analizadores/analizador_lexico.h"
#include "auxiliares/tabla_simbolos.h"

void help() {

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

void quit() {
    //finalizar();
}

void load(char *nombre) {
    leer_archivo(nombre);
}