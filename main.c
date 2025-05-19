#include "headers/graphquest.h"

void pantalla_carga() {
    for (int i = 0; i < 50; i++) {
        limpiar_pantalla();
        printf("\033[?25l\033[37m[");
        for (int j = 0; j < i; j++) printf("#");
        for (int j = i; j < 50; j++) printf(" ");
        printf("]");
        fflush(stdout);
        printf("\033[0m\033[?25h");
        sleep_ms(120);
    }
}

void pantalla_inicio() {
    char o;
    mostrar_menu_principal();
    leer_opcion(&o);
    switch (o) {
        case '1': { leer_escenarios(); esperar_enter(); break; }
        case '0': { return;}
    }
}

int main() {
    pantalla_carga();
    pantalla_inicio();

    limpiar_pantalla();
    return 0;
}