#ifndef GRAPHQUEST_H
#define GRAPHQUEST_H
#include "extras.h"

typedef struct {
  char* nombre;
  int valor;
  int peso;
} Item;

typedef struct {
    int ID;
    char* nombre;
    char* descripcion;
    List* items;
    List* adj_nodes;
    char final;
} State_Map;

void leer_escenarios(Map* );
void mostrar_menu_principal();

#endif