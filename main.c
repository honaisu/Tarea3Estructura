#include "headers/graphquest.h"

// Simple: gcc main.c headers/*.c -o main

// Debug: gcc -g main.c headers/*.c -o main

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
        if (o != '0') esperar_enter(); 
    } while (o != '0' && o != '2');
}

void pantalla_jugador() {
    char o;
    leer_opcion(&o);

    do {
        switch (o) {
            case '1': { esperar_enter(); break; }
            case '2': { break; }
            // Dato default
            case '0': { puts("Saliendo del programa..."); break; }
            // Respuesta predeterminada
            default: puts("Por favor, elija una opción"); 
        }
    } while (o != '0');
}

void jugar_juego(Map* mapa_juego) {
    limpiar_pantalla();
    puts("hola mundo");
    return;
}

int main() {
    Map* grafo_del_juego = map_create(is_equal_int);
    limpiar_pantalla();
    esperar_enter();
    pantalla_inicio(grafo_del_juego);

    limpiar_pantalla();
    return 0;
}