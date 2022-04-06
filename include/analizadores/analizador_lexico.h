#ifndef analizador_lexico_h
#define analizador_lexico_h


#include "auxiliares/definiciones.h"


void inicializar_analizador_lexico();
void finalizar_analisis();

comp_lexico *sig_comp_lexico();
void destruir_comp_lexico(comp_lexico *cp);


#endif