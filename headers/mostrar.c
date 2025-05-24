#include "gameset.h"
#include "mostrar.h"

// MACRO que sólo se encarga de cambiar el color de una palabra para mostrar si está disponible o no.
#define COLOR_ESTA_DISPONIBLE(variable) (variable) ? printf("\033[1;31m") : printf("\033[0m")

/*
\033[0m : Color Predeterminado.
\033[1;Xm : Negrita con X siendo algún color.
*/

void mostrar_items(List* items) {
  if (list_first(items) == NULL) return;
  puts("\033[1;35mITEMS:\033[0m");
  Item* item;
  for(CADA_RECORRIDO(item, items)) {
    printf("   -> %s (%d pts, %d kg)\n", item->nombre, item->valor, item->peso);
  }
}

void mostrar_conexiones(List* adyacentes) {
  if (list_first(adyacentes) == NULL) return;
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

void mostrar_historia(void) {
    imprimir_separador("GraphQuest: Journey of a Deadman's Tale", 50);
    puts("* historia * wow muy interesante narrativa");
    puts("  * mas historia ** ** *** ");
    puts("un poco más de historia *+ * ** *");
    esperar_enter();
}

void mostrar_menu_principal(void) {
    limpiar_pantalla();
    imprimir_separador("GRAPHQUEST", 30);
    COLOR_ESTA_DISPONIBLE(mapa_cargado);
    puts("(1). Cargar Laberinto");
    COLOR_ESTA_DISPONIBLE(!mapa_cargado) ;
    puts("(2). Iniciar Partida");
    printf("\033[0m");
    puts("(0). Salir") ;
}

void mostrar_menu_jugador(void) {
    limpiar_pantalla();
    imprimir_separador("Menú del Jugador", 30);
    puts("(1). Recoger Item(s)");
    puts("(2). Descartar Item(s)");
    puts("(3). Avanzar a una habitación");
    puts("(4). Ver datos de la habitación\n");
    puts("(-). Reiniciar Partida");
    puts("(0). Salir de la Partida") ;
}

void mostrar_estado_actual(Player* jugador, const State_Map* estado) {
    imprimir_separador("Te encuentras en la siguiente habitación", 50);
    mostrar_datos(estado);
    if (jugador == NULL) return;

    imprimir_separador("De momento, te encuentras con las siguientes estadísticas", 70);
    printf("Tiempo Restante: %d min\n", jugador->tiempo);
    printf("Puntaje Total: %d pts\n", jugador->puntaje);
    printf("Peso Total Actual: %d kg\n", jugador->peso_total);
    
    if (jugador->items == NULL) puts("Actualmente, no posees ningún item.");
    else mostrar_items(jugador->items);

    if (jugador->peso_total > 14) puts("\033[1;37m*Sientes tu cuerpo cada vez más cansado por el peso de tu mochila*\n*Cada paso se vuelve más lento*\033[0m");
}

void mostrar_reseteo(void) {
  imprimir_separador("Reseteaste el mundo que existía a tu alrededor", 60);
  puts("Has vuelto a renacer. Te has vuelto nuevamente uno con esta existencia.");
}