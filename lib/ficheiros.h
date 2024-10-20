#ifndef PROJETOPPP_FICHEIROS_H
#define PROJETOPPP_FICHEIROS_H
#include "STRUCTS.h"

void criar_Lista_Doentes_Registos(Doente **head);
void addToDoentesFile(char* nome, Data data_nasci, Info dados, int id);
void addToRegistosFile(int id, Data data_info, int tens_min, int tens_max, int altura, int peso);
void eliminarLinhas(FILE* file, const char* nome_ficheiro, int inicio, int fim);
void eliminarDoenteFicheiros(int id);
void reescreverFicheiros(Doente* head);

#endif
