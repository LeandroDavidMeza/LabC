// simulador.c
// Definición del flujo de cada opción de la simulación: pide los datos al
// usuario (con validaciones), llama a los cálculos de artemis y muestra
// los resultados.

#include <stdio.h>
#include "simulador.h"
#include "artemis.h"
#include "validaciones.h"

// Explica la situación física, propone los datos de la misión (o permite
// ingresar otros), calcula la energía cinética inicial y la muestra
void mostrarEnergiaCineticaInicial(void) {
    printf("\n--- Energía cinética inicial de la nave ---\n\n");
    printf("Tras la maniobra de inyección translunar (TLI), la etapa ICPS se desacopla\n");
    printf("y los motores se apagan. Desde ese instante la nave viaja por inercia, sin\n");
    printf("empuje. La energía cinética que tiene en ese momento es la 'inicial' del\n");
    printf("viaje: después, la gravedad terrestre la irá frenando.\n");

    // Datos por defecto: los del enunciado
    double masa = MASA_NAVE_ORION_KG;
    double velocidad = VELOCIDAD_ESCAPE_INYECCION_MS;

    printf("\nDatos de la misión: masa %.2f kg, velocidad de inyección %.2f m/s\n", masa, velocidad);

    // Pregunta si se usan los datos de la misión o se ingresan otros
    int opcion;
    do {
        opcion = leerEnteroValido("1 = usar estos datos, 2 = ingresar otros: ");
        if (opcion != 1 && opcion != 2)
            printf("Error: debe ingresar 1 o 2.\n");
    } while (opcion != 1 && opcion != 2);

    if (opcion == 2) {
        masa = leerDoubleEnRango("Masa de la nave (kg): ", MASA_MIN_KG, MASA_MAX_KG);
        velocidad = leerDoubleEnRango("Velocidad de inyección (m/s): ", VELOCIDAD_MIN_MS, VELOCIDAD_MAX_MS);
    }

    double energia = calcEnergiaCinetica(masa, velocidad);

    printf("\nMasa: %.2f kg\n", masa);
    printf("Velocidad: %.2f m/s\n", velocidad);
    printf("Energía cinética inicial: %.2f J\n", energia);
}