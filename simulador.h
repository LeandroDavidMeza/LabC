// simulador.h
// Declaración de los tipos de datos estructurados (struct) del simulador y
// prototipos de las funciones que implementan cada opción del programa.

#ifndef SIMULADOR_H
#define SIMULADOR_H

#define CANT_ETAPAS 5

// Estructura para registrar la telemetría de cada tramo del viaje
typedef struct {
    int numeroEtapa;
    double distanciaTierra;
    double velocidad;
    double energiaCinetica;
    double energiaPotencial;
    int maniobraRealizada; // 0 = Inercia pura, 1 = Corrección TCM / Auxiliar
} TelemetriaEtapa;

// Estructura principal que almacena el estado de la misión Orion
typedef struct {
    double masa;
    double velocidadInicial;
    double altitudInicial;
    int simulacionRealizada;
    int misionExitosa;
    double combustibleExtraKg;
    TelemetriaEtapa historial[CANT_ETAPAS]; // Arreglo de estructuras
} EstadoMision;

// Inicializa la estructura con los valores por defecto de Artemis (usa puntero)
void inicializarEstadoMision(EstadoMision *mision);

// Opción 1 (Original + actualiza el struct por puntero)
void mostrarEnergiaCineticaInicial(EstadoMision *mision);

// Opción 2: Responde Preguntas 1, 3 y 4 (Inercia, Trabajo W_ext/m y Comparativa Lunar)
void mostrarAnalisisConservacionYTrabajo(const EstadoMision *mision);

// Opción 3: Simulación interactiva por turnos a través de las 5 etapas hacia la Luna
void ejecutarSimulacionPorTurnos(EstadoMision *mision);

// Opción 4: Muestra el reporte tabular recorriendo el arreglo de telemetría
void mostrarReporteTelemetria(const EstadoMision *mision);

#endif