#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../lib/doentes.h"
#include "../lib/funcoes.h"
#include "../lib/ficheiros.h"
#include "../lib/registos.h"
#define BUF 1024

void inserir_Doente(Doente** head){
    char line[BUF];
    char nome[BUF];
    Data data;
    char cc[BUF];
    char telefone[BUF];
    char email[BUF];
    int valid;
    do {
        printf("Introduza o nome do doente: \n");
        if (fgets(line, sizeof(line), stdin)) {
            line[strcspn(line, "\n")] = '\0';
            valid = verifica_nome(line);
        }
    }while(valid == 0);
    strcpy(nome, line);

    do {
        printf("Introduza a data de nascimento(dd/mm/aaaa): \n");
        if (fgets(line, sizeof(line), stdin)) {
            line[strcspn(line, "\n")] = '\0';
            valid = verifica_Data(line);
        }
    }while(valid == 0);
    data = converteString_Data(line);

    do {
        printf("Introduza o número do CC do doente(xxxxxxxx-x-xxx): \n");
        if (fgets(line, sizeof(line), stdin)) {
            line[strcspn(line, "\n")] = '\0';
            valid = verifica_cc(*head, line);
        }
    }while(valid == 0);
    strcpy(cc, line);

    do {
        printf("Introduza o número de telefone do doente: \n");
        if (fgets(line, sizeof(line), stdin)) {
            line[strcspn(line, "\n")] = '\0';
            valid = verifica_telefone(*head, line);
        }
    }while(valid == 0);
    strcpy(telefone, line);

    do {
        printf("Introduza o email do doente: \n");
        if (fgets(line, sizeof(line), stdin)) {
            line[strcspn(line, "\n")] = '\0';
            valid = verifica_email(*head, line);
        }
        if(valid == 0){
            printf("Email inválido\n");
        }
    }while(valid == 0 || valid == 3);
    strcpy(email, line);

    Info dados;
    strcpy(dados.cc, cc);
    strcpy(dados.telefone, telefone);
    strcpy(dados.email, email);
    int id = maior_ID(*head);
    adicionar_Doente(head, nome, data, dados, id);
    addToDoentesFile(nome, data, dados, id);
}

void adicionar_Doente(Doente** head, char* nome, Data data_nasci, Info dados, int id){
    Doente* novo_doente = (Doente*) malloc(sizeof(Doente));
    if (novo_doente == NULL) {
        printf("Erro: Não foi possível alocar memória para o novo doente.\n");
        return;
    }
    novo_doente->id = id;
    strcpy(novo_doente->nome, nome);
    novo_doente->data_nasc = data_nasci;
    novo_doente->inform = dados;
    novo_doente->registo_data = NULL;
    novo_doente->registo_tens_max = NULL;
    novo_doente->prox = NULL;
    novo_doente->prox_tens_max = NULL;

    //Colocar Paciente no seu lugar por ordem alfabética
    Doente* current = (*head)->prox;
    Doente* prev = *head;

    while (current != NULL && strcmp(current->nome, novo_doente->nome) < 0) {
        prev = current;
        current = current->prox;
    }
    novo_doente->prox = current;
    prev->prox = novo_doente;

    // Inserir o Doente na posição correta por ordem de tensão máxima
    current = (*head)->prox_tens_max;
    prev = *head;
    if(current == NULL){
        novo_doente->prox_tens_max = current;
        prev->prox_tens_max = novo_doente;
    }
    else {
        while (current != NULL) {
            prev = current;
            current = current->prox_tens_max;
        }
        novo_doente->prox_tens_max = current;
        prev->prox_tens_max = novo_doente;
    }
}

void eliminar_Doente(Doente** head, int id){
    if (*head == NULL) {
        printf("Não existe nenhum doente na lista.\n");
        return;
    }
    Doente* temp = (*head)->prox;
    Doente* prev = (*head);

    while (temp != NULL && temp->id != id) {
        prev = temp;
        temp = temp->prox;
    }

    if (temp == NULL) {
        printf("O doente com ID %d não foi encontrado.\n", id);
        return;
    }

    prev->prox = temp->prox;

    // Remover doente da lista de doentes por tensão máxima
    Doente* temp_tens_max = (*head)->prox_tens_max;
    Doente* prev_tens_max = *head;

    while (temp_tens_max != NULL && temp_tens_max->id != id) {
        prev_tens_max = temp_tens_max;
        temp_tens_max = temp_tens_max->prox_tens_max;
    }

    if (temp_tens_max != NULL) {
        prev_tens_max->prox_tens_max = temp_tens_max->prox_tens_max;
    }

    limparRegistos(temp);
    free(temp);
    printf("\nDoente eliminado com sucesso\n");

}

void imprimirLista(Doente* head) {
    Doente* temp = head->prox;
    int count = 0;
    while (temp != NULL) {
        if(strcmp(temp->nome, "Especial") != 0){
            printf("\n%d", temp->id);
            printf(" -%s\n", temp->nome);
            count++;
        }
        temp = temp->prox;
    }
    if (count == 0) {
        printf("Não existem Doentes na Lista\n");
    }
}

void imprimir_Doentes_Tensao_Maxima(Doente* head, int tens_max) {
    int count = 0;
    for (Doente* doente = head; doente != NULL; doente = doente->prox_tens_max) {
        for (Registos* registo = doente->registo_tens_max; registo != NULL; registo = registo->prox_tensao_max) {
            if (registo->tensao_max > tens_max) {
                count++;
                printf("Doente ID: %d\n", doente->id);
                printf("%s\n", doente->nome);
                printf("Data: %d/%d/%d\n", registo->data.dia, registo->data.mes, registo->data.ano);
                printf("Tensão máxima: %d\n", registo->tensao_max);
                printf("\n");
            }
        }
    }
    if(count == 0){
        printf("Não foi encontrado um doente com a tensão máxima acima de %d\n", tens_max);
    }
}

void atualizar_Posicao_Doente(Doente** head, Doente* doente) {
    if (doente == NULL || doente->registo_tens_max == NULL) {
        printf("Erro: O doente não existe ou não contém registos");
        return;
    }

    // "Remoção" do doente da Lista
    Doente* current = (*head)->prox_tens_max;
    Doente* prev = *head;
    while (current != NULL && current != doente) {
        prev = current;
        current = current->prox_tens_max;
    }

    prev->prox_tens_max = doente->prox_tens_max;

    // Colocar na posição correta
    current = (*head)->prox_tens_max;
    prev = *head;
    while (current != NULL && current->registo_tens_max != NULL &&
           current->registo_tens_max->tensao_max > doente->registo_tens_max->tensao_max) {
        prev = current;
        current = current->prox_tens_max;
    }

    doente->prox_tens_max = current;
    prev->prox_tens_max = doente;
}

void limparLista(Doente** head) {
    Doente* temp;
    while (*head != NULL) {
        temp = *head;
        *head = (*head)->prox;
        limparRegistos(temp);
        free(temp);
    }
}

int imprimir_Doentes_Com_Nome(Doente* head, char* nome) {
    Doente* temp = head;
    int found = 0;
    int count = 0;
    while (temp != NULL) {
        if (strcmp(temp->nome, nome) == 0) {
            printf("\nID: %d", temp->id);
            printf("\nNome: %s", temp->nome);
            printf("\nData de Nascimento: %d/%d/%d\n", temp->data_nasc.dia, temp->data_nasc.mes, temp->data_nasc.ano);
            count++;
            found = 1;
        }
        temp = temp->prox;
    }
    if (found == 0) {
        printf("Não foram encontrados doentes com o nome %s.\n", nome);
    }
    return count;
}

void search(Doente **head, int id, Doente **prev, Doente **cur ){
    *prev = *head;
    *cur = (*head)->prox;
    while(*cur != NULL){
        if((*cur)->id == id){
            return;
        }
        *prev = *cur;
        *cur = (*cur)->prox;
    }
}

void search_name(Doente **head, char nome[], Doente **prev, Doente **cur ){
    *prev = *head;
    *cur = (*head)->prox;
    while(*cur != NULL){
        if(strcmp((*cur)->nome, nome) == 0){
            return;
        }
        *prev = *cur;
        *cur = (*cur)->prox;
    }
}