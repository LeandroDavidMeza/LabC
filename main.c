// MODULO PRINCIPAL
// Contine la función main, menu de navegación principal
// y la inicialización del flujo general del programa

#include <stdio.h>
#include "artemis.h"
#include "simulador.h"
#include "validaciones.h"

int main(void) {
    printf("\tMISIÓN ARTEMIS\n\tInyección Traslunar y Trayectoria Libre\n");
    printf("\n--------------------------------------\n\n");
    int accion;
    do
    {
        accion = leerEnteroValido("\nIngrese la acción a realizar (0 para salir): ");
        switch (accion)
        {
        case 0:
            printf("\nSaliendo del programa...\n");
            break;
        case 1:
            printf("\nIniciando simulación de inyección traslunar...\n");
            break;
        case 2:
            printf("\nIniciando simulación de trayectoria libre...\n");
            break;
        case 3:
            printf("\nMostrando resultados de la simulación...\n");
            break;
        default:
            printf("\nAcción no válida. Por favor, intente nuevamente.\n");
            break;
        }
    } while (accion != 0);
    return 0;
}