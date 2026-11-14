// simulador.c
// Definición del flujo de cada opción del programa: pide los datos al
// usuario (con validaciones), llama a los cálculos de artemis y muestra
// los resultados. Incluye la simulación por turnos y el reporte final.

#include <stdio.h>
#include "simulador.h"
#include "artemis.h"
#include "validaciones.h"

// ======================= Funciones auxiliares =======================
// Se usan solamente dentro de este archivo, por eso no están en simulador.h.

// Distancia desde el centro de la Tierra hasta la órbita inicial (LEO)
double calcRadioInicial(EstadoMision *mision) {
    return RADIO_TIERRA_M + mision->altitudInicial;
}

// Carga en el arreglo la distancia (al centro de la Tierra) de cada etapa
void cargarDistanciasEtapas(double distancias[], double rInicial) {
    distancias[0] = rInicial;
    distancias[1] = DISTANCIA_ETAPA_2_M;
    distancias[2] = DISTANCIA_ETAPA_3_M;
    distancias[3] = calcPuntoEquilibrioGravitatorio();
    distancias[4] = DISTANCIA_MEDIA_TIERRA_LUNA_M - DISTANCIA_LLEGADA_LUNA_M;
}

void mostrarNombreEtapa(int etapa) {
    switch (etapa) {
    case 0:
        printf("Fin de la inyección translunar (órbita LEO)");
        break;
    case 1:
    case 2:
        printf("Crucero translunar");
        break;
    case 3:
        printf("Punto de equilibrio Tierra-Luna");
        break;
    case 4:
        printf("Aproximación a la Luna");
        break;
    }
}

// Antes del punto de equilibrio: muestra si a la nave le alcanza la energía para
// llegar a él. Si no le alcanza, muestra cuánto tendría que corregir en este lugar.
void mostrarMargenEnergetico(double energiaMecanicaPorKg, double v, double rEquilibrio) {
    // Energía cinética por kilo que le quedaría al llegar al equilibrio: K = Em - U
    // Positiva = le sobra energía. Negativa = le falta.
    double margenPorKg = energiaMecanicaPorKg - calcEnergiaPotencialSistema(1.0, rEquilibrio);

    printf(" * Margen de energía para llegar al equilibrio: %.2f kJ/kg\n", margenPorKg / 1000.0);
    if (margenPorKg > 0.0) {
        printf("   -> Le alcanza: siguiendo por inercia llega al punto de equilibrio.\n");
    } else {
        double dvMinimo = calcDeltaVMinimo(v, -margenPorKg);
        printf("   -> [ALERTA] NO le alcanza. Corrección mínima en esta etapa: %.2f m/s\n", dvMinimo);
        if (dvMinimo > DV_TCM_MAX_MS) {
            printf("   -> Es más que el máximo permitido por maniobra (%.0f m/s).\n", DV_TCM_MAX_MS);
        }
    }
}

// Pregunta al jugador qué hacer y devuelve el Δv elegido (0 = seguir por inercia)
double pedirCorreccion(void) {
    double deltaV = 0.0;
    int decision = leerEnteroEnRango(
        " Decisión: 1 = Seguir por inercia | 2 = Encender motor (corrección TCM)", 1, 2);
    if (decision == 2) {
        deltaV = leerRealEnRango(" Δv de la corrección (m/s)", DV_TCM_MIN_MS, DV_TCM_MAX_MS);
    }
    return deltaV;
}

// Califica la misión comparando el Δv usado con el mínimo necesario
void mostrarCalificacion(EstadoMision *mision, double dvMinimo) {
    if (mision->misionExitosa == 0) {
        printf(" Desempeño: FALLIDA (la nave no llegó a la Luna)\n");
    } else if (mision->deltaVTotal == 0.0) {
        printf(" Desempeño: EXCELENTE (llegó por inercia pura, sin correcciones)\n");
    } else if (dvMinimo == 0.0) {
        printf(" Desempeño: BUENA (llegó, pero hizo correcciones que no hacían falta)\n");
    } else {
        double eficiencia = dvMinimo / mision->deltaVTotal * 100.0;
        printf(" Eficiencia en el uso de Δv: %.1f %%\n", eficiencia);
        if (eficiencia >= 90.0) {
            printf(" Desempeño: EXCELENTE (correcciones casi perfectas)\n");
        } else if (eficiencia >= 50.0) {
            printf(" Desempeño: BUENA (corregir antes hubiera ahorrado Δv)\n");
        } else {
            printf(" Desempeño: MEJORABLE (se gastó mucho más Δv del necesario)\n");
        }
    }
}

// ======================= Funciones del menú =======================

void inicializarEstadoMision(EstadoMision *mision) {
    mision->masa = MASA_NAVE_ORION_KG;
    mision->velocidadInicial = VELOCIDAD_TLI_MS;
    mision->altitudInicial = ALTITUD_LEO_M;
    mision->simulacionRealizada = 0;
    mision->misionExitosa = 0;
    mision->cantidadManiobras = 0;
    mision->deltaVTotal = 0.0;
    mision->distanciaMaxima = 0.0;
}

void mostrarParametrosMision(EstadoMision *mision) {
    printf("Masa = %.1f kg | v_TLI = %.1f m/s | Altitud LEO = %.1f km\n",
           mision->masa, mision->velocidadInicial, mision->altitudInicial / 1000.0);
}

void configurarParametrosMision(EstadoMision *mision) {
    printf("\n--- Configuración de parámetros de la misión ---\n");
    printf("Parámetros actuales: ");
    mostrarParametrosMision(mision);

    if (leerSiNo("¿Usar los datos oficiales del Escenario 3?") == 'S') {
        mision->masa = MASA_NAVE_ORION_KG;
        mision->velocidadInicial = VELOCIDAD_TLI_MS;
        mision->altitudInicial = ALTITUD_LEO_M;
    } else {
        mision->masa = leerRealEnRango("Masa de la nave (kg)", MASA_MIN_KG, MASA_MAX_KG);
        mision->velocidadInicial = leerRealEnRango("Velocidad de inyección v_TLI (m/s)",
                                                   VELOCIDAD_MIN_MS, VELOCIDAD_MAX_MS);
        double altitudKm = leerRealEnRango("Altitud de la órbita LEO (km)", ALTITUD_MIN_KM, ALTITUD_MAX_KM);
        mision->altitudInicial = altitudKm * 1000.0; // se guarda en metros
    }

    // Si ya se había simulado, ese reporte era con otros datos: se descarta
    if (mision->simulacionRealizada == 1) {
        mision->simulacionRealizada = 0;
        printf("\nAviso: se descartó el reporte anterior porque cambiaron los parámetros.\n");
    }
    printf("\nParámetros guardados: ");
    mostrarParametrosMision(mision);
}

void mostrarInercia(EstadoMision *mision) {
    double rInicial = calcRadioInicial(mision);
    double vEscape = calcVelocidadEscape(rInicial);
    double distancias[CANT_ETAPAS];
    cargarDistanciasEtapas(distancias, rInicial);

    printf("\n--- Pregunta a) 1.ª Ley de Newton: movimiento por inercia ---\n\n");
    printf("Tras la inyección translunar (TLI), la etapa ICPS se desacopla y los motores se\n");
    printf("apagan: sobre Orion ya no actúa ningún empuje y en el vacío no hay rozamiento.\n");
    printf("Por la 1.ª Ley de Newton, si la fuerza neta fuera cero la nave mantendría su\n");
    printf("velocidad (MRU): no necesita motores para seguir avanzando, viaja por inercia.\n\n");
    printf("Pero la fuerza neta NO es cero: la gravedad terrestre apunta hacia la Tierra,\n");
    printf("en sentido contrario al avance, y frena a la nave. Como F = G*M_T*m/r², ese\n");
    printf("frenado es cada vez más débil a medida que la nave se aleja, y la energía\n");
    printf("cinética se va transformando en energía potencial gravitatoria:\n\n");

    printf("  Etapa | Distancia al centro terrestre | Aceleración gravitatoria neta\n");
    printf("  ------+-------------------------------+------------------------------\n");
    for (int i = 0; i < CANT_ETAPAS; i++) {
        printf("    %d   | %26.1f km | %22.5f m/s²\n",
               i + 1, distancias[i] / 1000.0, calcAceleracionNeta(distancias[i]));
    }
    printf("  (negativa = hacia la Tierra, frena a la nave | positiva = hacia la Luna)\n\n");
    printf("En el punto de equilibrio (etapa 4) las atracciones de la Tierra y de la Luna\n");
    printf("se compensan; a partir de ahí la Luna atrae a la nave y la acelera.\n\n");

    printf("Dato extra: la velocidad de escape a %.1f km del centro terrestre es\n", rInicial / 1000.0);
    printf("v_esc = %.2f m/s. ", vEscape);
    if (mision->velocidadInicial < vEscape) {
        printf("Como v_TLI = %.2f m/s es menor, la nave sigue\n", mision->velocidadInicial);
        printf("atrapada por la gravedad terrestre: si no vuelve a encender los motores,\n");
        printf("termina regresando. Eso permite la trayectoria de retorno libre.\n");
    } else {
        printf("Como v_TLI = %.2f m/s es mayor o igual, la nave\n", mision->velocidadInicial);
        printf("escaparía de la gravedad terrestre: ya no sería una trayectoria de retorno libre.\n");
    }
    pausarConsola();
}

void mostrarEnergiaCineticaInicial(EstadoMision *mision) {
    double energia = calcEnergiaCinetica(mision->masa, mision->velocidadInicial);

    printf("\n--- Pregunta b) Energía cinética inicial de la nave ---\n\n");
    printf("Al apagarse los motores tras la TLI empieza el viaje por inercia. La energía\n");
    printf("cinética en ese instante es la inicial del viaje; después la gravedad la va\n");
    printf("transformando en energía potencial.\n\n");
    printf("  Ki = 1/2 * m * v²\n");
    printf("  m  = %.2f kg\n", mision->masa);
    printf("  v  = %.2f m/s\n", mision->velocidadInicial);
    printf("  Ki = %.4e J  (%.4f TJ)\n", energia, energia / 1e12);
    printf("  Ki por kilo de nave = %.4f MJ/kg\n", energia / mision->masa / 1e6);
    printf("\n(Para calcular con otros valores, cámbielos en la opción 1 del menú.)\n");
    pausarConsola();
}

void mostrarTrabajoHastaEquilibrio(EstadoMision *mision) {
    double m = mision->masa;
    double v = mision->velocidadInicial;
    double rInicial = calcRadioInicial(mision);
    double rEquilibrio = calcPuntoEquilibrioGravitatorio();
    double vOrbital = calcVelocidadOrbital(rInicial);

    // W_ext/m = ΔU/m - Ki/m  (llegando al equilibrio con velocidad 0)
    double wDesdeReposo = calcTrabajoPorUnidadMasa(rInicial, rEquilibrio);   // Ki = 0
    double wDesdeOrbita = wDesdeReposo - calcEnergiaCinetica(1.0, vOrbital); // Ki = orbital
    double wDespuesTLI = wDesdeReposo - calcEnergiaCinetica(1.0, v);         // Ki = TLI
    double vMinima = calcVelocidadDesdeEnergia(wDesdeReposo, 1.0);

    printf("\n--- Pregunta c) Trabajo mínimo hasta el punto de equilibrio Tierra-Luna ---\n\n");
    printf("Planteo: si actúan fuerzas externas, W_ext = ΔEm = ΔK + ΔU   (Em = K + U).\n");
    printf("El trabajo es mínimo si la nave llega al punto de equilibrio con v = 0, porque\n");
    printf("desde ahí la Luna la atrae sola. Entonces, por kilo: W_ext/m = ΔU/m - Ki/m\n");
    printf("con U = -G*M_T*m/r - G*M_L*m/(D - r)   (atracción de la Tierra + la Luna)\n\n");

    printf("  * Radio inicial r0 = R_T + h:   %12.1f km\n", rInicial / 1000.0);
    printf("  * Punto de equilibrio r_eq:     %12.2f km\n", rEquilibrio / 1000.0);
    printf("  * ΔU/m = U(r_eq)/m - U(r0)/m:   %12.4f MJ/kg\n\n", wDesdeReposo / 1e6);

    printf("  El resultado depende de con qué velocidad arranca la nave en r0:\n");
    printf("  1) Partiendo quieta (Ki = 0):\n");
    printf("     W_ext/m = %.4f MJ/kg  ->  W_ext = %.4e J (m = %.0f kg)\n",
           wDesdeReposo / 1e6, wDesdeReposo * m, m);
    printf("  2) Partiendo de la órbita circular LEO (v_orb = %.2f m/s):\n", vOrbital);
    printf("     W_ext/m = %.4f MJ/kg  ->  W_ext = %.4e J\n", wDesdeOrbita / 1e6, wDesdeOrbita * m);
    printf("  3) Después de la TLI (v_TLI = %.2f m/s): trabajo ADICIONAL\n", v);
    printf("     W_ext/m = %.4f MJ/kg  ->  W_ext = %.4e J\n", wDespuesTLI / 1e6, wDespuesTLI * m);

    printf("\n  Velocidad mínima de inyección para llegar al equilibrio: %.2f m/s\n", vMinima);
    if (wDespuesTLI > 0.0) {
        printf("  Conclusión: con v_TLI = %.2f m/s la energía NO alcanza, por muy poco.\n", v);
        printf("  Faltan %.2f kJ/kg, que equivalen a sumar %.2f m/s justo después de la TLI\n",
               wDespuesTLI / 1000.0, calcDeltaVMinimo(v, wDespuesTLI));
        printf("  (se puede probar en la simulación, opción 6).\n");
    } else {
        printf("  Conclusión: con v_TLI = %.2f m/s la energía alcanza: no hace falta trabajo\n", v);
        printf("  adicional (sobran %.2f kJ/kg).\n", -wDespuesTLI / 1000.0);
    }
    printf("\n  Nota: el modelo supone un viaje en línea recta Tierra-Luna y la Luna quieta.\n");
    printf("  En la misión real la trayectoria es curva y la Luna se mueve en su órbita.\n");
    pausarConsola();
}

void mostrarComparativaIdaVuelta(EstadoMision *mision) {
    double rInicial = calcRadioInicial(mision);
    double rEquilibrio = calcPuntoEquilibrioGravitatorio();
    double rSuperficieLuna = DISTANCIA_MEDIA_TIERRA_LUNA_M - RADIO_LUNA_M;

    double wSuperficieTierra = calcTrabajoPorUnidadMasa(RADIO_TIERRA_M, rEquilibrio);
    double wLeo = calcTrabajoPorUnidadMasa(rInicial, rEquilibrio);
    double wSuperficieLuna = calcTrabajoPorUnidadMasa(rSuperficieLuna, rEquilibrio);

    printf("\n--- Pregunta d) ¿Por qué ir a la Luna gasta mucho más combustible que volver? ---\n\n");
    printf("  * La Tierra tiene %.1f veces más masa que la Luna, así que atrae mucho más:\n",
           MASA_TIERRA_KG / MASA_LUNA_KG);
    printf("    su \"pozo\" de energía potencial es mucho más profundo.\n");
    printf("  * Por eso el punto de equilibrio está al %.1f %% del camino: a %.0f km de la\n",
           rEquilibrio / DISTANCIA_MEDIA_TIERRA_LUNA_M * 100.0, rEquilibrio / 1000.0);
    printf("    Tierra y a solo %.0f km de la Luna.\n\n", (DISTANCIA_MEDIA_TIERRA_LUNA_M - rEquilibrio) / 1000.0);

    printf("  Energía mínima por kilo para llegar al punto de equilibrio (partiendo quieta):\n");
    printf("    Desde la superficie de la Tierra: %7.2f MJ/kg\n", wSuperficieTierra / 1e6);
    printf("    Desde la órbita LEO:              %7.2f MJ/kg\n", wLeo / 1e6);
    printf("    Desde la superficie de la Luna:   %7.2f MJ/kg\n", wSuperficieLuna / 1e6);
    printf("\n  -> Desde LEO hace falta %.1f veces más energía por kilo que desde la Luna\n",
           wLeo / wSuperficieLuna);
    printf("     (%.1f veces más si se sale desde la superficie de la Tierra).\n",
           wSuperficieTierra / wSuperficieLuna);

    printf("\n  Pasado el punto de equilibrio, la gravedad del destino hace el resto: en el\n");
    printf("  regreso es la Tierra la que acelera a la nave, sin gastar combustible.\n");
    printf("  Además, el combustible también pesa: para llevar más combustible hay que\n");
    printf("  gastar todavía más combustible. Por eso la diferencia en kilos de combustible\n");
    printf("  entre la ida y la vuelta es muy grande.\n");
    pausarConsola();
}

void ejecutarSimulacionPorTurnos(EstadoMision *mision) {
    printf("\n--- Simulación interactiva de vuelo translunar (%d etapas) ---\n\n", CANT_ETAPAS);
    printf("Objetivo: llevar a Orion desde la órbita LEO hasta la Luna gastando el menor Δv\n");
    printf("posible (Δv = cuánto aumentan la velocidad los motores).\n");
    printf("Tras la TLI la nave viaja por inercia y la gravedad terrestre la frena. Si su\n");
    printf("energía no alcanza para pasar el punto de equilibrio Tierra-Luna, se detiene y\n");
    printf("vuelve a caer. En las etapas 1 a 3 puede encender el motor para corregir\n");
    printf("(corrección TCM de hasta %.0f m/s).\n", DV_TCM_MAX_MS);
    printf("Pista: corregir temprano, cuando la nave va más rápido, cuesta menos Δv.\n\n");
    printf("Parámetros actuales: ");
    mostrarParametrosMision(mision);
    if (leerSiNo("¿Desea cambiar los parámetros antes de empezar?") == 'S') {
        configurarParametrosMision(mision);
    }

    double m = mision->masa;
    double rInicial = calcRadioInicial(mision);
    double distancias[CANT_ETAPAS];
    cargarDistanciasEtapas(distancias, rInicial);
    double rEquilibrio = distancias[ETAPA_EQUILIBRIO];

    // Energía mecánica inicial (Em = K + U). Mientras la nave viaja por inercia se
    // conserva; solo cambia cuando se enciende el motor.
    double energiaMecanica = calcEnergiaCinetica(m, mision->velocidadInicial)
                           + calcEnergiaPotencialSistema(m, rInicial);

    mision->misionExitosa = 0;
    mision->cantidadManiobras = 0;
    mision->deltaVTotal = 0.0;
    mision->distanciaMaxima = 0.0;

    int etapa = 0;
    int enVuelo = 1; // pasa a 0 si la nave no puede seguir
    while (etapa < CANT_ETAPAS && enVuelo == 1) {
        double r = distancias[etapa];
        double ep = calcEnergiaPotencialSistema(m, r);
        double ec = energiaMecanica - ep; // conservación de la energía: K = Em - U

        if (ec <= 0.0) {
            // No le alcanza la energía para llegar a esta etapa: la misión falla
            double rAnterior = rInicial;
            if (etapa > 0) {
                rAnterior = distancias[etapa - 1];
            }
            mision->distanciaMaxima = calcDistanciaMaxima(energiaMecanica / m, rAnterior);

            // Las etapas que faltaban quedan marcadas como no alcanzadas
            for (int j = etapa; j < CANT_ETAPAS; j++) {
                mision->historial[j].numeroEtapa = j + 1;
                mision->historial[j].alcanzada = 0;
                mision->historial[j].distanciaTierra = distancias[j];
            }

            printf("\n------------------------------------------------------------\n");
            printf(" [MISIÓN FALLIDA] Orion no alcanza la etapa %d (%.1f km).\n", etapa + 1, r / 1000.0);
            printf(" Se queda sin energía cinética a unos %.0f km del centro de la Tierra:\n",
                   mision->distanciaMaxima / 1000.0);
            printf(" ahí se frena y la gravedad terrestre la hace caer de regreso.\n");
            enVuelo = 0;
        } else {
            double v = calcVelocidadDesdeEnergia(ec, m);
            double deltaV = 0.0;

            printf("\n------------------------------------------------------------\n");
            printf(" ETAPA %d/%d - ", etapa + 1, CANT_ETAPAS);
            mostrarNombreEtapa(etapa);
            printf("\n * Distancia al centro terrestre: %.1f km\n", r / 1000.0);
            printf(" * Velocidad:                     %.2f m/s (%.2f km/h)\n", v, v * 3.6);
            printf(" * Energía cinética (K):          %.4e J\n", ec);
            printf(" * Energía potencial (U):         %.4e J\n", ep);
            printf(" * Energía mecánica (K + U):      %.4e J\n", energiaMecanica);
            printf(" * Aceleración gravitatoria neta: %.5f m/s²\n", calcAceleracionNeta(r));

            if (etapa < ETAPA_EQUILIBRIO) {
                mostrarMargenEnergetico(energiaMecanica / m, v, rEquilibrio);
                deltaV = pedirCorreccion();
                if (deltaV > 0.0) {
                    // El motor aumenta la velocidad: cambian K y la energía mecánica
                    v = v + deltaV;
                    ec = calcEnergiaCinetica(m, v);
                    energiaMecanica = ec + ep;
                    mision->cantidadManiobras = mision->cantidadManiobras + 1;
                    mision->deltaVTotal = mision->deltaVTotal + deltaV;
                    printf(" [OK] Corrección aplicada. Nueva velocidad: %.2f m/s\n", v);
                    mostrarMargenEnergetico(energiaMecanica / m, v, rEquilibrio);
                }
            } else if (etapa == ETAPA_EQUILIBRIO) {
                printf(" [HITO] Punto de equilibrio alcanzado: desde aquí la Luna atrae a Orion.\n");
            } else {
                printf(" [HITO] Orion pasa a %.1f km de la superficie lunar.\n",
                       (DISTANCIA_LLEGADA_LUNA_M - RADIO_LUNA_M) / 1000.0);
            }

            // Se guardan los datos de la etapa en el arreglo historial
            mision->historial[etapa].numeroEtapa = etapa + 1;
            mision->historial[etapa].alcanzada = 1;
            mision->historial[etapa].distanciaTierra = r;
            mision->historial[etapa].velocidad = v;
            mision->historial[etapa].energiaCinetica = ec;
            mision->historial[etapa].energiaPotencial = ep;
            mision->historial[etapa].deltaV = deltaV;
            etapa = etapa + 1;
        }
    }

    if (enVuelo == 1) {
        mision->misionExitosa = 1;
        printf("\n[ÉXITO] Orion completó el viaje translunar y llegó a la Luna.\n");
    }
    mision->simulacionRealizada = 1;
    printf("Vea el desempeño de la misión en el reporte final (opción 7).\n");
    pausarConsola();
}

void mostrarReporteTelemetria(EstadoMision *mision) {
    printf("\n==========================================================================================\n");
    printf("                        REPORTE FINAL DE TELEMETRÍA TRANSLUNAR\n");
    printf("==========================================================================================\n");

    if (mision->simulacionRealizada == 0) {
        printf(" Todavía no hay una simulación. Ejecute la simulación (opción 6 del menú).\n");
    } else {
        // Δv mínimo posible: una sola corrección en la etapa 1, donde la nave va más rápido
        double rInicial = calcRadioInicial(mision);
        double rEquilibrio = calcPuntoEquilibrioGravitatorio();
        double faltantePorKg = calcTrabajoPorUnidadMasa(rInicial, rEquilibrio)
                             - calcEnergiaCinetica(1.0, mision->velocidadInicial);
        double dvMinimo = calcDeltaVMinimo(mision->velocidadInicial, faltantePorKg);

        if (mision->misionExitosa == 1) {
            printf(" Resultado: MISIÓN EXITOSA\n");
        } else {
            printf(" Resultado: MISIÓN FALLIDA\n");
        }
        printf(" Parámetros: ");
        mostrarParametrosMision(mision);
        printf("------------------------------------------------------------------------------------------\n");
        printf(" Etapa | Dist. Tierra (km) | Velocidad (m/s) | E. Cinética (J) | E. Potencial (J) | Maniobra\n");
        printf("------------------------------------------------------------------------------------------\n");

        for (int i = 0; i < CANT_ETAPAS; i++) {
            printf("   %d   | %15.1f   |", mision->historial[i].numeroEtapa,
                   mision->historial[i].distanciaTierra / 1000.0);
            if (mision->historial[i].alcanzada == 1) {
                printf(" %13.2f   | %13.4e   | %14.4e   | ",
                       mision->historial[i].velocidad,
                       mision->historial[i].energiaCinetica,
                       mision->historial[i].energiaPotencial);
                if (mision->historial[i].deltaV > 0.0) {
                    printf("TCM +%.2f m/s\n", mision->historial[i].deltaV);
                } else {
                    printf("Inercia\n");
                }
            } else {
                printf("                     ----- NO ALCANZADA -----\n");
            }
        }
        printf("------------------------------------------------------------------------------------------\n");

        if (mision->misionExitosa == 0) {
            printf(" Distancia máxima alcanzada: unos %.0f km del centro de la Tierra\n",
                   mision->distanciaMaxima / 1000.0);
        }
        printf(" Correcciones realizadas: %d | Δv total usado: %.2f m/s\n",
               mision->cantidadManiobras, mision->deltaVTotal);
        printf(" Δv mínimo necesario (una sola corrección en la etapa 1): %.2f m/s\n", dvMinimo);
        mostrarCalificacion(mision, dvMinimo);
    }
    printf("==========================================================================================\n");
    pausarConsola();
}
