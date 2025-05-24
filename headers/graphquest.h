#ifndef GRAPHQUEST_H
#define GRAPHQUEST_H
#include "extras.h"

// MACRO encargado de colocar valores en iteraciones 'for' para el recorrido de listas.
// Necesita de dos argumentos: Valor, puntero encargado de recorrer el elemento guardado en la lista;
// Lista, que recibe una lista que quiere ser recorrida hasta acabar sus elementos.
#define CADA_RECORRIDO(valor, lista) valor = list_first(lista); valor != NULL; valor = list_next(lista)

extern unsigned short mapa_cargado; 

// Estructura que guarda los elementos (nombre, valor y peso) de los Objetos.
typedef struct Item{
  char* nombre;
  int valor; // Puntaje del Objeto (pts)
  int peso; // Peso del Objeto (kg)
} Item;

// Estructura que guarda cada elemento de la sala en la que se encuentre.
// Es la encargada de poder darle forma a las habitaciones del mapa cargado por el archivo CSV.
typedef struct State_Map{
    int ID; // Identificador de la sala que permite ubicarla por número y crear conexiones con otras salas.
    char* nombre; 
    char* descripcion; 
    List* items; // Lista de Objetos útiles que posee la sala.
    int direcciones[4]; // Guardan el ID de una sala adjunta [Arriba, Abajo, Izquierda, Derecha], y permite saber si existen conexiones con otras salas.
    List* adj_nodes; // Lista de las salas adjuntas a la sala actual.
    char final; // Comprueba si la sala es la final o no.
} State_Map;

// Función que se encarga de leer un archivo CSV y generar un mapa con las salas (nodos) correspondientes.
// Recibe de parámetro un mapa que guardará todos los nodos que se generen.
void leer_mapa_completo(Map*);

#endif