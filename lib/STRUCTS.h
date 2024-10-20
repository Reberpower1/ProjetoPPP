#ifndef STRUCTS
#define STRUCTS

typedef struct Data{
    int dia;
    int mes;
    int ano;
}Data;

typedef struct Info{
    char cc[15];
    char telefone[10];
    char email[100];
}Info;

typedef struct Registos {
    Data data;
    int tensao_min;
    int tensao_max;
    int peso;
    int altura;
    struct Registos* prox_data;
    struct Registos* prox_tensao_max;
} Registos;

typedef struct Doente {
    int id;
    char nome[100];
    Data data_nasc;
    Info inform;
    Registos* registo_data;
    Registos* registo_tens_max;
    struct Doente* prox;
    struct Doente* prox_tens_max;
} Doente;

#endif