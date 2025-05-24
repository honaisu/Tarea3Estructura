#include "gameset.h"
#include "mostrar.h"

unsigned short es_juego = 0;

#define _TIEMPO 60
#define VER_ESTADO(player) limpiar_pantalla(); mostrar_estado_actual(player, jugador->sala_actual)
#define FORMULA_TIEMPO (jugador->peso_total + 1) * 0.1

// puts("*Intentas acabar con tu vida*\n
//      *Al intentarlo, rompes el espacio-tiempo y vuelves a un estado nunca visto*\n
//      *Ves como el mundo se resetea y deforma con el paso de los nanosegundos*\n
//      *Terminas sin conciencia*\n FIN DE LA PARTIDA")

void jugador_recoger(Player*);
void jugador_descartar(Player*);
void jugador_avanzar(Player*, Map*);

Item* item_buscar(List* items, const char* nombre) {
    Item* item;
    for (CADA_RECORRIDO(item, items)) {
        if (!strcmp(item->nombre, nombre)) return item;
    }
    return NULL;
}

void jugador_recoger(Player* jugador) {
    State_Map* sala = jugador->sala_actual;
    if (list_first(sala->items) == NULL) { puts("*Intentas ver si hay items útiles en la habitación, pero no hay nada*"); return; }
    
    mostrar_items(sala->items);
    char entrada[100];
    leer_entrada(entrada);
    
    Item* items_sala = item_buscar(sala->items, entrada);
    if (items_sala == NULL) { puts("*Intentaste buscar un item que no existia en la habitación*"); return; }
        
    list_pushBack(jugador->items, items_sala);
    jugador->peso_total += items_sala->peso;
    jugador->puntaje += items_sala->valor;
    list_popCurrent(sala->items);
    --(jugador->tiempo);
    return;
}

void jugador_descartar(Player* jugador) {
    if (list_first(jugador->items) == NULL) { puts("*Intentas abrir tu mochila para descargar objetos, pero no tienes ninguno*"); return; }

    mostrar_items(jugador->items);
    char entrada[100];
    leer_entrada(entrada);
    
    Item* items_jugador = item_buscar(jugador->items, entrada);
    if (items_jugador == NULL) { puts("*Intentaste sacar un objeto que no existia en tu mochila*\n*-1 de estilo*"); return; }
        
    jugador->peso_total -= items_jugador->peso;
    jugador->puntaje -= items_jugador->valor;
    list_popCurrent(jugador->items);
    --(jugador->tiempo);
    return;
}

void jugador_avanzar(Player* jugador, Map* mapa_juego) {
    limpiar_pantalla();
    imprimir_separador("Decides ver las habitaciones adjuntas a la actual", 50);
    mostrar_conexiones(jugador->sala_actual->adj_nodes);
    Map* conexiones = map_create(is_equal_str);
    State_Map* lista_conexiones = list_first(jugador->sala_actual->adj_nodes);
    while (lista_conexiones != NULL) {
        map_insert(conexiones, lista_conexiones->nombre, lista_conexiones);
        lista_conexiones = list_next(jugador->sala_actual->adj_nodes);
    }
    char entrada[100];
    puts("¿A qué habitación decides moverte? (Ninguna: \"0\")");
    MapPair* a;
    do {    
        leer_entrada(entrada);
        if (!strcmp(entrada, "0")) { puts("*Decides quedarte en la misma habitación*"); return; }
    } while((a = map_search(conexiones, entrada)) == NULL);

    limpiar_pantalla();
    jugador->sala_actual = a->value;
    printf("Decides moverte a esta habitación: \033[1;37m%s\033[0m\n", jugador->sala_actual->nombre);
    jugador->tiempo -= FORMULA_TIEMPO;
    free(conexiones);
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
  } while (o != '*' && jugador->tiempo > 0);
  return;
}

void jugador_default(Player* jugador){
    jugador->tiempo = _TIEMPO;
    jugador->sala_actual = NULL;
    jugador->puntaje = 0;
    jugador->peso_total = 0;
    (jugador->items == NULL) ? jugador->items = list_create() : list_clean(jugador->items);
}

Player* inicializar_jugador(void) {
    Player* nuevo_jugador = (Player*) malloc(sizeof(Player));
    nuevo_jugador->items = NULL;
    jugador_default(nuevo_jugador);
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
