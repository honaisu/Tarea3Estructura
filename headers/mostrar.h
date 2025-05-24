#ifndef MOSTRAR_H
#define MOSTRAR_H

typedef struct Player Player;
typedef struct List List;
typedef struct State_Map State_Map;

void mostrar_items(List*);
void mostrar_conexiones(List*);
void mostrar_datos(const State_Map*);
void mostrar_historia(void);
void mostrar_menu_principal(void);
void mostrar_menu_jugador(void);
void mostrar_estado_actual(Player*, const State_Map*);

#endif