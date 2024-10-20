#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <ctype.h>
#include "../lib/funcoes.h"
#include "../lib/doentes.h"
#define BUF 1024

int menu(){
    int menu = 0;
    char line[BUF];
    asciiArt();
    printf("***********MENU***********\n");
    printf("[1] Inserir novo doente\n");
    printf("[2] Eliminar doente existente\n");
    printf("[3] Lista de doentes por ordem alfabética\n");
    printf("[4] Lista de doentes com tensão máxima acima de um determinado valor\n");
    printf("[5] Informação sobre um determinado doente\n");
    printf("[6] Registar novas informações de um determinado doente num determinado dia\n");
    printf("[7] Terminar\n");
    do {
        printf("Opção: ");
        if (fgets(line, sizeof(line), stdin)) {
            line[strcspn(line, "\n")] = '\0';
            menu = my_atoi(line);
        }
    }while(menu == INT_MIN || menu > 7 || menu < 1);
    return menu;
}

int compDates(Data d1, Data d2){
    if (d1.ano != d2.ano) {
        return d1.ano - d2.ano;
    } else if (d1.mes != d2.mes) {
        return d1.mes - d2.mes;
    } else {
        return d1.dia - d2.dia;
    }
}

int my_atoi(char line[]){
    int num = 0;
    int size = strlen(line);
    for (int i = 0; i < size; i++) {
        if (line[i] < '0' || line[i] > '9'){
            return INT_MIN;
        } else {
            num = num * 10 + (line[i] - '0');
        }
    }
    return num;
}

int my_atoi_ficheiros(char line[]){
    int num = 0;
    int size = strlen(line);
    for (int i = 0; i < size; i++) {
        if(iscntrl(line[i])){
            printf("Caracter especial\n");
            continue;
        }
        if (line[i] < '0' || line[i] > '9'){
            return INT_MIN;
        } else {
            num = num * 10 + (line[i] - '0');
        }
    }
    return num;
}

int verifica_nome(char line[]){
    int size = strlen(line);
    if(size == 0){
        printf("Nome vazio\n");
        return 0;
    }
    if(line[0] == ' ' || line[size - 1] == ' '){
        printf("Nome começa ou acaba com espaços\n");
        return 0;
    }

    for (int i = 0; i < size; i++) {
        //Dois espaços consecutivos
        if (i > 0 && line[i] == ' ' && line[i - 1] == ' ') {
            printf("Nome com dois espaços consecutivos\n");
            return 0;
        }
        //Duas letras Maiusculas consecutivas
        if ((line[i] >= 'A' && line[i] <= 'Z') && (line[i + 1] >= 'A' && line[i + 1] <= 'Z') ) {
            printf("Nome com duas letras maisculas consecutivas\n");
            return 0;
        }
        //Se não for um Caracter Especial
        if(!((line[i] >= 'A' && line[i] <= 'Z') || (line[i] >= 'a' && line[i] <= 'z') || line[i] == ' ')){
            printf("Nome contém algum caracter não permitido\n");
            return 0;
        }
        //Nomes com letras maiúsculas
        if(i == 0 || line[i - 1] == ' '){
            if(line[i] < 'A' || line[i] > 'Z'){
                printf("Nome não começa com letra maiúscula\n");
                return 0;
            }
        }
    }
    return 1;
}

Data converteString_Data(char* line){
    Data data;
    data.dia = 99;
    data.mes = 99;
    data.ano = 9999;
    char *token = NULL;
    char* line_copy = strdup(line);

    token = strtok(line_copy, "/");
    if (token == NULL) {
        printf("A data não está no formato correto.\n");
        return data;
    }
    data.dia = my_atoi_ficheiros(token);

    token = strtok(NULL, "/");
    data.mes = my_atoi_ficheiros(token);

    token = strtok(NULL, "/");
    data.ano = my_atoi_ficheiros(token);

    free(line_copy);

    return data;
}

int verifica_altura(char line[]){
    int altura = my_atoi(line);
    if(altura == INT_MIN || altura < 30 || altura > 250){
        return 0;
    }else{
        return 1;
    }
}

int verifica_peso(char line[]){
    int peso = my_atoi(line);
    if(peso == INT_MIN || peso < 1 || peso > 200){
        return 0;
    }else{
        return 1;
    }
}

int verifica_Data(char line[]){
    int size = strlen(line);
    Data data_nasci;
    if(size == 0){
        printf("Data Vazia\n");
        return 0;
    }
    if(line[0] == ' ' || line[size - 1] == ' ') {
        printf("Data começa ou acaba com um espaço\n");
        return 0;
    }
    int bars = 0;

    for (int i = 0; i < size; i++) {
        if((line[i] >= 'A' && line[i] <= 'Z') && (line[i] >= 'a' && line[i] <= 'z') && line[i] != '/') {
            printf("A data contém caracteres inválidos\n");
            return 0;
        }

        if(line[i] == '/') bars++;
    }

    if (bars != 2) {
        printf("Formato incorreto\n");
        return 0;
    }

    data_nasci = converteString_Data(line);

    if(data_nasci.dia < 1 || data_nasci.dia > 31){
        printf("%d",data_nasci.dia );
        printf(" Dia invalido\n");
        return 0;
    }
    if(data_nasci.mes < 1 || data_nasci.mes > 12){
        printf("mes invalido\n");
        return 0;}
    if(data_nasci.ano < 1900 || data_nasci.ano > 2024){
        printf("ano invalido\n");
        return 0;}

    return 1;
}

int verifica_cc(Doente* head, char line[]){
    int size = strlen(line);
    if(size != 14) return 0;

    for (int i = 0; i < size; i++){
        if(i == 8 || i == 10) {
            if(line[i] != '-'){
                printf("Formato incorreto\n");
                return 0;
            }
        } else if(i == 11 || i == 12) {
            if(!(line[i] >= 'A' && line[i] <= 'Z')){
                printf("Digitos de Controlo no formato incorreto\n");
                return 0;
            }
        } else {
            if(!(line[i] >= '0' && line[i] <= '9')){
                printf("Formato incorreto\n");
                return 0;}
        }
    }
    //Informação Duplicada
    Doente* temp = head->prox;
    while(temp != NULL){
        if (strcmp(temp->inform.cc, line) == 0) {
            printf("Número de CC encontrado em outro Paciente\n");
            return 0;
        }
        temp = temp->prox;
    }
    return 1;
}

int verifica_telefone(Doente* head, char line[]){
    int size = strlen(line);
    if(size != 9) return 0;
    for(int i = 0; i < size; i++){
        if(!(line[i] >= '0' && line[i] <= '9')){
            printf("Número inválido\n");
            return 0;
        }
    }
    //Informação Duplicada
    Doente* temp = head->prox;
    while(temp != NULL){
        if (strcmp(temp->inform.telefone, line) == 0) {
            printf("Número de telemóvel encontrado em outro Paciente\n");
            return 0;
        }
        temp = temp->prox;
    }
    return 1;
}

int verifica_email(Doente* head, char line[]){
    //user@mail.exte
    int size = strlen(line);
    if(size == 0) return 0;
    int arroba = -1;
    int ponto = -1;
    for (int i = 0; i < size; i++){
        if(line[i] == '@'){
            if(arroba != -1) return 0;
            arroba = i;
        } else if(line[i] == '.'){
            ponto = i;
        }
    }
    if(arroba == -1 || ponto == -1) return 0;
    if(arroba > ponto) return 0;
    if(arroba == 0 || ponto == 0 || ponto == size - 1) return 0;
    //Informação Duplicada
    Doente* temp = head->prox;
    while(temp != NULL){
        if (strcmp(temp->inform.email, line) == 0) {
            printf("Email encontrado em outro Paciente\n");
            return 3;
        }
        temp = temp->prox;
    }
    return 1;
}

int verificaDadosDuplicados(Doente* head, Info dados){
    Doente* temp = head->prox;
    while(temp!=NULL){
        if (strcmp(temp->inform.cc, dados.cc) == 0 && strcmp(temp->inform.telefone, dados.telefone) == 0 && strcmp(temp->inform.email, dados.email) == 0) {
            return 1;
        }
        temp = temp->prox;
    }
    return 0;
}

int verifica_id(Doente* head, char line[]){
    int id = my_atoi(line);
    if(id == INT_MIN){
        return 0;
    }else{
        Doente* temp = head->prox;
        while(temp!=NULL){
            if (temp->id == id) {
                return 1;
            }
            temp = temp->prox;
        }
        return 0;
    }
}

int pedir_id(Doente* head){
    int id;
    char line[BUF];
    int valid;
    do {
        printf("Introduza o ID: \n");
        if (fgets(line, sizeof(line), stdin)) {
            line[strcspn(line, "\n")] = '\0';
            valid = verifica_id(head, line);
        }
        if(valid == 0){
            printf("ID inválido\n");
        }else{
            id = my_atoi(line);
        }
    }while(valid == 0);

    return id;
}

int verifica_tensao(char line[]){
    int tens = my_atoi(line);
    if(tens == INT_MIN || tens < 0 || tens > 220){
        return 0;
    }else{
        return 1;
    }
}

int pedir_TensMax(){
    int tens_max;
    char line[BUF];
    int valid;
    do {
        printf("Introduza o valor da Tensão Máxima: \n");
        if (fgets(line, sizeof(line), stdin)) {
            line[strcspn(line, "\n")] = '\0';
            valid = verifica_tensao(line);
        }
        if(valid == 0){
            printf("Valor de Tensão inválido\n");
        }else{
            tens_max = my_atoi(line);
        }
    }while(valid == 0);

    return tens_max;
}

char* pedir_nome(Doente* head) {
    char* nome = malloc(BUF * sizeof(char));
    if(nome == NULL){
        printf("Erro: Não foi possível alocar memória para o nome.\n");
        return NULL;
    }
    char line[BUF];
    int valid;
    do {
        printf("Introduza o nome do doente: \n");
        if (fgets(line, sizeof(line), stdin)) {
            line[strcspn(line, "\n")] = '\0';
            valid = verifica_nome(line);
            Doente *prev, *cur;
            search_name(&head, line, &prev, &cur);
            if (cur == NULL) {
                valid = 0;
            }
        }
        if(valid == 0){
            printf("Nome inválido\n");
        }
    }while(valid == 0);
    strcpy(nome, line);
    return nome;
}


int verifica_opcao(char line[]){
    int opcao = my_atoi(line);
    if(opcao == INT_MIN || opcao < 1 || opcao > 2){
        return 0;
    }if(opcao == 1){
        return 1;
    }if(opcao == 2){
        return 2;
    }
    return 0;
}

int pedirNomeouID(){
    char line[BUF];
    int opcao = 0;
    do {
        printf("Escolha uma das opções para procurar: \n");
        printf("Para utilizar o ID selecione: 1\n");
        printf("Para utilizar o Nome selecione: 2\n");
        if (fgets(line, sizeof(line), stdin)) {
            line[strcspn(line, "\n")] = '\0';
            opcao = verifica_opcao(line);
        }
        if(opcao == 0){
            printf("Opção inválida\n");
        }
    }while(opcao == 0);
    return opcao;
}

void init_special_node(Doente **head){
    (*head)->id = 0;
    strcpy((*head)->nome, "Especial");
    (*head)->data_nasc.dia = 0;
    (*head)->data_nasc.mes = 0;
    (*head)->data_nasc.ano = 0;
    strcpy((*head)->inform.cc, "");
    strcpy((*head)->inform.telefone, "");
    strcpy((*head)->inform.email, "");
    (*head)->registo_data = NULL;
    (*head)->registo_tens_max = NULL;
    (*head)->prox = NULL;
    (*head)->prox_tens_max = NULL;
}

int maior_ID(Doente* head){
    int maior_id = 1;
    Doente* temp = head->prox;
    while (temp != NULL) {
        if(temp->id > maior_id){
            maior_id = temp->id;
        }
        temp = temp->prox;
    }
    return maior_id + 1;
}

void asciiArt(){
    printf("\n"
    "                                                                                \n"
    "              _____                    _____                    _____           \n"
    "             /\\    \\                  /\\    \\                  /\\    \\          \n"
    "            /::\\    \\                /::\\    \\                /::\\    \\         \n"
    "           /::::\\    \\              /::::\\    \\              /::::\\    \\        \n"
    "          /::::::\\    \\            /::::::\\    \\            /::::::\\    \\       \n"
    "         /:::/\\:::\\    \\          /:::/\\:::\\    \\          /:::/\\:::\\    \\      \n"
    "        /:::/__\\:::\\    \\        /:::/__\\:::\\    \\        /:::/__\\:::\\    \\     \n"
    "       /::::\\   \\:::\\    \\      /::::\\   \\:::\\    \\      /::::\\   \\:::\\    \\    \n"
    "      /::::::\\   \\:::\\    \\    /::::::\\   \\:::\\    \\    /::::::\\   \\:::\\    \\   \n"
    "     /:::/\\:::\\   \\:::\\____\\  /:::/\\:::\\   \\:::\\____\\  /:::/\\:::\\   \\:::\\____\\  \n"
    "    /:::/  \\:::\\   \\:::|    |/:::/  \\:::\\   \\:::|    |/:::/  \\:::\\   \\:::|    | \n"
    "    \\::/    \\:::\\  /:::|____|\\::/    \\:::\\  /:::|____|\\::/    \\:::\\  /:::|____| \n"
    "     \\/_____\\/:::\\/:::/    /  \\/_____\\/:::\\/:::/    /  \\/_____\\/:::\\/:::/    /  \n"
    "              \\::::::/    /            \\::::::/    /            \\::::::/    /   \n"
    "               \\::::/    /              \\::::/    /              \\::::/    /    \n"
    "                \\::/____/                \\::/____/                \\::/____/     \n"
    "                 ~~                       ~~                       ~~           \n"
    "                                                                                \n");
    printf("Bem-vindo Doutor!\n");
}