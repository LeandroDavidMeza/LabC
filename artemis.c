// artemis.c
// Definición de las funciones de cálculo físico de la misión (energía
// cinética, energía potencial, etc.). Son funciones puras: reciben
// números y devuelven números, sin leer ni imprimir nada.

#include <stdio.h>
#include "artemis.h"

// Acá se definen las funciones

// Función para calcular energia cinética
// Recibe masa en kg y velocidad en m/s
// Devuelve energía en Joules
double calcEnergiaCinetica(double m, double v){
    double Ec = 0.5 * m * v * v;
    return Ec;
}

// Función para calcular la energía potencial gravitatoria
// Recibe masa de la nave en kg y distancia al CENTRO de la Tierra en m
// Devuelve energía en Joules (siempre negativa)
double calcEnergiaPotencial(double m, double r){
    //Fórmula para la energía potencial gravitatoria entre la Tierra
    // y un objeto de masa m a una distancia r del centro de la Tierra
    double U = -CONSTANTE_G * MASA_TIERRA_KG * m / r;
    return U;
}