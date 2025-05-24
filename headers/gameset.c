#include "gameset.h"
#include "mostrar.h"

// MACRO correspondiente al valor del tiempo del jugador.
#define _TIEMPO 30 
// MACRO usado para mostrar el estado actual de la sala. Limpia la pantalla y muestra los datos correspondientes.
// Sólo es usado para poder ver el estado de la sala actual del jugador.
// El parámetro "player" se encarga de reemplazar el valor en la función para saber si es necesario mostrar datos del jugador o no.
#define VER_ESTADO(player) limpiar_pantalla(); mostrar_estado_actual(player, jugador->sala_actual)
// MACRO encargado de la fórmula del tiempo [(Peso Total + 1) / 10].
// Optimizado para no usar división (equivalencia de 1/10 con 0.1)
#define FORMULA_TIEMPO (jugador->peso_total + 1) * 0.1

unsigned short es_final = 0;

//--- Funciones Principales del Jugador ---//

void jugador_recoger(Player*);
void jugador_descartar(Player*);
void jugador_avanzar(Player*, Map*);
void resetear_partida(Player*, Map*);
void salir_partida(char*);

//--- Funciones Varias del Jugador ---//

Player* inicializar_jugador(void);
void jugador_default(Player*);
Item* item_buscar(List*, const char*);
void jugador_actualizar_items(Player*, List*, Item*, const int);

// --- //

void jugador_recoger(Player* jugador) {
    State_Map* sala = jugador->sala_actual;
    if (list_first(sala->items) == NULL) { puts("*Intentas ver si hay items útiles en la habitación, pero no hay nada*"); return; }
    
    mostrar_items(sala->items);
    char entrada[100];
    leer_entrada(entrada);
    
    Item* items_sala = item_buscar(sala->items, entrada);
    if (items_sala == NULL) { puts("*Intentaste buscar un item que no existia en la habitación*"); return; }
        
    jugador_actualizar_items(jugador, sala->items, items_sala, 1);
    return;
}

void jugador_descartar(Player* jugador) {
    if (list_first(jugador->items) == NULL) { puts("*Intentas abrir tu mochila para descargar objetos, pero no tienes ninguno*"); return; }

    mostrar_items(jugador->items);
    char entrada[200];
    leer_entrada(entrada);
    
    Item* items_jugador = item_buscar(jugador->items, entrada);
    if (items_jugador == NULL) { puts("*Intentaste sacar un objeto que no existia en tu mochila*\n*-1 de estilo*"); return; }
        
    jugador_actualizar_items(jugador, jugador->items, items_jugador, -1);
    return;
}

void jugador_avanzar(Player* jugador, Map* mapa_juego) {
    limpiar_pantalla();
    imprimir_separador("Decides ver las habitaciones adjuntas a la actual", 50);
    mostrar_conexiones(jugador->sala_actual->adj_nodes);

    // --- //
    Map* conexiones = map_create(is_equal_str);
    State_Map* lista_conexiones;
    for (CADA_RECORRIDO(lista_conexiones, jugador->sala_actual->adj_nodes)) {
        map_insert(conexiones, lista_conexiones->nombre, lista_conexiones);
    }

    // --- //
    char entrada[200];
    puts("¿A qué habitación decides moverte? (Ninguna: \"0\")");
    MapPair* a;
    do {    
        leer_entrada(entrada);
        if (!strcmp(entrada, "0")) { puts("*Decides quedarte en la misma habitación*"); return; }
    } while((a = map_search(conexiones, entrada)) == NULL);

    // --- //
    limpiar_pantalla();
    jugador->sala_actual = a->value;
    if (jugador->sala_actual->final == 'S') { 
        mostrar_mensaje_final(jugador);
        es_final = 1;
    } else {
        printf("Decides moverte a esta habitación: \033[1;37m%s\033[0m\n", jugador->sala_actual->nombre);
        jugador->tiempo -= FORMULA_TIEMPO;
    }
    free(conexiones);
    return;
}

void resetear_partida(Player* jugador, Map* mapa_juego) {
    limpiar_pantalla();
    imprimir_separador("¿Quierés resetear la partida actual (PERDERÁS TODO EL PROGRESO)? [S/N]", 75);
    char o;
    leer_opcion(&o);
    if (o == 'N') {
        puts("Decidiste quedarte en la misma partida.");
        return;
    } else if (o != 'S') {
        puts("*Te das cuenta que decidir fue muy abrumador*");
        puts("*No haces nada*");
        return;
    }

    // --- //
    map_clean(mapa_juego);
    mapa_cargado = 0;
    leer_mapa_completo(mapa_juego);
    jugador_default(jugador);
    MapPair* pair = map_first(mapa_juego);
    jugador->sala_actual = pair->value;
    mostrar_reseteo();
}

void salir_partida(char* o) {
    limpiar_pantalla();
    imprimir_separador("¿Estás seguro de querer salir de la partida? [S/N]", 60);
    leer_opcion(o);
    if (*o == 'S') {
        *o = '0';
        imprimir_separador("FIN DE LA PARTIDA", 60);
    } else if (*o != 'N') {
        puts("*Intentaste pensar si salir valdría la pena o no*");
        puts("*No haces nada*");
    } else puts("No decidiste sucumbir a la tentación.");
}

void casos_opciones(char* o, Player* jugador, Map* mapa_juego) {
    switch (*o) {
        case '1': { jugador_recoger(jugador); break; }
        case '2': { jugador_descartar(jugador); break; }
        case '3': { jugador_avanzar(jugador, mapa_juego); *o = '\0'; break; }
        case '4': { VER_ESTADO(NULL); break; }

        case '-': { resetear_partida(jugador, mapa_juego); break; }
        case '0': { salir_partida(o); break; }
        // Respuesta predeterminada
        default: { puts("*Intentaste usar una opción no disponible*\n*No surgió efecto*"); *o = 'd'; }
    }
    if (*o != '0' && !es_final) esperar_enter();
    if (*o == '\0' && !es_final) { VER_ESTADO(jugador); esperar_enter(); }
}

void pantalla_jugador(Player* jugador, Map* mapa_juego) {
  MapPair* pair = map_first(mapa_juego);
  jugador->sala_actual = pair->value;

  char o = '\0';
  do {
    mostrar_menu_jugador();
    leer_opcion(&o);
    casos_opciones(&o, jugador, mapa_juego);
  } while (o != '0' && jugador->tiempo > 0 && !es_final);

  if (!(jugador->tiempo > 0)) mostrar_mensaje_derrota();
  return;
}

Item* item_buscar(List* items, const char* nombre) {
    Item* item;
    for (CADA_RECORRIDO(item, items)) {
        if (!strcmp(item->nombre, nombre)) return item;
    }
    return NULL;
}

void jugador_actualizar_items(Player* jugador, List* lista, Item* item, const int signo) {
    if (signo > 0) list_pushBack(jugador->items, item);
    list_popCurrent(lista);

    jugador->peso_total += signo * item->peso;
    jugador->puntaje += signo * item->valor;
    --(jugador->tiempo);
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
    if (!mapa_cargado) { puts("NO SE HA CARGADO EL LABERINTO"); return; }
    Player* jugador = inicializar_jugador();

    limpiar_pantalla();
    mostrar_historia();
    pantalla_jugador(jugador, mapa_juego);
    jugador_default(jugador);
    free(jugador);
    return;
}
