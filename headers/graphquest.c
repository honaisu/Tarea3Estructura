#include "graphquest.h"

#define LEER_CAMPO campos = leer_linea_csv(archivo, ',')
#define CADA_CAMPO LEER_CAMPO; campos != NULL; LEER_CAMPO
#define CADA_ITEM  char* item = list_first(items); item != NULL; item = list_next(items)

void guardar_items(State_Map* nodo, char** campos){
  List* items = split_string(campos[3], ";");
  nodo->items = list_create();
  for(CADA_ITEM){
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

  return nuevo_nodo;
}

void mostrar_items(const List* items) {
  
}

void mostrar_datos(const State_Map* estado) {
  printf("ID: %d\n", estado->ID);
  printf("NOMBRE: %s\n", estado->nombre);
  printf("DESCRIPCION: %s\n", estado->descripcion);
  mostrar_items(estado->items);
  printf("ES FINAL?: %c\n", estado->final);
}

void leer_mapa_completo(Map* mapa_juego) {
  FILE *archivo = fopen("data/graphquest.csv", "r");
  if (archivo == NULL) { perror("Error al abrir el archivo"); return; }
  
  char** LEER_CAMPO;
  for(CADA_CAMPO) {
    State_Map* nuevo_nodo = crear_estado(campos);
    if (nuevo_nodo == NULL) continue;
    int* key = (int*) malloc(sizeof(int)); 
    *key = nuevo_nodo->ID;
    map_insert(mapa_juego, key, nuevo_nodo);
  }

  MapPair* first = map_first(mapa_juego);
  while (first != NULL) {
    printf("Llave: %p\nCada dato\n", first->key);
    mostrar_datos(first->value);
    first = map_next(mapa_juego);
    putchar('\n');
  }

  fclose(archivo);
  return;
}

void mostrar_menu_principal() {
    limpiar_pantalla() ;
    imprimir_separador("GRAPHQUEST", 30) ;
    puts("(1). Cargar Laberinto") ;
    puts("(2). \033[1;34mIniciar Partida\033[0m") ;
    puts("(0). Salir") ;
}
