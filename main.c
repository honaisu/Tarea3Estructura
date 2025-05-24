#include "headers/gameset.h"

// Simple: gcc main.c headers/*.c -o main

// Debug: gcc -g main.c headers/*.c -o main

#define ES_OPCION_VALIDA (o != '0')

extern unsigned short mapa_cargado; 

void pantalla_inicio(Map*);
void jugar_juego(Map*);
void pantalla_jugador(void);

void pantalla_inicio(Map* mapa_juego) {
    char o;
    do {
        mostrar_menu_principal();
        leer_opcion(&o);
        switch (o) {
            case '1': { leer_mapa_completo(mapa_juego); break; }
            case '2': { jugar_juego(mapa_juego); break; }
            // Dato default
            case '0': { puts("Saliendo del programa..."); break; }
            // Respuesta predeterminada
            default: puts("Por favor, elija una opción"); 
        }
        if (ES_OPCION_VALIDA) esperar_enter(); 
    } while (ES_OPCION_VALIDA);
}

int main() {
    Map* grafo_del_juego = map_create(is_equal_str);
    limpiar_pantalla();
    esperar_enter();
    pantalla_inicio(grafo_del_juego);
    return 0;
}