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
    int direcciones[4];
    List* adj_nodes;
    char final;
} State_Map;

typedef struct {
  List* items;
  State_Map* sala_actual;
  int tiempo;
} Player;

void leer_mapa_completo(Map*);
void mostrar_menu_principal();

#endif