#include "sistema_entrada.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


char *lectura;
int delantero, inicio;


void iniciar_sistema_entrada() {
    FILE *codigo;
    int size, bytes;


    codigo=fopen("concurrentSum.go", "r");

    if(codigo==NULL) {
        perror("Error al abrir el fichero\n");
        exit(EXIT_FAILURE);
    }


    fseek(codigo, 0, SEEK_END); // seek to end of file
    size = ftell(codigo); // get current file pointer
    fseek(codigo, 0, SEEK_SET);

    lectura = malloc(size+1);

    if(lectura == NULL) {
        perror("Error al hacer malloc\n");
        exit(EXIT_FAILURE);
    }


    bytes = fread(lectura, size, 1, codigo);
    lectura[size] = EOF;

    if(bytes == 0) {
        perror("Error al leer el fichero\n");
        exit(EXIT_FAILURE);
    }

    delantero = 0;
    inicio = 0;

    //printf("%c\n", lectura[delantero++]);
}


char siguiente_caracter() {
    return lectura[delantero++]; //Accede a delantero y luego incrementa su valor
}

char *pedir_lexema() {
    int tamano = delantero - inicio;
    char *cadena = malloc(tamano);


    for (int i=0; i<tamano; i++) {
        cadena[i] = lectura[inicio+i];
    }


    return cadena;
}

void fin_lexema() {
    inicio = delantero;
}

void retroceder() {
    delantero--;
}

void terminar() {
    free(lectura);
}
