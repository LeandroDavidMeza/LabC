// MODULO PRINCIPAL
// Contiene la función main, el menú de navegación principal
// y la inicialización del flujo general del programa.

#include <stdio.h>
#include "simulador.h"
#include "validaciones.h"

static void mostrarMenu(void) {
    printf("\n--------------------------------------------------\n");
    printf("  MENÚ PRINCIPAL - MISIÓN ARTEMIS (ESCENARIO 3)\n");
    printf("--------------------------------------------------\n");
    printf("  1. Energía cinética inicial (Configuración y Cálculo)\n");
    printf("  2. Análisis de Inercia, Trabajo (Wext) y Equilibrio\n");
    printf("  3. Simulación interactiva de vuelo translunar (Turnos)\n");
    printf("  4. Reporte final de telemetría\n");
    printf("  0. Salir\n");
    printf("--------------------------------------------------\n");
}

int main(void) {
    configurarConsola();
    printf("\tMISIÓN ARTEMIS\nInyección Translunar y Trayectoria Libre\n");

    EstadoMision mision;
    inicializarEstadoMision(&mision);

    int accion;
    do {
        mostrarMenu();
        accion = leerEnteroValido("Ingrese la acción a realizar: ");
        switch (accion) {
        case 0:
            printf("\nSaliendo del programa...\n");
            break;
        case 1:
            mostrarEnergiaCineticaInicial(&mision);
            break;
        case 2:
            mostrarAnalisisConservacionYTrabajo(&mision);
            break;
        case 3:
            ejecutarSimulacionPorTurnos(&mision);
            break;
        case 4:
            mostrarReporteTelemetria(&mision);
            break;
        default:
            printf("\nAcción no válida. Por favor, intente nuevamente.\n");
            break;
        }
    } while (accion != 0);

    return 0;
}