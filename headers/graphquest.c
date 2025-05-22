#include "graphquest.h"

void leer_escenarios(Map* mapa_juego) {
  FILE *archivo = fopen("data/graphquest.csv", "r");
  if (archivo == NULL) { perror("Error al abrir el archivo"); return; }
  
  char** campos = leer_linea_csv(archivo, ',');

  while ((campos = leer_linea_csv(archivo, ',')) != NULL) {
    State_Map* nuevo_nodo = (State_Map*) malloc(sizeof(State_Map));
    if (nuevo_nodo == NULL) { perror("No se pudo crear el nodo"); break; }
    // ID atoi(campos[0])
    nuevo_nodo->ID = atoi(campos[0]);

    int* key = (int*) malloc(sizeof(int)); 
    *key = nuevo_nodo->ID;

    map_insert(mapa_juego, key, nuevo_nodo);
    // Nombre campos[1]
    nuevo_nodo->nombre = campos[1];
    // Descripción campos[2]
    nuevo_nodo->descripcion = campos[2];
    // Items campos[3]
    List* items = split_string(campos[3], ";");
    nuevo_nodo->items = list_create();
    for(char* item = list_first(items); item != NULL; item = list_next(items)){
        List* values = split_string(item, ",");
        Item* nuevo_item = (Item*) malloc(sizeof(Item));

        nuevo_item->nombre = list_first(values);
        nuevo_item->valor = atoi(list_next(values));
        nuevo_item->peso = atoi(list_next(values));
    
        printf("  - %s (%d pts, %d kg)\n", nuevo_item->nombre, nuevo_item->valor, nuevo_item->peso);

        list_pushBack(nuevo_nodo->items, nuevo_item);
        list_clean(values);
        free(values);
    }

    // Arriba campos[4] ; Abajo campos[5] ; Izquierda campos[6] ; Derecha campos[7]
    for (int i = 4; i < 8; i++) {
      int direccion = atoi(campos[i]);
      if (direccion < 0) continue;
      printf("%d ", direccion);
      int* adjacente = (int*) malloc(sizeof(int));
      *adjacente = direccion;
      list_pushBack(nuevo_nodo->adj_nodes, adjacente);
    }

    nuevo_nodo->final = *campos[8];
    if (nuevo_nodo->final == 'N') printf("No es final\n");

    list_clean(items);
    free(items); 
  }
  fclose(archivo); // Cierra el archivo después de leer todas las líneas
  return;
}

void mostrar_menu_principal() {
    limpiar_pantalla() ;
    imprimir_separador("GRAPHQUEST", 30) ;
    puts("(1). Cargar Laberinto") ;
    puts("(2). \033[1;34mIniciar Partida\033[0m") ;
    puts("(0). Salir") ;
}
