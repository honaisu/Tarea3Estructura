#include "gameset.h"
#include "mostrar.h"

// MACRO que sólo se encarga de cambiar el color de una palabra para mostrar si está disponible o no.
#define COLOR_ESTA_DISPONIBLE(variable) (variable) ? printf("\033[1;31m") : printf("\033[0m")

/* Colores de BASH
\033[0m : Color Predeterminado.
\033[Xm : Color X.
\033[1;Xm : Negrita con X siendo algún color.
*/

void mostrar_items(List* items) {
  if (list_first(items) == NULL) return;
  puts("\033[1;35mOBJETOS:\033[0m");
  Item* item;
  for(CADA_RECORRIDO(item, items)) {
    printf("   -> %s (%d pts, %d kg)\n", item->nombre, item->valor, item->peso);
  }
}

void mostrar_conexiones(List* adyacentes) {
  if (list_first(adyacentes) == NULL) return;
  State_Map* sala;
  puts("\033[1;33mHABITACIONES ADJUNTAS:\033[0m");
  for(CADA_RECORRIDO(sala, adyacentes)) {
    printf("   -> \033[1;37m%s\033[0m\n", sala->nombre);
  }
}

void mostrar_datos(const State_Map* estado) {
  printf("\033[1;34mNOMBRE:\033[0m %s\n", estado->nombre);
  printf("\033[1;32mDESCRIPCION:\033[0m %s\n", estado->descripcion);
  mostrar_items(estado->items);
  mostrar_conexiones(estado->adj_nodes);
  putchar('\n');
}

void mostrar_historia(void) {
    imprimir_separador("GraphQuest: Journey of a Deadman's Tale", 50);
    puts("Eres un viajero sin nombre, conocido solo por tu astucia y ambición.");
    puts("Las leyendas hablan de un antiguo laberinto, repleto de tesoros olvidados por viajeros que sucumbieron.");
    puts("Tu objetivo es simple: adentrarte en sus pasillos, recolectar los objetos más valiosos y salir con vida.");
    puts("No buscas gloria, solo riquezas que venderás en los rincones más oscuros del mercado negro.");
    puts("¿Serás capaz de burlar los peligros y convertirte en leyenda entre los mercaderes clandestinos?");
    esperar_enter();
}

void mostrar_menu_principal(void) {
    limpiar_pantalla();
    imprimir_separador("GRAPHQUEST", 40);
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
    imprimir_separador("Te encuentras en la siguiente habitación", 70);
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
  imprimir_separador("Un nuevo comienzo...", 60);
  puts("El viajero sin nombre siente cómo su historia se desvanece en la penumbra.");
  puts("Pero el laberinto no olvida, y una nueva oportunidad surge entre sus muros.");
  puts("Renaces, sin recuerdos ni cargas, listo para desafiar una vez más los secretos y peligros del laberinto.");
  puts("El ciclo comienza de nuevo. ¿Lograrás esta vez salir con vida y riquezas?");
}

void mostrar_mensaje_final(Player* jugador) {
  imprimir_separador("Llegaste al final del laberinto!", 50);
  mostrar_datos(jugador->sala_actual);
  if (!jugador->puntaje) {
    puts("Cruzaste el laberinto, pero tus manos están vacías.");
    puts("La avaricia fue reemplazada por la prudencia, pero sin tesoros, tu viaje no será recordado.");
    puts("Quizás la próxima vez, el riesgo valga la recompensa...");
  } else {
    puts("Has logrado salir del laberinto con valiosos tesoros!");
    puts("Cada objeto recolectado es testimonio de tu astucia y coraje.");
    puts("En los rincones del mercado negro, tu nombre resonará como leyenda entre los mercaderes clandestinos.");
  }
  char s = (jugador->puntaje == 1) ? '\0' : 's';
  imprimir_separador("Terminaste con las siguientes estadísticas:", 50);
  mostrar_items(jugador->items);
  printf("Tiempo Restante: %d min\n", jugador->tiempo);
  printf("\033[1;37mPuntaje final: %d pt%c\033[0m\n", jugador->puntaje, s);
}

void mostrar_mensaje_derrota(void) {
  limpiar_pantalla();
  imprimir_separador("El sesgo avaricio, en las manos del tiempo...", 50);
  puts("El viajero sin nombre, cegado por la avaricia, sintió cómo el peso de su mochila lo arrastraba hacia la oscuridad.");
  puts("Cada tesoro recolectado se convirtió en una carga imposible de soportar.");
  puts("Sin fuerzas para avanzar, te pierdes en las penumbras eternas del laberinto, donde el eco de tus pasos se apaga para siempre.");
  imprimir_separador("GAME OVER", 50);
}