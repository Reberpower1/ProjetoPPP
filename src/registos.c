#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../lib/registos.h"
#include "../lib/funcoes.h"
#include "../lib/ficheiros.h"
#include "../lib/doentes.h"
#define BUF 1024

void adicionar_Registo(Doente** head, Doente* doente, Data data_info, int tens_min, int tens_max, int altura, int peso){
    Registos* novo_registo = (Registos*) malloc(sizeof(Registos));
    if (novo_registo == NULL) {
        printf("Erro: Não foi possível alocar memória para o novo registo.\n");
        return;
    }
    novo_registo->data = data_info;
    novo_registo->tensao_max = tens_max;
    novo_registo->tensao_min = tens_min;
    novo_registo->altura = altura;
    novo_registo->peso = peso;
    novo_registo->prox_data = NULL;
    novo_registo->prox_tensao_max = NULL;

    // Inserir o registo na posição correta por Data
    Registos* current = doente->registo_data;
    Registos* prev = NULL;
    while (current != NULL && compDates(current->data, novo_registo->data) > 0) {
        prev = current;
        current = current->prox_data;
    }
    if (prev == NULL) {
        novo_registo->prox_data = doente->registo_data;
        doente->registo_data = novo_registo;
    } else {
        novo_registo->prox_data = current;
        prev->prox_data = novo_registo;
    }

    // Inserir o registo na posição correta por Tensão Máxima
    inserir_Registo_Tensao_Max(doente, novo_registo);

    // Atualizar a posição do doente na lista de doentes
    atualizar_Posicao_Doente(head, doente);
}

void inserir_Registo(Doente** head, int id){
    Data data_registo;
    int tens_max;
    int tens_min;
    float altura;
    float peso;
    char line[BUF];
    int valid;
    Doente* doente = NULL;
    do {
        printf("Introduza a data do registo(dd/mm/aaaa): \n");
        if (fgets(line, sizeof(line), stdin)) {
            line[strcspn(line, "\n")] = '\0';
            valid = verifica_Data(line);
        }
        if(valid == 0){
            printf("Data inválida\n");
        }
    }while(valid == 0);
    data_registo = converteString_Data(line);

    do {
        printf("Introduza a tensão máxima registada: \n");
        if (fgets(line, sizeof(line), stdin)) {
            line[strcspn(line, "\n")] = '\0';
            valid = verifica_tensao(line);
        }
        if(valid == 0){
            printf("Valor de tensão inválido\n");
        }
    }while(valid == 0);
    tens_max = my_atoi(line);

    do {
        printf("Introduza a tensão mínima registada: \n");
        if (fgets(line, sizeof(line), stdin)) {
            line[strcspn(line, "\n")] = '\0';
            valid = verifica_tensao(line);
        }
        if(valid == 0){
            printf("Valor de tensão inválido\n");
        }
    }while(valid == 0);
    tens_min = my_atoi(line);

    do {
        printf("Introduza o peso registado: \n");
        if (fgets(line, sizeof(line), stdin)) {
            line[strcspn(line, "\n")] = '\0';
            valid = verifica_peso(line);
        }
        if(valid == 0){
            printf("Valor de peso inválido\n");
        }
    }while(valid == 0);
    peso = my_atoi(line);

    do {
        printf("Introduza a altura registada: \n");
        if (fgets(line, sizeof(line), stdin)) {
            line[strcspn(line, "\n")] = '\0';
            valid = verifica_altura(line);
        }
        if(valid == 0){
            printf("Altura inválida\n");
        }
    }while(valid == 0);
    altura = my_atoi(line);

    Doente* prev;
    Doente* cur;
    search(head, id, &prev, &cur);

    if(cur != NULL){
        doente = cur;
    }

    if(doente != NULL){
        adicionar_Registo(head, doente, data_registo, tens_min, tens_max, altura, peso);
        addToRegistosFile(id, data_registo, tens_min, tens_max, altura, peso);
    }
}

void imprimir_Registos_Doente(Doente* head, int id) {
    Doente* prev;
    Doente* doente;
    search(&head, id, &prev, &doente);
    int count = 1;
    if(doente == NULL){
        printf("\nDoente com ID %d não existe.\n", id);
        return;
    }

    Registos* registo = doente->registo_data;
    while (registo != NULL) {
        printf("Registo %d do doente com ID %d:\n", count, id);
        count++;
        printf("Data: %d/%d/%d\n", registo->data.dia, registo->data.mes, registo->data.ano);
        printf("Tensão mínima: %d\n", registo->tensao_min);
        printf("Tensão máxima: %d\n", registo->tensao_max);
        printf("Peso: %d\n", registo->peso);
        printf("Altura: %d\n", registo->altura);
        printf("\n");
        registo = registo->prox_data;
    }
    if(count == 1){
        printf("O doente não tem registos.\n");
    }
}

void inserir_Registo_Tensao_Max(Doente* doente, Registos* novo_registo) {
    Registos* current = doente->registo_tens_max;
    Registos* prev = NULL;
    while (current != NULL && current->tensao_max > novo_registo->tensao_max) {
        prev = current;
        current = current->prox_tensao_max;
    }
    if (prev == NULL) {
        novo_registo->prox_tensao_max = doente->registo_tens_max;
        doente->registo_tens_max = novo_registo;
    } else {
        novo_registo->prox_tensao_max = current;
        prev->prox_tensao_max = novo_registo;
    }
}

void limparRegistos(Doente* doente){
    Registos* temp;
    while (doente->registo_tens_max != NULL) {
        temp = doente->registo_tens_max;
        doente->registo_tens_max = doente->registo_tens_max->prox_tensao_max;
        free(temp);
    }
}