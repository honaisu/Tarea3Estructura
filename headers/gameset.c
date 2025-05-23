#include "gameset.h"
#include "mostrar.h"

unsigned short es_juego = 0;

#define _TIEMPO 10
#define VER_ESTADO(player) limpiar_pantalla(); mostrar_estado_actual(player, jugador->sala_actual)

// puts("*Intentas acabar con tu vida*\n
//      *Al intentarlo, rompes el espacio-tiempo y vuelves a un estado nunca visto*\n
//      *Ves como el mundo se resetea y deforma con el paso de los nanosegundos*\n
//      *Terminas sin conciencia*\n FIN DE LA PARTIDA")

void jugador_recoger(Player*);
void jugador_descartar(Player*);
void jugador_avanzar(Player*, Map*);

void jugador_recoger(Player* jugador) {
    State_Map* sala = jugador->sala_actual;
    List* items_sala = list_first(sala->items);
    if (items_sala == NULL) { puts("*Intentas ver si hay items útiles en la habitación, pero no hay nada*"); return; }
    
    mostrar_items(items_sala);
    return;
}

void jugador_descartar(Player* jugador) {
    List* items_jugador = list_first(jugador->items);
    if (items_jugador == NULL) { puts("*Intentas abrir tu mochila para descargar objetos, pero no tienes ninguno*"); return; }
    return;
}

void jugador_avanzar(Player* jugador, Map* mapa_juego) {
    limpiar_pantalla();
    imprimir_separador("Decides ver las habitaciones adjuntas a la actual", 50);
    mostrar_conexiones(jugador->sala_actual->adj_nodes);

    char* entrada;
    puts("¿A qué habitación decides moverte? (Ninguna: \"0\")");
    leer_entrada(entrada);

    return;
}

void pantalla_jugador(Player* jugador, Map* mapa_juego) {
  char o = '\0';
  MapPair* pair = map_first(mapa_juego);
  jugador->sala_actual = pair->value;
  do {
    if (o == '\0') { VER_ESTADO(jugador); esperar_enter(); }
    mostrar_menu_jugador();
    leer_opcion(&o);
    switch (o) {
        case '1': { jugador_recoger(jugador); break; }
        case '2': { jugador_descartar(jugador); break; }
        case '3': { jugador_avanzar(jugador, mapa_juego); o = '\0'; break; }
        case '4': { VER_ESTADO(NULL); break; }

        case '5': { break; }
        case '*': { puts("FIN DE LA PARTIDA"); break; }
        // Respuesta predeterminada
        default: { puts("*Intentaste usar una opción no disponible*\n*No surgió efecto*"); o = 'd'; }
    }
    if (o != '*') esperar_enter(); 
  } while (o != '*');
  return;
}

Player* inicializar_jugador(void) {
    Player* nuevo_jugador = (Player*) malloc(sizeof(Player));
    nuevo_jugador->tiempo = _TIEMPO;
    nuevo_jugador->items = list_create();
    nuevo_jugador->sala_actual = NULL;
    nuevo_jugador->puntaje = 0;
    nuevo_jugador->peso_total = 0;
    return nuevo_jugador;
}

void jugar_juego(Map* mapa_juego) {
    if (mapa_cargado) es_juego = 1; 
    else { puts("NO SE HA CARGADO EL LABERINTO"); return; }
    Player* jugador = inicializar_jugador() ;

    limpiar_pantalla();
    mostrar_historia();
    pantalla_jugador(jugador, mapa_juego);
    return;
}
