#include "extras.h"

#define MAX_LINE_LENGTH 1024
#define MAX_FIELDS 300

char **leer_linea_csv(FILE *archivo, char separador) {
    static char linea[MAX_LINE_LENGTH] ;
    static char *campos[MAX_FIELDS] ;
    char *ptr, *start ;
    int idx = 0 ;

    if (fgets(linea, MAX_LINE_LENGTH, archivo) == NULL) {
        return NULL; // No hay más líneas para leer
    }

    // Eliminar salto de linea
    linea[strcspn(linea, "\n")] = '\0' ;

    ptr = start = linea ;
    while (*ptr) {
        if (idx >= MAX_FIELDS - 1) break ;

        if (*ptr == '\"') { // Inicio de un campo entrecomillado
            start = ++ptr ;    // Saltar la comilla inicial
            while (*ptr && !(*ptr == '\"' && *(ptr + 1) == separador)) ptr++ ;
        } else { // Campo sin comillas
            start = ptr ;
            while (*ptr && *ptr != separador) ptr++ ;
        }

        if (*ptr) { 
            *ptr = '\0' ; // Reemplazar comilla final o separador por terminación
            ptr++ ;
            if (*ptr == separador) ptr++ ;
        }

        // Quitar comilla final si existe
        if (*(ptr - 2) == '\"') *(ptr - 2) = '\0' ;
        campos[idx++] = start ;
    }

    campos[idx] = NULL ; // Marcar el final del array
    return campos ;
}

int is_equal_str(void *key1, void *key2) {
  return strcmp((char *)key1, (char *)key2) == 0 ;
}

List *split_string(const char *str, const char *delim) {
  List *result = list_create() ;
  char *token = strtok((char *)str, delim) ;

  while (token != NULL) {
    // Eliminar espacios en blanco al inicio del token
    while (*token == ' ') {
      token++ ;
    }
    
    // Eliminar espacios en blanco al final del token
    char *end = token + strlen(token) - 1 ;
    while (*end == ' ' && end > token) {
      *end = '\0' ;
      end-- ;
    }

    // Copiar el token en un nuevo string
    char *new_token = strdup(token) ;

    // Agregar el nuevo string a la lista
    list_pushBack(result, new_token) ;

    // Obtener el siguiente token
    token = strtok(NULL, delim) ;
  }

  return result ;
}

//---//

void limpiar_pantalla() {
  // Verifica si es que el OS es Windows o "UNIX" (Linux/MacOS)
  #ifdef _WIN32
    system("cls") ;
  #else
    printf("\033[H\033[J"); 
  #endif
}

void imprimir_separador(const char* mensaje) {
  int largo = strlen(mensaje) + 12 ;
  int i ;
  putchar('#') ;
  for (i = 0 ; i < largo ; i++) {
      putchar('=') ;
  }
  puts("#") ;
  printf("       %s\n", mensaje) ;
  putchar('#') ;
  for (i = 0 ; i < largo ; i++) {
      putchar('=') ;
  }
  putchar('#') ;
  putchar('\n') ;
}

void mostrar_menu() {
    limpiar_pantalla() ;
    imprimir_separador("SPOTIFIND") ;
    puts("(1). Cargar Canciones") ;
    puts("(2). Buscar por Género") ;
    puts("(3). Buscar por Artista") ;
    puts("(4). Buscar por Tempo") ;
    puts("(0). Salir") ;
}

void esperar_enter() {
    puts("Presione ENTER para continuar...") ;
    while (getchar() != '\n') ;
}

void leer_entrada(char* string) {
    char buffer[200] ;
    fgets(buffer, sizeof(buffer), stdin) ;
    buffer[strcspn(buffer, "\n")] = '\0' ;
    strcpy(string, buffer) ;
}

void leer_opcion(char* opcion) { 
  // Asigna el puntero (primer valor) al valor final de una cadena (\0).
  // En caso de que el usuario no ingrese nada.
  *opcion = '\0' ;
  printf("Ingrese su opción: ") ;
  char buffer[200] ;
  leer_entrada(buffer) ;
  sscanf(buffer, "%c", opcion) ; 
}

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

    
    char is_final = atoi(campos[8]);
    if (is_final) printf("Es final\n");

    list_clean(items);
    free(items);
    
  }
  fclose(archivo); // Cierra el archivo después de leer todas las líneas
  return;
}