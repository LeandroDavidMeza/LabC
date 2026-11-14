// simulador.h
// Declaración de los tipos de datos estructurados (struct) del simulador,
// constantes de la simulación por turnos y prototipos de las funciones que
// implementan cada opción del programa.

#ifndef SIMULADOR_H
#define SIMULADOR_H

// ---------------- Etapas del viaje a la Luna ----------------
#define CANT_ETAPAS 5
#define ETAPA_EQUILIBRIO 3                 // posición en el arreglo (empieza en 0) del punto de equilibrio
#define DISTANCIA_ETAPA_2_M 100000000.0    // 100.000 km del centro de la Tierra
#define DISTANCIA_ETAPA_3_M 250000000.0    // 250.000 km del centro de la Tierra
#define DISTANCIA_LLEGADA_LUNA_M 2000000.0 // la etapa 5 está a 2.000 km del centro de la Luna

// ---------------- Correcciones de trayectoria (TCM) ----------------
#define DV_TCM_MIN_MS 0.1   // m/s, corrección más chica que se puede pedir
#define DV_TCM_MAX_MS 100.0 // m/s, corrección más grande por maniobra

// Datos (telemetría) de una etapa del viaje
typedef struct {
    int numeroEtapa;
    int alcanzada;           // 1 = la nave llegó a esta etapa, 0 = no llegó
    double distanciaTierra;  // m, desde el centro de la Tierra
    double velocidad;        // m/s
    double energiaCinetica;  // J
    double energiaPotencial; // J
    double deltaV;           // m/s de corrección aplicada (0 = viajó por inercia)
} TelemetriaEtapa;

// Estado completo de la misión Orion
typedef struct {
    double masa;              // kg
    double velocidadInicial;  // m/s (v_TLI)
    double altitudInicial;    // m (altura de la órbita LEO)
    int simulacionRealizada;  // 1 = hay una simulación para mostrar en el reporte
    int misionExitosa;        // 1 = llegó a la Luna
    int cantidadManiobras;    // cantidad de correcciones hechas
    double deltaVTotal;       // m/s, suma de todas las correcciones
    double distanciaMaxima;   // m, hasta dónde llegó la nave si la misión falló
    TelemetriaEtapa historial[CANT_ETAPAS]; // arreglo de structs: una posición por etapa
} EstadoMision;

// Todas las funciones reciben un PUNTERO a la misión (EstadoMision *mision):
// así trabajan sobre la misma variable de main, sin hacer una copia.

void inicializarEstadoMision(EstadoMision *mision);
void mostrarParametrosMision(EstadoMision *mision);

void configurarParametrosMision(EstadoMision *mision);    // Opción 1
void mostrarInercia(EstadoMision *mision);                // Opción 2 - Pregunta a)
void mostrarEnergiaCineticaInicial(EstadoMision *mision); // Opción 3 - Pregunta b)
void mostrarTrabajoHastaEquilibrio(EstadoMision *mision); // Opción 4 - Pregunta c)
void mostrarComparativaIdaVuelta(EstadoMision *mision);   // Opción 5 - Pregunta d)
void ejecutarSimulacionPorTurnos(EstadoMision *mision);   // Opción 6
void mostrarReporteTelemetria(EstadoMision *mision);      // Opción 7

#endif
