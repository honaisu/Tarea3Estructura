#ifndef MOSTRAR_H
#define MOSTRAR_H

typedef struct Player Player;
typedef struct List List;
typedef struct State_Map State_Map;

void mostrar_menu_principal(void);
// Muestra Lista Items
void mostrar_items(List*);
// Mostrar las conexiones (salas adjuntas) de un nodo estado (sala).
void mostrar_conexiones(List*); 
// Mostrar datos de un nodo estado (sala).
void mostrar_datos(const State_Map*);
// Función que toma dos parámetros: el jugador y una sala.
// Permite poder mostrar los parámetros sólo del jugador o sólo de la sala.
// Si el jugador es nulo, muestra sólo los datos de la sala (similar a mostrar_datos).
// Si la sala es nula, muestra sólo los datos del jugador.
void mostrar_estado_actual(Player*, const State_Map*);

// Historia al iniciar partida
void mostrar_historia(void);
void mostrar_menu_jugador(void);
// Mensaje en caso de resetear
void mostrar_reseteo(void);
void mostrar_mensaje_final(Player*);
void mostrar_mensaje_derrota(void);

#endif