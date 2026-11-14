// MODULO PRINCIPAL
// Contiene la función main, el menú de navegación principal
// y la inicialización del flujo general del programa.

#include <stdio.h>
#include "simulador.h"
#include "validaciones.h"

void mostrarMenu(EstadoMision *mision) {
    printf("\n------------------------------------------------------------------------\n");
    printf("  MENÚ PRINCIPAL - MISIÓN ARTEMIS (ESCENARIO 3)\n");
    printf("  ");
    mostrarParametrosMision(mision);
    printf("------------------------------------------------------------------------\n");
    printf("  1. Configurar parámetros de la misión\n");
    printf("  2. Pregunta a) Inercia tras el apagado de los motores (1.ª Ley de Newton)\n");
    printf("  3. Pregunta b) Energía cinética inicial\n");
    printf("  4. Pregunta c) Trabajo mínimo hasta el punto de equilibrio\n");
    printf("  5. Pregunta d) Comparación energética ida vs. vuelta\n");
    printf("  6. Simulación interactiva de vuelo translunar (por turnos)\n");
    printf("  7. Reporte final de telemetría y desempeño\n");
    printf("  0. Salir\n");
    printf("------------------------------------------------------------------------\n");
}

int main(void) {
    configurarConsola();
    printf("\tMISIÓN ARTEMIS\nInyección Translunar y Trayectoria Libre\n");

    EstadoMision mision;              // toda la información de la misión
    inicializarEstadoMision(&mision); // &mision = dirección de memoria (puntero)

    int salir = 0;
    while (salir == 0) {
        mostrarMenu(&mision);
        int opcion = leerEnteroEnRango("Ingrese la opción", 0, 7);
        switch (opcion) {
        case 0:
            if (leerSiNo("¿Confirma que desea salir?") == 'S') {
                printf("\nSaliendo del programa...\n");
                salir = 1;
            }
            break;
        case 1:
            configurarParametrosMision(&mision);
            pausarConsola();
            break;
        case 2:
            mostrarInercia(&mision);
            break;
        case 3:
            mostrarEnergiaCineticaInicial(&mision);
            break;
        case 4:
            mostrarTrabajoHastaEquilibrio(&mision);
            break;
        case 5:
            mostrarComparativaIdaVuelta(&mision);
            break;
        case 6:
            ejecutarSimulacionPorTurnos(&mision);
            break;
        case 7:
            mostrarReporteTelemetria(&mision);
            break;
        }
    }

    return 0;
}
