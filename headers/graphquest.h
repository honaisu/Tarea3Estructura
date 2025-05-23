#ifndef GRAPHQUEST_H
#define GRAPHQUEST_H
#include "extras.h"

#define CADA_RECORRIDO(valor, lista) valor = list_first(lista); valor != NULL; valor = list_next(lista)

extern unsigned short mapa_cargado; 

typedef struct Item{
  char* nombre;
  int valor;
  int peso;
} Item;

typedef struct State_Map{
    int ID;
    char* nombre;
    char* descripcion;
    List* items;
    int direcciones[4];
    List* adj_nodes;
    char final;
} State_Map;

void leer_mapa_completo(Map*);

#endif