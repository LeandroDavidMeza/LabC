// artemis.c
// Definición de las funciones de cálculo físico de la misión (energía
// cinética, energía potencial, punto de equilibrio, trabajo externo).
// Son funciones puras: reciben números y devuelven números.

#include <math.h>
#include "artemis.h"

// K = 1/2 * m * v²
double calcEnergiaCinetica(double m, double v) {
    return 0.5 * m * v * v;
}

// Energía potencial por la atracción de la Tierra: U = -G * M_T * m / r
double calcEnergiaPotencial(double m, double r) {
    return -CONSTANTE_G * MASA_TIERRA_KG * m / r;
}

// Energía potencial por la atracción de la Tierra + la Luna.
// La nave está a r del centro de la Tierra, así que está a (D - r) de la Luna.
double calcEnergiaPotencialSistema(double m, double r) {
    double distanciaLuna = DISTANCIA_MEDIA_TIERRA_LUNA_M - r;
    double uTierra = calcEnergiaPotencial(m, r);
    double uLuna = -CONSTANTE_G * MASA_LUNA_KG * m / distanciaLuna;
    return uTierra + uLuna;
}

// Aceleración gravitatoria neta sobre la nave: a = G*M_L/(D - r)² - G*M_T/r²
// Negativa = apunta hacia la Tierra (frena a la nave). Positiva = hacia la Luna.
double calcAceleracionNeta(double r) {
    double distanciaLuna = DISTANCIA_MEDIA_TIERRA_LUNA_M - r;
    double aTierra = CONSTANTE_G * MASA_TIERRA_KG / (r * r);
    double aLuna = CONSTANTE_G * MASA_LUNA_KG / (distanciaLuna * distanciaLuna);
    double aNeta = aLuna - aTierra;

    // En el punto de equilibrio da casi cero (ej: -0.0000000001 por redondeo).
    // Se deja en 0 para que no se muestre como "-0.00000".
    if (aNeta > -0.000001 && aNeta < 0.000001) {
        aNeta = 0.0;
    }
    return aNeta;
}

// Velocidad de escape: v_esc = raíz(2 * G * M_T / r)
double calcVelocidadEscape(double r) {
    return sqrt(2.0 * CONSTANTE_G * MASA_TIERRA_KG / r);
}

// Velocidad de una órbita circular: v_orb = raíz(G * M_T / r)
double calcVelocidadOrbital(double r) {
    return sqrt(CONSTANTE_G * MASA_TIERRA_KG / r);
}

// Punto donde la atracción de la Tierra y la de la Luna son iguales:
// G*M_T/r² = G*M_L/(D - r)²  =>  r = D / (1 + raíz(M_L / M_T))
double calcPuntoEquilibrioGravitatorio(void) {
    return DISTANCIA_MEDIA_TIERRA_LUNA_M / (1.0 + sqrt(MASA_LUNA_KG / MASA_TIERRA_KG));
}

// Trabajo por kilo para llevar la nave desde rInicial hasta rFinal, partiendo
// quieta y llegando quieta: W/m = U(rFinal)/m - U(rInicial)/m
double calcTrabajoPorUnidadMasa(double rInicial, double rFinal) {
    double uPorKgInicial = calcEnergiaPotencialSistema(1.0, rInicial);
    double uPorKgFinal = calcEnergiaPotencialSistema(1.0, rFinal);
    return uPorKgFinal - uPorKgInicial;
}

// Despeja la velocidad de K = 1/2 * m * v²  =>  v = raíz(2 * K / m)
double calcVelocidadDesdeEnergia(double energiaCinetica, double m) {
    if (energiaCinetica <= 0.0) {
        return 0.0;
    }
    return sqrt(2.0 * energiaCinetica / m);
}

// Cuánto hay que aumentar la velocidad (Δv) de una nave que va a velocidad v
// para que gane "energiaFaltantePorKg" de energía cinética por kilo:
// 1/2*(v + Δv)² - 1/2*v² = faltante  =>  Δv = raíz(v² + 2*faltante) - v
double calcDeltaVMinimo(double v, double energiaFaltantePorKg) {
    if (energiaFaltantePorKg <= 0.0) {
        return 0.0; // no falta energía: no hace falta corregir
    }
    return sqrt(v * v + 2.0 * energiaFaltantePorKg) - v;
}

// Busca hasta dónde llega la nave cuando no le alcanza la energía.
// Empieza en rDesde y avanza de a 100 km mientras a la nave todavía le quede
// energía cinética (K = Em - U mayor que cero) en el punto siguiente.
double calcDistanciaMaxima(double energiaMecanicaPorKg, double rDesde) {
    double r = rDesde;
    double ecSiguiente = energiaMecanicaPorKg - calcEnergiaPotencialSistema(1.0, r + PASO_BUSQUEDA_M);
    while (ecSiguiente > 0.0) {
        r = r + PASO_BUSQUEDA_M;
        ecSiguiente = energiaMecanicaPorKg - calcEnergiaPotencialSistema(1.0, r + PASO_BUSQUEDA_M);
    }
    return r;
}
