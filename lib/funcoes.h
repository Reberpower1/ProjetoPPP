#ifndef PROJETOPPP_FUNCOES_H
#define PROJETOPPP_FUNCOES_H
#include "STRUCTS.h"

int menu();
int compDates(Data d1, Data d2);
int my_atoi(char line[]);
int my_atoi_ficheiros(char line[]);
int verifica_nome(char line[]);
Data converteString_Data(char* line);
int verifica_altura(char line[]);
int verifica_peso(char line[]);
int verifica_Data(char line[]);
int verifica_cc(Doente* head, char line[]);
int verifica_telefone(Doente* head, char line[]);
int verifica_email(Doente* head, char line[]);
int verificaDadosDuplicados(Doente* head, Info dados);
int verifica_id(Doente* head, char line[]);
int pedir_id(Doente* head);
int verifica_tensao(char line[]);
int pedir_TensMax();
char* pedir_nome(Doente* head);
int verifica_opcao(char line[]);
int pedirNomeouID();
void init_special_node(Doente **head);
int maior_ID(Doente* head);

#endif //PROJETOPPP_FUNCOES_H
