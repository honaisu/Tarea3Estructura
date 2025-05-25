#include "headers/gameset.h"
// La jerarquización de los headers es la siguiente:
// 1. Librerías    (stdlib; list; map;...)               [1. -> 2.]
// 2. Extras       (Funciones que utilizo para trabajar) [2. -> 3.]
// 3. GraphQuest   (Funciones del Mapa)                  [3. -> 4.]
// 4. GameSet      (Funciones del Juego)                 [4. -> main.c]

// Simple: gcc main.c headers/*.c headers/TDAs/*.c -o main
// Debug: gcc -g main.c headers/*.c headers/TDAs/*.c -o main

void casos_opciones_inicio(char*, Map*);
void pantalla_inicio(Map*);
void limpiar_grafo(Map*);

// --- //

void limpiar_grafo(Map* grafo) {
    MapPair* pair = map_first(grafo);
    State_Map* sala;
    while (pair != NULL) {
        sala = pair->value;
        list_clean(sala->adj_nodes);
        list_clean(sala->items);
        free(sala->adj_nodes);
        free(sala->items);
        pair = map_next(grafo);
    }
    map_clean(grafo);
    free(grafo);
}

void casos_opciones_inicio(char* o, Map* mapa_juego) {
    leer_opcion(o);
    switch (*o) {
        // Leer y cargar el laberinto del juego
        case '1': { leer_mapa_completo(mapa_juego, 'N'); break; }
        // Jugar al juego
        case '2': { jugar_juego(mapa_juego); break; }
        // Salida del programa
        case '0': { puts("Saliendo del programa..."); break; }
        // Respuesta predeterminada
        default: puts("Por favor, elija una opción."); 
    }
    if (*o != '0') esperar_enter(); 
}

void pantalla_inicio(Map* mapa_juego) {
    limpiar_pantalla();
    esperar_enter();
    char o;
    do {
        mostrar_menu_principal();
        casos_opciones_inicio(&o, mapa_juego); 
    } while (o != '0');
}

int main(void) {
    Map* grafo_del_juego = map_create(is_equal_str);
    pantalla_inicio(grafo_del_juego);
    limpiar_grafo(grafo_del_juego);
    return 0;
}