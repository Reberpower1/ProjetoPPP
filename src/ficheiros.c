#include <stdio.h>
#include <string.h>
#include "../lib/ficheiros.h"
#include "../lib/funcoes.h"
#include "../lib/doentes.h"
#include "../lib/registos.h"
#define BUF 1024

void criar_Lista_Doentes_Registos(Doente **head){
    FILE* file = fopen("docs/doentes.txt", "r");
    if(file != NULL){
        int id = 0;
        char line[BUF];
        char nome[BUF];
        Data data;
        char cc[BUF];
        char telefone[BUF];
        char email[BUF];
        int i = 1;
        while (fgets(line, sizeof(line), file)) {
            line[strcspn(line, "\n")] = '\0';
            if(i % 6 == 1){
                id = my_atoi_ficheiros(line);
            }else if(i % 6 == 2){
                strcpy(nome, line);
            }else if(i % 6 == 3){
                data = converteString_Data(line);
            }else if(i % 6 == 4){
                strcpy(cc, line);
            }else if(i % 6 == 5){
                strcpy(telefone, line);
            }else if(i % 6 == 0){
                strcpy(email, line);
                Info dados;
                strcpy(dados.cc, cc);
                strcpy(dados.telefone, telefone);
                strcpy(dados.email, email);
                adicionar_Doente(head, nome, data, dados, id);
            }
            i++;
        }
        fclose(file);
    }else{
        printf("Não foi possível abrir o arquivo doentes.txt\n");
    }

    FILE* registo = fopen("docs/registos.txt", "r");
    if(registo != NULL){
        Doente* doente = NULL;
        int id;
        char line[BUF];
        Data data;
        int tensao_min;
        int tensao_max;
        float peso;
        float altura;
        int i = 1;
        while (fgets(line, sizeof(line), registo)) {
            line[strcspn(line, "\n")] = '\0';
            if(i % 6 == 1){
                id = my_atoi_ficheiros(line);
                Doente* prev;
                Doente* cur;
                search(head, id, &prev, &cur);

                if(cur != NULL){
                    doente = cur;
                }

            }else if(i % 6 == 2){
                data = converteString_Data(line);
            }else if(i % 6 == 3){
                tensao_max = my_atoi_ficheiros(line);
            }else if(i % 6 == 4){
                tensao_min = my_atoi_ficheiros(line);
            }else if(i % 6 == 5){
                peso = my_atoi_ficheiros(line);
            }else if(i % 6 == 0){
                altura = my_atoi_ficheiros(line);
                adicionar_Registo(head, doente, data, tensao_min, tensao_max, altura, peso);
            }
            i++;
        }
        fclose(registo);

    }else{
        printf("Não foi possível abrir o arquivo registos.txt\n");
    }
}

void addToDoentesFile(char* nome, Data data_nasci, Info dados, int id){
    FILE* file = fopen("docs/doentes.txt", "a");
    if (file != NULL) {
        fprintf(file, "%d\n", (id));
        fprintf(file, "%s\n", nome);
        fprintf(file, "%02d/%02d/%04d\n", data_nasci.dia, data_nasci.mes, data_nasci.ano);
        fprintf(file, "%s\n", dados.cc);
        fprintf(file, "%s\n", dados.telefone);
        fprintf(file, "%s\n", dados.email);
        fclose(file);
    } else {
        printf("Não foi possível abrir o arquivo doentes.txt\n");
        fclose(file);
    }
}

void addToRegistosFile(int id, Data data_info, int tens_min, int tens_max, int altura, int peso){
    FILE* file = fopen("docs/registos.txt", "a");
    if (file != NULL) {
        fprintf(file, "%d\n", id);
        fprintf(file, "%d/%d/%d\n", data_info.dia, data_info.mes, data_info.ano);
        fprintf(file, "%d\n", tens_max);
        fprintf(file, "%d\n", tens_min);
        fprintf(file, "%d\n", peso);
        fprintf(file, "%d\n", altura);
        fclose(file);
    } else {
        printf("Não foi possível abrir o arquivo doentes.txt\n");
        fclose(file);
    }
}

void eliminarLinhas(FILE* file, const char* nome_ficheiro, int inicio, int fim){
    FILE* temp = fopen("temp.txt", "w");
    if(file != NULL && temp != NULL){
        char line[BUF];
        for(int i = 1; fgets(line, sizeof(line), file); i++){
            line[strcspn(line, "\n")] = '\0';
            if(i < inicio || i > fim){
                fprintf(temp, "%s\n", line);
            }
        }
    } else{
        printf("Não foi possível abrir o arquivo doentes.txt\n");
        fclose(temp);
        return;
    }
    remove(nome_ficheiro);
    rename("temp.txt", nome_ficheiro);
    fclose(temp);
}

void eliminarDoenteFicheiros(int id){
    FILE* doentes = fopen("docs/doentes.txt", "r");
    char line[BUF];
    int id_atual;
    if (doentes != NULL) {
        for(int i = 1; fgets(line, sizeof(line), doentes); i++){
            line[strcspn(line, "\n")] = '\0';
            id_atual = my_atoi_ficheiros(line);
            if(i % 6 == 1 && id == id_atual){
                fclose(doentes);
                doentes = fopen("docs/doentes.txt", "r");
                eliminarLinhas(doentes, "docs/doentes.txt", i, i + 5);
                fclose(doentes);
                break;
            }
        }
    } else{
        printf("Não foi possível abrir um dos arquivo\n");
    }

    FILE* registos = fopen("docs/registos.txt", "r");
    int found;
    if(registos != NULL){
        do{
            found = 0;
            for(int i = 1; fgets(line, sizeof(line), registos); i++){
                line[strcspn(line, "\n")] = '\0';
                id_atual = my_atoi_ficheiros(line);
                if(i % 6 == 1 && id == id_atual){
                    fclose(registos);
                    registos = fopen("docs/registos.txt", "r");
                    eliminarLinhas(registos, "docs/registos.txt", i, i + 5);
                    fclose(registos);
                    registos = fopen("docs/registos.txt", "r");
                    found = 1;
                    break;
                }
            }
        }while (found);
        fclose(registos);
    }else{
        printf("Não foi possível abrir um dos arquivo\n");
    }
}

void reescreverFicheiros(Doente* head) {
    FILE *doentes = fopen("docs/doentes.txt", "w");
    FILE* registos = fopen("docs/registos.txt", "w");
    if (doentes != NULL && registos != NULL) {
        for (Doente *doente = head->prox; doente != NULL; doente = doente->prox) {
            if(doente->id != 0){
                fprintf(doentes, "%d\n", doente->id);
                fprintf(doentes, "%s\n", doente->nome);
                fprintf(doentes, "%02d/%02d/%04d\n", doente->data_nasc.dia, doente->data_nasc.mes, doente->data_nasc.ano);
                fprintf(doentes, "%s\n", doente->inform.cc);
                fprintf(doentes, "%s\n",doente->inform.telefone);
                fprintf(doentes, "%s\n", doente->inform.email);
                for (Registos* registo = doente->registo_data; registo != NULL; registo = registo->prox_data) {
                    fprintf(registos, "%d\n", doente->id);
                    fprintf(registos, "%d/%d/%d\n", registo->data.dia, registo->data.mes, registo->data.ano);
                    fprintf(registos, "%d\n", registo->tensao_max);
                    fprintf(registos, "%d\n", registo->tensao_min);
                    fprintf(registos, "%d\n", registo->peso);
                    fprintf(registos, "%d\n", registo->altura);
                }
            }
        }
        fclose(doentes);
        fclose(registos);
    } else {
        printf("Não foi possível abrir o arquivo doentes.txt\n");
        fclose(doentes);
        fclose(registos);
    }

}