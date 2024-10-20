#include <stdio.h>
#include <stdlib.h>
#include "../lib/STRUCTS.h"
#include "../lib/doentes.h"
#include "../lib/registos.h"
#include "../lib/ficheiros.h"
#include "../lib/funcoes.h"

int main(){
    int opcao_menu;
    int id;
    int tens_max;
    int opcao;
    Doente* head = (Doente*)malloc(sizeof(Doente));
    if(head == NULL){
        printf("Erro ao alocar memória para a lista de doentes.\n");
        return 1;
    }
    init_special_node(&head);
    criar_Lista_Doentes_Registos(&head);
    do{
        opcao_menu = menu();
        switch (opcao_menu){
            case 1:
                printf("\n");
                printf("Inserir novo doente\n");
                inserir_Doente(&head);
                break;
            case 2:
                printf("Eliminar doente da lista\n");
                opcao = pedirNomeouID();
                if(opcao == 1){
                    id = pedir_id(head);
                    eliminar_Doente(&head, id);
                    reescreverFicheiros(head);
                }else if(opcao == 2){
                    char* nome = pedir_nome(head);
                    Doente* prev;
                    Doente* doente;
                    search_name(&head, nome, &prev, &doente);
                    opcao = imprimir_Doentes_Com_Nome(head, doente->nome);
                    if(opcao > 1){
                        id = pedir_id(head);
                        eliminar_Doente(&head, id);
                        reescreverFicheiros(head);
                    }else if (opcao == 1){
                        eliminar_Doente(&head, doente->id);
                        reescreverFicheiros(head);
                    }
                    free(nome);
                }
                break;
            case 3:
                printf("Lista de doentes por ordem alfabética\n");
                imprimirLista(head);
                break;
            case 4:
                printf("Lista de doentes com tensão máxima acima de um determinado valor\n");
                tens_max = pedir_TensMax();
                imprimir_Doentes_Tensao_Maxima(head, tens_max);
                break;
            case 5:
                printf("Informação sobre um determinado doente\n");
                opcao = pedirNomeouID();
                if(opcao == 1){
                    id = pedir_id(head);
                    imprimir_Registos_Doente(head, id);
                }else if(opcao == 2){
                    char* nome = pedir_nome(head);
                    Doente* prev;
                    Doente* doente;
                    search_name(&head, nome, &prev, &doente);
                    opcao = imprimir_Doentes_Com_Nome(head, doente->nome);
                    if(opcao > 1){
                        id = pedir_id(head);
                        imprimir_Registos_Doente(head, id);
                    }else if (opcao == 1){
                        imprimir_Registos_Doente(head, doente->id);
                    }
                    free(nome);
                }
                break;
            case 6:
                printf("Registar novas informações de um determinado doente num determinado dia\n");
                opcao = pedirNomeouID();
                if(opcao == 1){
                    id = pedir_id(head);
                    inserir_Registo(&head, id);
                }else if(opcao == 2){
                    char* nome = pedir_nome(head);
                    Doente* prev;
                    Doente* doente;
                    search_name(&head, nome, &prev, &doente);
                    opcao = imprimir_Doentes_Com_Nome(head, doente->nome);
                    if(opcao > 1){
                        id = pedir_id(head);
                        inserir_Registo(&head, doente->id);
                    }else if (opcao == 1){
                        inserir_Registo(&head, id);
                    }
                    free(nome);
                }
                break;
            case 7:
                printf("\nObrigado pela Utilização\n");
                limparLista(&head);
                free(head);
                break;
        }
    }while(opcao_menu != 7);
    return 0;
}