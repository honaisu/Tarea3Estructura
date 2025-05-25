#ifndef GAMESET_H
#define GAMESET_H
#include "graphquest.h"

// Estructura que posee las componentes esenciales que utiliza el jugador.
// Está conformado por un inventario (Lista), su sala actual y datos referentes al tiempo, puntaje y peso total.
typedef struct Player{
  List* items; // Lista de Objetos. Usado para mantener un inventario del jugador con los elementos que posee.
  State_Map* sala_actual; 
  int tiempo; // Tiempo restante del jugador. Avanza con cada acción que haga el jugador. La cantidad de tiempo restado dependerá de la acción.
  int puntaje; // Puntaje total del jugador. Depende de la cantidad de Objetos que lleve el jugador.
  int peso_total; // Peso total del jugador. Depende del peso de los Objetos que lleve el jugador.
} Player;

// Se encarga de la funcionalidad general de todo el juego al iniciar una partida.
// Es la función que permite iniciar la partida, mostrar el menú del jugador, y terminar la partida. 
void jugar_juego(Map*);
// Muestra el menú principal de la pantalla de inicio (no el del jugador).
void mostrar_menu_principal(void);

#endif