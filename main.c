// MODULO PRINCIPAL
// Contiene la función main, el menú de navegación principal
// y la inicialización del flujo general del programa.

#include <stdio.h>
#include "simulador.h"
#include "validaciones.h"

// Muestra las opciones disponibles del menú principal
static void mostrarMenu(void) {     // static indica que la función es privada al archivo simulador.c
    printf("\n--------------------------------------\n");
    printf("  MENÚ PRINCIPAL\n");
    printf("--------------------------------------\n");
    printf("  1. Energía cinética inicial\n");
    printf("  0. Salir\n");
    printf("--------------------------------------\n");
}

int main(void) {
    configurarConsola();
    printf("\tMISIÓN ARTEMIS\nInyección Traslunar y Trayectoria Libre\n");

    int accion;
    do
    {
        mostrarMenu();
        accion = leerEnteroValido("Ingrese la acción a realizar: ");
        switch (accion)
        {
        case 0:
            printf("\nSaliendo del programa...\n");
            break;
        case 1:
            mostrarEnergiaCineticaInicial();
            break;
        default:
            printf("\nAcción no válida. Por favor, intente nuevamente.\n");
            break;
        }
    } while (accion != 0);

    return 0;
}