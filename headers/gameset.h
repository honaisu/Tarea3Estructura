#ifndef GAMESET_H
#define GAMESET_H
#include "graphquest.h"

typedef struct Player{
  List* items;
  State_Map* sala_actual;
  int tiempo;
  int puntaje;
  int peso_total;
} Player;

extern unsigned short es_juego;

void jugar_juego(Map*);
void mostrar_menu_principal(void);

#endif