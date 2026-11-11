// artemis.c
// Definición de las funciones de cálculo físico de la misión (energía
// cinética, energía potencial, punto de equilibrio, trabajo externo).
// Son funciones puras: reciben números y devuelven números.

#include <stdio.h>
#include <math.h>
#include "artemis.h"

double calcEnergiaCinetica(double m, double v) {
    return 0.5 * m * v * v;
}

double calcEnergiaPotencial(double m, double r) {
    return -CONSTANTE_G * MASA_TIERRA_KG * m / r;
}

// Calcula la energía potencial gravitatoria combinada (Tierra + Luna)
double calcEnergiaPotencialSistema(double m, double rTierra) {
    double distLuna = DISTANCIA_MEDIA_TIERRA_LUNA_M - rTierra;
    double uTierra = -CONSTANTE_G * MASA_TIERRA_KG * m / rTierra;
    double uLuna   = -CONSTANTE_G * MASA_LUNA_KG * m / distLuna;
    return uTierra + uLuna;
}

// Calcula la distancia desde el centro de la Tierra hasta el punto de equilibrio Tierra-Luna
double calcPuntoEquilibrioGravitatorio(void) {
    return DISTANCIA_MEDIA_TIERRA_LUNA_M / (1.0 + sqrt(MASA_LUNA_KG / MASA_TIERRA_KG));
}

// Calcula el trabajo externo mínimo por unidad de masa (W_ext / m) entre dos posiciones
double calcTrabajoPorUnidadMasa(double rInicial, double rFinal) {
    double uEspInicial = calcEnergiaPotencialSistema(1.0, rInicial);
    double uEspFinal   = calcEnergiaPotencialSistema(1.0, rFinal);
    return uEspFinal - uEspInicial;
}

// Despeja la velocidad a partir de la energía cinética: v = sqrt(2*K / m)
double calcVelocidadDesdeEnergia(double energiaCinetica, double m) {
    if (energiaCinetica <= 0.0) return 0.0;
    return sqrt((2.0 * energiaCinetica) / m);
}