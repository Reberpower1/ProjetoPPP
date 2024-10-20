#ifndef PROJETOPPP_REGISTOS_H
#define PROJETOPPP_REGISTOS_H
#include "STRUCTS.h"

void adicionar_Registo(Doente** head, Doente* doente, Data data_info, int tens_min, int tens_max, int altura, int peso);
void inserir_Registo(Doente** head, int id);
void imprimir_Registos_Doente(Doente* head, int id);
void inserir_Registo_Tensao_Max(Doente* doente, Registos* novo_registo);
void limparRegistos(Doente* doente);

#endif