#ifndef PROJETOPPP_DOENTES_H
#define PROJETOPPP_DOENTES_H
#include "STRUCTS.h"

void inserir_Doente(Doente** head);
void adicionar_Doente(Doente** head, char* nome, Data data_nasci, Info dados, int id);
void eliminar_Doente(Doente** head, int id);
void imprimirLista(Doente* head);
void imprimir_Doentes_Tensao_Maxima(Doente* head, int tens_max);
void atualizar_Posicao_Doente(Doente** head, Doente* doente);
void limparLista(Doente** head);
int imprimir_Doentes_Com_Nome(Doente* head, char* nome);
void search(Doente **head, int id, Doente **prev, Doente **cur );
void search_name(Doente **head, char nome[], Doente **prev, Doente **cur );

#endif
