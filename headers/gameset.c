#include "gameset.h"
#include "mostrar.h"

// MACRO correspondiente al valor del tiempo del jugador.
#define _TIEMPO 30
// MACRO encargado de la fórmula del tiempo [(Peso Total + 1) / 10].
// Optimizado para no usar división (equivalencia de 1/10 con 0.1)
#define FORMULA_TIEMPO (jugador->peso_total + 1) * 0.1
// MACRO definido para poder mostrar un mensaje personalizado en las opciones con Objetos.
// Necesita de una entrada (puntero a char para leerse), y un mensaje que cambiará la última parte de un mensaje general. 
#define ENTRADA(entrada, mensaje) printf("Ingrese el nombre del Objeto %s: ", mensaje); leer_entrada(entrada);

// Variable global que se encarga de poder ver cuando se llega al final o no.
unsigned short es_final = 0;

//--- Funciones Principales del Jugador ---//

// Permite que el jugador pueda recoger un Objeto en la sala en la que se encuentre.
// Si no nay ningún Objeto para recoger, no hace nada.
void jugador_recoger(Player*);
// Permite que el jugador descarte un Objeto de su inventario (mochila).
// Si no hay ningún Objeto a descartar, no hace nada.
void jugador_descartar(Player*);
// Hace que el jugador pueda avanzar por las distintas salas (nodos) del laberinto.
// Necesita que el jugador escriba la dirección de la sala correspondiente para avanzar.
// Si el jugador no hace nada, se mantiene en la misma habitación.
// Cada vez que se avanza a una habitación, se restará tiempo del jugador en base a la fórmula del tiempo (MACRO).
void jugador_avanzar(Player*);
// Permite poder volver a crear una partida en medio de la partida.
// Cada vez que se resetea la partida, muestra un mensaje correspondiente a como "volver" al juego.
// Si el jugador decide no resetear la partida, continua su partida correspondiente.
void resetear_partida(Player*, Map*);
// Opción que permite salir de la partida para volver a la pantalla de inicio.
// Da la posibilidad de no salir de la partida (en caso de haber sido presionado sin querer).
// Si se sale de la partida, se borran los datos del jugador y el mapa vuelve a su estado "default".
void salir_partida(char*, Player*, Map*);

//--- Funciones Varias del Jugador ---//

// Inicializa las variables del jugador.
// Pone los parámetros que existan dentro de la estructura del jugador en valores ceros o nulos.
Player* inicializar_jugador(void);
// Función que vuelve un jugador a su estado predeterminado ("default").
// Devuelve cada parámetro del jugador a valores cero o nulo y elimina los Objetos que haya acumulado.
// Si resulta que es primer vez que se crea al jugador (la lista de Objetos igual a nula), inicializa la lista correspondiente para ser usada.
void jugador_default(Player*);
// Busca un Objeto especifico por nombre (const char*) dentro de una lista de Objetos.
// Si el Objeto existe dentro de la lista, retorna ese Objeto.
// Si no, retorna nulo.
Item* item_buscar(List*, const char*);
// Actualiza (agrega o remueve) los Objetos que existan dentro del jugador.
// El primer parámetro es el jugador, necesario para poder actualizar su lista de Objetos.
// El segundo es la lista de Objetos a usar para agregar (o remover) un Objeto específico.
// El tercer parámetro es el Objeto a buscar. Si no existe el Objeto, no hace nada; si existe, lo agrega o remueve del jugador y cambia su peso y puntaje total.
// El cuarto parámetro es una constante que verifica si es que se necesita agregar o remover el Objeto al inventario del jugador.
void jugador_actualizar_items(Player*, List*, Item*, const int);

// Resetea el mapa para poder volver a ser usado en medio de la partida (casos de reinicio o salida).
void resetear_mapa(Map*);

// --- //

void jugador_recoger(Player* jugador) {
    State_Map* sala = jugador->sala_actual;
    // --- Impresion --- //
    if (list_first(sala->items) == NULL) { puts("*Intentas ver si hay Objetos útiles en la habitación, pero no hay nada*"); return; }
    mostrar_items(sala->items);

    char entrada[200];
    ENTRADA(entrada, "a recoger");
    // --- //
    
    Item* items_sala = item_buscar(sala->items, entrada);
    if (items_sala == NULL) { puts("*Intentaste materializar un Objeto inexistente en la habitación*\n*No tuvo efecto*"); return; }
        
    jugador_actualizar_items(jugador, sala->items, items_sala, 1);
    return;
}

void jugador_descartar(Player* jugador) {
    // --- Impresion --- //
    if (list_first(jugador->items) == NULL) { puts("*Intentas abrir tu mochila para descartar Objetos, pero no tienes ninguno*"); return; }
    mostrar_items(jugador->items);

    char entrada[200];
    ENTRADA(entrada, "a descartar");
    // --- //

    Item* items_jugador = item_buscar(jugador->items, entrada);
    if (items_jugador == NULL) { puts("*Intentaste sacar un Objeto que no existia en tu mochila*\n*-1 de estilo*"); return; }
        
    jugador_actualizar_items(jugador, jugador->items, items_jugador, -1);
    return;
}

void jugador_avanzar(Player* jugador) {
    // --- Impresion --- //
    limpiar_pantalla();
    imprimir_separador("Decides ver las habitaciones adjuntas a la actual", 50);
    // --- //

    const char* direcciones[4] = {"Arriba", "Abajo", "Izquierda", "Derecha"};
    State_Map* sala = jugador->sala_actual;
    State_Map* adyacente = NULL;
    int direccion_deseada = -1;

    // Mostrará las direcciones posibles
    for (int i = 0, limite = 0; i < 4; i++) {
        if (sala->direcciones[i] == -1) continue; // NULA
        
        adyacente = list_first(sala->adj_nodes);
        // Irá recorriendo los nodos adyacentes
        // Limite es una variable que hará que avance a la posición deseada (la dirección correcta)
        // EJ: lim = 2 ... sala 1 -> sala 2 -> *sala 3* (IZQUIERDA)
        for (int k = 0; k < limite; k++) adyacente = list_next(sala->adj_nodes);
        // Imprime la dirección y el nombre de la sala adyacente.
        printf("   -> %s: \033[1;37m%s\033[0m\n", direcciones[i], adyacente->nombre);
        ++(limite);
    }
    // --- //

    char entrada[200];
    puts("¿A qué dirección decides moverte? (Ninguna: \"0\")");
    leer_entrada(entrada);

    if (!strcmp(entrada, "0")) { puts("*Decides quedarte en la misma habitación*"); return; }

    // Buscar dirección ingresada
    // Buscará la dirección de la sala a buscar
    for (int i = 0; i < 4; i++) {
        if (sala->direcciones[i] == -1) continue; // NULA (OMITIR)
        if (strcmp(entrada, direcciones[i]) != 0) continue; // No es el mismo nombre (OMITIR)
        
        direccion_deseada = i;
        break;
    }
    // --- //

    if (direccion_deseada == -1) { puts("*Intentaste moverte a una dirección inexistente*\n*No surge efecto*"); return; }

    // Encontrar la sala del jugador
    adyacente = list_first(sala->adj_nodes);
    for (int i = 0; i < 4; i++) {
        if (sala->direcciones[i] == -1) continue; // NULA (OMITIR)
        
        if (i == direccion_deseada) break; // Si llega a la dirección deseada, termina el ciclo
        adyacente = list_next(sala->adj_nodes); // Sigue recorriendo la lista de nodos adyacentes
    }
    jugador->sala_actual = adyacente; // El nodo de la lista recorrida con la dirección correcta será la sala actual.

    // --- Impresion --- //
    limpiar_pantalla();
    if (jugador->sala_actual->final == 'S') { // En caso de que la habitación sea final.
        mostrar_mensaje_final(jugador);
        es_final = 1;
    } else {
        printf("Decides moverte a esta habitación: \033[1;37m%s\033[0m\n", jugador->sala_actual->nombre);
        // Se resta el tiempo
        jugador->tiempo -= FORMULA_TIEMPO;
        esperar_enter();
        mostrar_estado_actual(jugador, jugador->sala_actual);
    }
    // --- //
}


void resetear_partida(Player* jugador, Map* mapa_juego) {
    // --- Impresion --- //
    limpiar_pantalla();
    imprimir_separador("¿Quieres resetear la partida actual (PERDERÁS TODO EL PROGRESO)? [S/N]", 75);
    // --- //

    char o;
    leer_opcion(&o);
    // --- Impresion --- //
    if (o == 'N') {
        puts("Decidiste quedarte en la misma partida.");
        return;
    } else if (o != 'S') {
        puts("*Te das cuenta que decidir fue muy abrumador*");
        puts("*No haces nada*");
        return;
    }
    // --- //
    
    // SI se decide resetear, devuelve los valores predeterminados el jugador y mapa
    jugador_default(jugador);
    resetear_mapa(mapa_juego);
    MapPair* pair = map_first(mapa_juego);
    jugador->sala_actual = pair->value; // Del mapa, consigue la primer sala para que sea la actual del jugador
    mostrar_reseteo(); // Mensaje reseteo
}

void salir_partida(char* o_original, Player* jugador, Map* mapa_juego) {
    // --- Impresion --- //
    limpiar_pantalla();
    imprimir_separador("¿Estás seguro de querer salir de la partida? [S/N]", 60);
    
    char o;
    leer_opcion(&o);
    if (o == 'S') {
        *o_original = '0'; // Al terminar la función, acabará la partida
        jugador_default(jugador);
        free(jugador);
        imprimir_separador("FIN DE LA PARTIDA", 60);
        return;
    } 

    // --- Impresion --- //
    if (o != 'N') {
        puts("*Intentaste pensar si salir valdría la pena o no*");
        puts("*No haces nada*");
    } else puts("No decidiste sucumbir a la tentación.");

    *o_original = 'd'; // Evita que el jugador SALGA sin querer
}

void casos_opciones(char* o, Player* jugador, Map* mapa_juego) {
    switch (*o) {
        // Recoger Item(s)
        case '1': { jugador_recoger(jugador); break; }
        // Descartar Item(s)
        case '2': { jugador_descartar(jugador); break; }
        // Avanzar a una habitación
        case '3': { jugador_avanzar(jugador); break; }
        // Ver estado de la habitación
        case '4': { mostrar_estado_actual(NULL, jugador->sala_actual); break; }
        // Ver estado del jugador
        case '5': { mostrar_estado_actual(jugador, NULL); break; }

        case '-': { resetear_partida(jugador, mapa_juego); break; }
        case '0': { salir_partida(o, jugador, mapa_juego); break; }
        // Respuesta predeterminada
        default: { puts("*Intentaste usar una opción no disponible*\n*No surgió efecto*"); }
    }
    if (*o != '0' && !es_final) esperar_enter();
}

void pantalla_jugador(Player* jugador, Map* mapa_juego) {
    MapPair* pair = map_first(mapa_juego);
    jugador->sala_actual = pair->value;

    char o = '\0';
    do {
        mostrar_menu_jugador();
        leer_opcion(&o);
        casos_opciones(&o, jugador, mapa_juego);
    } while (o != '0' && jugador->tiempo > 0 && !es_final);

    if (jugador->tiempo <= 0) mostrar_mensaje_derrota();
    return;
}

void jugar_juego(Map* mapa_juego) {
    if (!mapa_cargado) { puts("NO SE HA CARGADO EL LABERINTO"); return; }
    resetear_mapa(mapa_juego);
    Player* jugador = inicializar_jugador();
    es_final = 0;
    limpiar_pantalla();
    mostrar_historia();
    pantalla_jugador(jugador, mapa_juego);
    return;
}

// --- //

void resetear_mapa(Map* mapa_juego) {
    map_clean(mapa_juego);
    mapa_cargado = 0;
    // Lee el mapa con la condición de modo reset
    leer_mapa_completo(mapa_juego, 'S');
}

Item* item_buscar(List* items, const char* nombre) {
    Item* item;
    for (CADA_RECORRIDO(item, items)) {
        if (!strcmp(item->nombre, nombre)) return item;
    }
    return NULL;
}

void jugador_actualizar_items(Player* jugador, List* lista, Item* item, const int signo) {
    if (signo > 0) list_pushBack(jugador->items, item);
    list_popCurrent(lista);

    jugador->peso_total += signo * item->peso;
    jugador->puntaje += signo * item->valor;
    --(jugador->tiempo);
}

void jugador_default(Player* jugador){
    jugador->tiempo = _TIEMPO;
    jugador->sala_actual = NULL;
    jugador->puntaje = 0;
    jugador->peso_total = 0;
    (jugador->items == NULL) ? jugador->items = list_create() : list_clean(jugador->items);
}

Player* inicializar_jugador(void) {
    Player* nuevo_jugador = (Player*) malloc(sizeof(Player));
    nuevo_jugador->items = NULL;
    jugador_default(nuevo_jugador);
    return nuevo_jugador;
}