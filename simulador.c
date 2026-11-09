// simulador.c
// Definición de los algoritmos de simulación, actualización del 
// estado del juego, cálculos físicos y lógica de negocio.

#include <stdio.h>
#include "simulador.h"

// Acá se definen las funciones

// Función para calcula energia cínetica
// Recibe masa en kg y velocidad en m/s
// Devuelve energía en Joules
float calcEnergiaCinetica(float m, float v){
    float Ec = 0.5f * m * v * v;
    return Ec;
}
