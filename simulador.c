// simulador.c
// Definición del flujo de cada opción de la simulación: pide los datos al
// usuario (con validaciones), llama a los cálculos de artemis y muestra
// los resultados.

#include <stdio.h>
#include "simulador.h"
#include "artemis.h"
#include "validaciones.h"

void inicializarEstadoMision(EstadoMision *mision) {
    mision->masa = MASA_NAVE_ORION_KG;
    mision->velocidadInicial = VELOCIDAD_ESCAPE_INYECCION_MS;
    mision->altitudInicial = ALTITUD_LEO_M;
    mision->simulacionRealizada = 0;
    mision->misionExitosa = 0;
    mision->combustibleExtraKg = 0.0;
}

void mostrarEnergiaCineticaInicial(EstadoMision *mision) {
    printf("\n--- Energía cinética inicial de la nave (Pregunta 2) ---\n\n");
    printf("Tras la maniobra de inyección translunar (TLI), la etapa ICPS se desacopla\n");
    printf("y los motores se apagan. Desde ese instante la nave viaja por inercia, sin\n");
    printf("empuje. La energía cinética que tiene en ese momento es la 'inicial' del\n");
    printf("viaje: después, la gravedad terrestre la irá frenando.\n");

    double masa = mision->masa;
    double velocidad = mision->velocidadInicial;

    printf("\nDatos actuales: masa %.2f kg, velocidad de inyección %.2f m/s\n", masa, velocidad);

    int opcion = leerEnteroEnRango("1 = usar datos oficiales de la misión, 2 = ingresar otros: ", 1, 2);

    if (opcion == 1) {
        masa = MASA_NAVE_ORION_KG;
        velocidad = VELOCIDAD_ESCAPE_INYECCION_MS;
    } else {
        masa = leerDoubleEnRango("Masa de la nave (kg): ", MASA_MIN_KG, MASA_MAX_KG);
        velocidad = leerDoubleEnRango("Velocidad de inyección (m/s): ", VELOCIDAD_MIN_MS, VELOCIDAD_MAX_MS);
    }

    mision->masa = masa;
    mision->velocidadInicial = velocidad;

    double energia = calcEnergiaCinetica(masa, velocidad);

    printf("\nMasa: %.2f kg\n", masa);
    printf("Velocidad: %.2f m/s\n", velocidad);
    printf("Energía cinética inicial (Ki): %.4e J (%.2f TeraJoules)\n", energia, energia / 1e12);
    pausarConsola();
}

void mostrarAnalisisConservacionYTrabajo(const EstadoMision *mision) {
    printf("\n--- Análisis de Inercia, Conservación de Energía y Trabajo (Preguntas 1, 3 y 4) ---\n");

    double rInicial = RADIO_TIERRA_M + mision->altitudInicial;
    double rEquilibrio = calcPuntoEquilibrioGravitatorio();
    double trabajoPorKg = calcTrabajoPorUnidadMasa(rInicial, rEquilibrio);
    double trabajoTotal = trabajoPorKg * mision->masa;

    double rSupLunaDesdeTierra = DISTANCIA_MEDIA_TIERRA_LUNA_M - RADIO_LUNA_M;
    double trabajoRegresoPorKg = calcTrabajoPorUnidadMasa(rSupLunaDesdeTierra, rEquilibrio);

    printf("\n[Pregunta 1 - 1.ª Ley de Newton (Inercia)]:\n");
    printf("  Al apagarse la etapa ICPS, no hay empuje activo. Por inercia, Orion mantiene\n");
    printf("  su movimiento en el vacío, mientras la gravedad terrestre actúa en sentido\n");
    printf("  opuesto desacelerándola progresivamente (convierte Ec en Ep gravitatoria).\n");

    printf("\n[Pregunta 3 - Trabajo adicional mínimo hasta el Punto de Equilibrio]:\n");
    printf("  * Altitud LEO inicial:           %.1f km (r = %.1f km del centro terrestre)\n",
           mision->altitudInicial / 1000.0, rInicial / 1000.0);
    printf("  * Punto de equilibrio T-L:       %.2f km del centro terrestre\n", rEquilibrio / 1000.0);
    printf("  * Trabajo por unidad de masa:    %.4e J/kg (%.2f MJ/kg)\n", trabajoPorKg, trabajoPorKg / 1e6);
    printf("  * Trabajo total (m = %.0f kg):  %.4e J\n", mision->masa, trabajoTotal);

    printf("\n[Pregunta 4 - Justificación Energética Tierra-Luna vs. Luna-Tierra]:\n");
    printf("  La Tierra tiene ~81 veces más masa que la Luna. El punto de equilibrio está\n");
    printf("  al 90%% del camino hacia la Luna (a %.0f km de la Tierra y solo a %.0f km\n",
           rEquilibrio / 1000.0, (DISTANCIA_MEDIA_TIERRA_LUNA_M - rEquilibrio) / 1000.0);
    printf("  de la Luna).\n");
    printf("  * Energía p/ ir de LEO al equilibrio:      %.2f MJ/kg\n", trabajoPorKg / 1e6);
    printf("  * Energía p/ volver de Luna al equilibrio: %.2f MJ/kg (~22 veces menor)\n",
           trabajoRegresoPorKg / 1e6);

    pausarConsola();
}

void ejecutarSimulacionPorTurnos(EstadoMision *mision) {
    printf("\n--- Simulación Interactiva de Vuelo Translunar (5 Etapas) ---\n");
    printf("Parámetros actuales: Masa = %.1f kg | v_TLI = %.1f m/s | Altitud LEO = %.1f km\n",
           mision->masa, mision->velocidadInicial, mision->altitudInicial / 1000.0);

    int cambiar = leerEnteroEnRango("¿Desea mantener estos parámetros (1) o configurar nuevos (2)?: ", 1, 2);
    if (cambiar == 2) {
        mision->masa = leerDoubleEnRango("Masa de la nave (kg): ", MASA_MIN_KG, MASA_MAX_KG);
        mision->velocidadInicial = leerDoubleEnRango("Velocidad TLI (m/s): ", VELOCIDAD_MIN_MS, VELOCIDAD_MAX_MS);
        mision->altitudInicial = leerDoubleEnRango("Altitud LEO (m): ", ALTITUD_MIN_M, ALTITUD_MAX_M);
    }

    double rInicial = RADIO_TIERRA_M + mision->altitudInicial;
    double rEquilibrio = calcPuntoEquilibrioGravitatorio();

    double distanciasEtapas[CANT_ETAPAS] = {
        rInicial,
        100000000.0,
        250000000.0,
        rEquilibrio,
        DISTANCIA_MEDIA_TIERRA_LUNA_M - 2000000.0
    };

    double ecInicial = calcEnergiaCinetica(mision->masa, mision->velocidadInicial);
    double epInicial = calcEnergiaPotencialSistema(mision->masa, rInicial);
    double energiaMecanica = ecInicial + epInicial;

    mision->combustibleExtraKg = 0.0;
    mision->misionExitosa = 1;

    for (int i = 0; i < CANT_ETAPAS; i++) {
        double r = distanciasEtapas[i];
        double epActual = calcEnergiaPotencialSistema(mision->masa, r);
        double ecActual = energiaMecanica - epActual;
        int maniobra = 0;

        printf("\n------------------------------------------------------------\n");
        printf(" ETAPA %d/%d - Distancia al centro terrestre: %.1f km\n", i + 1, CANT_ETAPAS, r / 1000.0);

        if (ecActual <= 0.0) {
            printf(" [ALERTA] Energía cinética agotada. La gravedad terrestre frenó la nave.\n");
            int resp = leerEnteroEnRango(" ¿Activar propulsores auxiliares de emergencia? (1=Sí, 2=No): ", 1, 2);
            if (resp == 1) {
                energiaMecanica += calcEnergiaCinetica(mision->masa, 1500.0);
                ecActual = energiaMecanica - epActual;
                mision->combustibleExtraKg += 180.0;
                maniobra = 1;
                printf(" [OK] Impulso auxiliar ejecutado. Trayectoria recuperada.\n");
            } else {
                for (int j = i; j < CANT_ETAPAS; j++) {
                    mision->historial[j].numeroEtapa = j + 1;
                    mision->historial[j].distanciaTierra = distanciasEtapas[j];
                    mision->historial[j].velocidad = 0.0;
                    mision->historial[j].energiaCinetica = 0.0;
                    mision->historial[j].energiaPotencial = epActual;
                    mision->historial[j].maniobraRealizada = 0;
                }
                mision->misionExitosa = 0;
                printf("\n [MISIÓN ABORTADA] Orion cae de regreso hacia la Tierra.\n");
                break;
            }
        }

        double vActual = calcVelocidadDesdeEnergia(ecActual, mision->masa);
        printf(" * Velocidad actual:     %.2f m/s (%.2f km/h)\n", vActual, vActual * 3.6);
        printf(" * Energía Cinética:     %.4e J\n", ecActual);
        printf(" * Energía Potencial:    %.4e J\n", epActual);

        if (i == 2 || i == 3) {
            if (i == 3) {
                printf(" [HITO] Punto de equilibrio gravitatorio alcanzado. La Luna comienza a atraer a Orion.\n");
            }
            int dec = leerEnteroEnRango(
                " Decisión de vuelo: 1 = Mantener inercia pura | 2 = Pulso corrector TCM (+50 m/s): ", 1, 2);
            if (dec == 2) {
                vActual += 50.0;
                ecActual = calcEnergiaCinetica(mision->masa, vActual);
                energiaMecanica = ecActual + epActual;
                mision->combustibleExtraKg += 25.0;
                maniobra = 1;
                printf(" [OK] Corrección aplicada. Nueva velocidad: %.2f m/s\n", vActual);
            }
        }

        mision->historial[i].numeroEtapa = i + 1;
        mision->historial[i].distanciaTierra = r;
        mision->historial[i].velocidad = vActual;
        mision->historial[i].energiaCinetica = ecActual;
        mision->historial[i].energiaPotencial = epActual;
        mision->historial[i].maniobraRealizada = maniobra;
    }

    mision->simulacionRealizada = 1;
    if (mision->misionExitosa) {
        printf("\n[ÉXITO] La nave Orion completó la inyección translunar correctamente.\n");
    }
    pausarConsola();
}

void mostrarReporteTelemetria(const EstadoMision *mision) {
    printf("\n================================================================================\n");
    printf("                     REPORTE FINAL DE TELEMETRÍA TRANSLUNAR                     \n");
    printf("================================================================================\n");

    if (!mision->simulacionRealizada) {
        printf(" Aún no se ha corrido la simulación por turnos (Opción 3 del menú).\n");
        pausarConsola();
        return;
    }

    printf(" Resultado: %s | Masa: %.1f kg | v_TLI: %.1f m/s | Combustible extra: %.1f kg\n",
           mision->misionExitosa ? "MISIÓN EXITOSA" : "MISIÓN ABORTADA",
           mision->masa, mision->velocidadInicial, mision->combustibleExtraKg);
    printf("--------------------------------------------------------------------------------\n");
    printf(" Etapa | Dist. Tierra (km) | Velocidad (m/s) | E. Cinética (J) | Propulsión\n");
    printf("--------------------------------------------------------------------------------\n");

    for (int i = 0; i < CANT_ETAPAS; i++) {
        TelemetriaEtapa e = mision->historial[i];
        printf("   %d   |    %10.1f     |    %9.2f    |   %11.3e   | %s\n",
               e.numeroEtapa,
               e.distanciaTierra / 1000.0,
               e.velocidad,
               e.energiaCinetica,
               e.maniobraRealizada ? "Pulso Activo" : "Inercia (OFF)");
    }
    printf("================================================================================\n");
    pausarConsola();
}