#include "graphquest.h"

#define LEER_CAMPO campos = leer_linea_csv(archivo, ',')
#define CADA_CAMPO LEER_CAMPO; campos != NULL; LEER_CAMPO
#define CADA_RECORRIDO(valor, lista)  valor = list_first(lista); valor != NULL; valor = list_next(lista)

void mostrar_items(List*);
void mostrar_conexiones(List*);
void mostrar_datos(const State_Map*);

void* IDs[16];
unsigned short mapa_cargado = 0; 

void guardar_items(State_Map* nodo, char** campos){
  List* items = split_string(campos[3], ";");
  nodo->items = list_create();
  char* item;
  for(CADA_RECORRIDO(item, items)) {
      List* values = split_string(item, ",");

      Item* nuevo_item = (Item*) malloc(sizeof(Item));
      nuevo_item->nombre = list_first(values);
      nuevo_item->valor = atoi(list_next(values));
      nuevo_item->peso = atoi(list_next(values));
      list_pushBack(nodo->items, nuevo_item);

      list_clean(values);
      free(values);
  }

  list_clean(items);
  free(items); 
}

void guardar_direcciones(State_Map* nodo, char** campos) {
  // Arriba campos[4] ; Abajo campos[5] ; Izquierda campos[6] ; Derecha campos[7]
  for (int i = 0; i < 4; i++) {
    int direccion = atoi(campos[i + 4]);
    nodo->direcciones[i] = direccion;
  }
}

State_Map* crear_estado(char** campos) {
  State_Map* nuevo_nodo = (State_Map*) malloc(sizeof(State_Map));
  if (nuevo_nodo == NULL) { perror("No se pudo crear el nodo"); return NULL; }
  
  nuevo_nodo->ID = atoi(campos[0]);
  nuevo_nodo->nombre = strdup(campos[1]);
  nuevo_nodo->descripcion = strdup(campos[2]);
  guardar_items(nuevo_nodo, campos);
  guardar_direcciones(nuevo_nodo, campos);
  nuevo_nodo->final = *campos[8];
  nuevo_nodo->adj_nodes = list_create();

  return nuevo_nodo;
}

void crear_conexiones(Map* mapa_juego) {
  for (int k = 0; k < 15 ; k++) {
    MapPair* pair = map_search(mapa_juego, IDs[k]);
    State_Map* sala = pair->value;
    
    for (int i = 0; i < 4; i++) {
      int key = sala->direcciones[i];
      if (key < 0) continue;
      
      MapPair* sala_nodo = map_search(mapa_juego, IDs[key-1]);
      State_Map* sala_adyacente = sala_nodo->value;
      list_pushBack(sala->adj_nodes, sala_adyacente);
    }
  }
}

void leer_mapa_completo(Map* mapa_juego) {
  limpiar_pantalla();
  if (mapa_cargado) { puts("El laberinto ya fue cargado!"); return; }
  
  FILE *archivo = fopen("data/graphquest.csv", "r");
  if (archivo == NULL) { perror("Error al abrir el archivo"); return; }

  char** LEER_CAMPO;
  int posicionID = 0;
  for(CADA_CAMPO, posicionID++) {
    State_Map* nuevo_nodo = crear_estado(campos);
    if (nuevo_nodo == NULL) continue;
    IDs[posicionID] = &nuevo_nodo->ID;
    map_insert(mapa_juego, IDs[posicionID], nuevo_nodo);
  }

  crear_conexiones(mapa_juego);
  mapa_cargado = 1;
  fclose(archivo);
  return;
}

void mostrar_menu_principal() {
    limpiar_pantalla();
    imprimir_separador("GRAPHQUEST", 30);
    puts("(1). Cargar Laberinto");
    printf("\033[1;34m");
    puts("(2). Iniciar Partida");
    printf("\033[0m");
    puts("(0). Salir") ;
}

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
    printf("   -> \033[1;37m%s\033[0m (ID: %d)\n", sala->nombre, sala->ID);
  }
}

void mostrar_datos(const State_Map* estado) {
  printf("\033[1;31mID:\033[0m %d\n", estado->ID);
  printf("\033[1;34mNOMBRE:\033[0m %s\n", estado->nombre);
  printf("\033[1;32mDESCRIPCION:\033[0m %s\n", estado->descripcion);
  mostrar_items(estado->items);
  mostrar_conexiones(estado->adj_nodes);
  if (estado->final == 'S') puts("Es Final!\n");
  putchar('\n');
}