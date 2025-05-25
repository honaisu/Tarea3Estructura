#include "graphquest.h"
#include "mostrar.h"

// MACRO que se encarga de crear una variable que lea los campos.
// Usada para poder leer cada línea del archivo CSV del archivo correspondiente.
#define LEER_CAMPO(campo) campo = leer_linea_csv(archivo, ',')
// MACRO usado para poder recorrer cada línea del archivo CSV que se presente.
#define CADA_CAMPO(campo) LEER_CAMPO(campo); campo != NULL; LEER_CAMPO(campo)

// Variable global que se encarga de guardar las IDs que se usan para la lectura.
// Como sólo necesito la dirección de memoria del ID (la uso como clave), es una lista de punteros a void.
void* IDs[16];

// Variable global que se encarga de ver si es que el mapa se ha cargado o no. Cargado (1) y no cargado (0).
unsigned short mapa_cargado = 0; 

// Guarda los Items que haya en un campo dentro de un nodo estado (sala).
// Dentro del nodo, todos los Items del campo los guardará en forma de Lista para poder accederlos.
void guardar_items(State_Map*, char**);
// Guarda las direcciones de las salas adjuntas a un nodo estado (sala).
void guardar_direcciones(State_Map*, char**);
// Crea las conexiones de cada uno de los nodos estado (salas).
void crear_conexiones(Map*);
// Crea el estado de una sala a partir de los parámetros del campo (línea) en el archivo.
State_Map* crear_estado(char**);

// --- //

void guardar_items(State_Map* nodo, char** campos){
  List* items = split_string(campos[3], ";"); // Separa los datos del campo otorgado (Items) por un separador ";"
  nodo->items = list_create(); // Crea la lista de Items del nodo (sala)
  char* item;
  for(CADA_RECORRIDO(item, items)) {
      List* values = split_string(item, ","); // Separa los datos de un item por un separador ",".

      Item* nuevo_item = (Item*) malloc(sizeof(Item));
      nuevo_item->nombre = list_first(values); // Primer dato de la "lista" (Nombre)
      nuevo_item->valor = atoi(list_next(values)); // Segundo dato (String del Valor [pts] a Entero)
      nuevo_item->peso = atoi(list_next(values)); // Tercer dato (String del Peso [kg] a Entero)
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
    nodo->direcciones[i] = direccion; // Si resulta que no hay dirección, el valor será -1.
  }
}

State_Map* crear_estado(char** campos) {
  State_Map* nuevo_nodo = (State_Map*) malloc(sizeof(State_Map));
  if (nuevo_nodo == NULL) { perror("No se pudo crear el nodo"); return NULL; }
  
  nuevo_nodo->ID = atoi(campos[0]);
  nuevo_nodo->nombre = strdup(campos[1]); // Duplica el contenido del String de Campos para guardarlo en el nodo (sala)
  nuevo_nodo->descripcion = strdup(campos[2]);
  guardar_items(nuevo_nodo, campos);
  guardar_direcciones(nuevo_nodo, campos);
  nuevo_nodo->final = *campos[8]; // Toma el primer carácter del String de "Es Final" ("Si"/"No"). Guarda el primer carácter dentro del nodo (sala).
  nuevo_nodo->adj_nodes = list_create(); // Se inicializa la lista de nodos adyacentes.

  return nuevo_nodo;
}

void crear_conexiones(Map* mapa_juego) {
  // Se recorre el mapa (16 - 1) veces (la última sala no tiene ninguna conexión) para ir creando las conexiones de las salas.
  for (int k = 0; k < 15 ; k++) {
    MapPair* pair = map_search(mapa_juego, IDs[k]); // Busca dentro del mapa la sala correspondiente con mi lista de IDs.
    State_Map* sala = pair->value;
    
    for (int i = 0; i < 4; i++) {
      int key = sala->direcciones[i];
      if (key < 0) continue; // Si la dirección no es válida, continuo
      
      MapPair* sala_nodo = map_search(mapa_juego, IDs[key-1]); // Busca en el mapa del juego una ID que corresponda a key (-1, para no acceder a posiciones inexistentes).
      State_Map* sala_adyacente = sala_nodo->value; // El nodo (sala) encontrado será la sala adyacente para agregar a la lista.
      list_pushBack(sala->adj_nodes, sala_adyacente);
    }
  }
}

void leer_mapa_completo(Map* mapa_juego, const char es_reset) {
  limpiar_pantalla();
  if (mapa_cargado) { imprimir_separador("El laberinto ya fue cargado!", 40); imprimir_gato(); return; }
  
  FILE *archivo = fopen("data/graphquest.csv", "r");
  if (archivo == NULL) { perror("Error al abrir el archivo"); return; }

  imprimir_separador("Leyendo el laberinto...", 40);
  char** LEER_CAMPO(campos);
  int posicionID = 0; // Avanza para ir colocando la ID (dirección) como clave en el mapa.
  for(CADA_CAMPO(campos), posicionID++) {
    State_Map* nuevo_nodo = crear_estado(campos);
    if (nuevo_nodo == NULL) continue;
    IDs[posicionID] = &nuevo_nodo->ID;
    map_insert(mapa_juego, IDs[posicionID], nuevo_nodo);
  }
  crear_conexiones(mapa_juego); // Crea la lista de adyacente de todos los nodos estado
  
  limpiar_pantalla();
  mapa_cargado = 1; // Verificador de que se cargó el mapa

  // Si la función no está en modo reset, imprime el mensaje del laberinto cargado.
  if (es_reset == 'N') imprimir_separador("El laberinto ha sido cargado!", 40);
  fclose(archivo);
  return;
}