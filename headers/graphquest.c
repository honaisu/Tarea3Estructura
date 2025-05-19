#include "graphquest.h"

typedef struct {
    int ID;
    char* nombre;
    char* descripcion;
    List* items;
    List* adj_nodes;
} State_Map;

void leer_escenarios() {
  // Intenta abrir el archivo CSV que contiene datos de películas
  FILE *archivo = fopen("data/graphquest.csv", "r");
  if (archivo == NULL) { perror("Error al abrir el archivo"); return; }
  
  char** campos;
  // Leer y parsear una línea del archivo CSV. La función devuelve un array de
  // strings, donde cada elemento representa un campo de la línea CSV procesada.
  campos = leer_linea_csv(archivo, ','); // Lee los encabezados del CSV

  // Lee cada línea del archivo CSV hasta el final
  while ((campos = leer_linea_csv(archivo, ',')) != NULL) {
    // ID
    printf("\033[36mID:\033[0m %d\n", atoi(campos[0]));
    // Nombre
    printf("\033[33mNombre:\033[0m %s\n", campos[1]);
    // Descripción
    printf("\033[32mDescripción:\033[0m %s\n", campos[2]);

    List* items = split_string(campos[3], ";");

    // Items
    char* item = list_first(items);
    (item) ? printf("\033[31mItems:\033[0m \n") : 0 ;
    while (item != NULL) {
        List* values    = split_string(item, ",");
        char* item_name = list_first(values);
        int item_value  = atoi(list_next(values));
        int item_weight = atoi(list_next(values));
        printf(" - %s (%d pts, %d kg)\n", item_name, item_value, item_weight);
        list_clean(values);
        free(values);
        item = list_next(items);
    }

    int arriba = atoi(campos[4]);
    int abajo = atoi(campos[5]);
    int izquierda = atoi(campos[6]);
    int derecha = atoi(campos[7]);

    if (arriba != -1) printf("[Arriba: %d]\n", arriba);
    if (abajo != -1) printf("[Abajo: %d]\n", abajo);
    if (izquierda != -1) printf("[Izquierda: %d]\n", izquierda);
    if (derecha != -1) printf("[Derecha: %d]\n", derecha);


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
    puts("(2). \033[34mIniciar Partida\033[0m") ;
    puts("(0). Salir") ;
}