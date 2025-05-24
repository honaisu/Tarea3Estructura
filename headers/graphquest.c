#include "graphquest.h"
#include "mostrar.h"

// MACRO que se encarga de crear una variable que lea los campos.
// Usada para poder leer cada línea del archivo CSV del archivo correspondiente.
#define LEER_CAMPO(campo) campo = leer_linea_csv(archivo, ',')
// MACRO usado para poder recorrer cada línea del archivo CSV que se presente.
#define CADA_CAMPO(campo) LEER_CAMPO(campo); campo != NULL; LEER_CAMPO(campo)

// Variable global que se encarga de guardar las IDs que se usan para la lectura
void* IDs[16];
// Variable global que se encarga de vef si es que el mapa se ha cargado o no. Cargado (1) y no cargado (0).
unsigned short mapa_cargado = 0; 

void guardar_items(State_Map*, char**);
void guardar_direcciones(State_Map*, char**);
State_Map* crear_estado(char**);
void crear_conexiones(Map*);

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
  if (mapa_cargado) { imprimir_separador("El laberinto ya fue cargado!", 40); imprimir_gato(); return; }
  
  FILE *archivo = fopen("data/graphquest.csv", "r");
  if (archivo == NULL) { perror("Error al abrir el archivo"); return; }

  imprimir_separador("Leyendo el laberinto...", 40);
  char** LEER_CAMPO(campos);
  int posicionID = 0;
  for(CADA_CAMPO(campos), posicionID++) {
    State_Map* nuevo_nodo = crear_estado(campos);
    if (nuevo_nodo == NULL) continue;
    IDs[posicionID] = &nuevo_nodo->ID;
    map_insert(mapa_juego, IDs[posicionID], nuevo_nodo);
  }
  crear_conexiones(mapa_juego);
  
  limpiar_pantalla();
  imprimir_separador("El laberinto ha sido cargado!", 40);
  mapa_cargado = 1;
  fclose(archivo);
  return;
}