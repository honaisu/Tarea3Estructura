#include "gameset.h"
#include "mostrar.h"

void mostrar_items(List* items) {
  if (list_first(items) == NULL) return;
  puts("\033[1;35mITEMS:\033[0m");
  Item* item;
  for(CADA_RECORRIDO(item, items)) {
    printf("   -> %s (%d pts, %d kg)\n", item->nombre, item->valor, item->peso);
  }
}

void mostrar_conexiones(List* adyacentes) {
  State_Map* sala;
  puts("\033[1;33mCONEXIONES:\033[0m");
  for(CADA_RECORRIDO(sala, adyacentes)) {
    printf("   -> \033[1;37m%s\033[0m\n", sala->nombre);
  }
}

void mostrar_datos(const State_Map* estado) {
  printf("\033[1;34mNOMBRE:\033[0m %s\n", estado->nombre);
  printf("\033[1;32mDESCRIPCION:\033[0m %s\n", estado->descripcion);
  mostrar_items(estado->items);
  mostrar_conexiones(estado->adj_nodes);
  if (estado->final == 'S') puts("Es Final!\n");
  putchar('\n');
}

void mostrar_historia() {
    imprimir_separador("GraphQuest: Journey of a Deadman's Tale", 50);
    puts("* historia * wow muy interesante narrativa");
    puts("  * mas historia ** ** *** ");
    puts("un poco más de historia *+ * ** *");
    esperar_enter();
}

#define COLOR_ESTA_DISPONIBLE(variable) (variable) ? printf("\033[1;30m") : printf("\033[0m")

void mostrar_menu_principal() {
    limpiar_pantalla();
    imprimir_separador("GRAPHQUEST", 30);
    COLOR_ESTA_DISPONIBLE(mapa_cargado);
    puts("(1). Cargar Laberinto");
    COLOR_ESTA_DISPONIBLE(!mapa_cargado) ;
    puts("(2). Iniciar Partida");
    printf("\033[0m");
    puts("(0). Salir") ;
}

void mostrar_menu_jugador() {
    limpiar_pantalla();
    imprimir_separador("Menú del jugador", 25);
    puts("(1). Recoger Item(s)");
    puts("(2). Descartar Item(s)");
    puts("(3). Avanzar a una habitación");
    puts("(4). Ver datos de la habitación");
    puts("(5). Reiniciar Partida");
    puts("(*). Salir de la Partida") ;
}

void mostrar_estado_actual(Player* jugador, State_Map* estado) {
    imprimir_separador("Te encuentras en la siguiente habitación", 50);
    mostrar_datos(estado);
    if (jugador == NULL) return;

    imprimir_separador("De momento, te encuentras con las siguientes estadísticas", 50);
    printf("Tiempo Restante: %d\n", jugador->tiempo);
    
    Item* item;
    for (CADA_RECORRIDO(item, jugador->items)) {
        printf("   ->%s", item->nombre);
    }
    if (item == NULL) puts("Actualmente, no posees ningún item.");

    free(item);
}