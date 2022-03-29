#ifndef sistema_entrada_h
#define sistema_entrada_h


#define TAM_BUFFER 64 //tamaño de cada uno de los 2 buffers (tam máximo de lexema)


void iniciar_sistema_entrada();
char siguiente_caracter();
char *pedir_lexema();
void liberar_lexema(char *cadena);
void fin_lexema();
void retroceder();
void terminar();
int fin_alcanzado();


#endif