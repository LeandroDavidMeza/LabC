// artemis.h
// Declaración de constantes físicas y de misión (#define), límites de los
// datos de entrada y prototipos de las funciones de cálculo físico.

#ifndef ARTEMIS_H
#define ARTEMIS_H

// ---------------- Datos del Escenario 3 (consigna) ----------------
#define MASA_NAVE_ORION_KG 26000.0            // kg
#define VELOCIDAD_TLI_MS 10900.0              // m/s (la consigna la llama "velocidad de escape/inyección")
#define ALTITUD_LEO_M 192000.0                // m (192 km)
#define DISTANCIA_MEDIA_TIERRA_LUNA_M 384000000.0 // m (384.000 km)

// ---------------- Constantes físicas ----------------
// G, masa y radio de la Tierra: valores que da la consigna (Escenario 2)
#define CONSTANTE_G 6.674e-11                 // N*m²/kg²
#define MASA_TIERRA_KG 5.972e24               // kg
#define RADIO_TIERRA_M 6371000.0              // m
// Masa y radio de la Luna: valores de referencia (no figuran en la consigna)
#define MASA_LUNA_KG 7.342e22                 // kg
#define RADIO_LUNA_M 1737400.0                // m

// ---------------- Límites de los datos que ingresa el usuario ----------------
#define MASA_MIN_KG 1000.0
#define MASA_MAX_KG 1000000.0
#define VELOCIDAD_MIN_MS 1000.0
#define VELOCIDAD_MAX_MS 50000.0
#define ALTITUD_MIN_KM 100.0
#define ALTITUD_MAX_KM 2000.0

// Paso con el que se busca hasta dónde llega la nave cuando no le alcanza la energía
#define PASO_BUSQUEDA_M 100000.0              // m (100 km)

// ---------------- Funciones físicas ----------------
// Son funciones "puras": reciben números y devuelven números (no usan printf ni scanf).
// r es siempre la distancia al CENTRO de la Tierra, en metros.
// Truco: si se usa m = 1.0, el resultado es la energía "por kilo" de nave.
double calcEnergiaCinetica(double m, double v);
double calcEnergiaPotencial(double m, double r);
double calcEnergiaPotencialSistema(double m, double r);
double calcAceleracionNeta(double r);
double calcVelocidadEscape(double r);
double calcVelocidadOrbital(double r);
double calcPuntoEquilibrioGravitatorio(void);
double calcTrabajoPorUnidadMasa(double rInicial, double rFinal);
double calcVelocidadDesdeEnergia(double energiaCinetica, double m);
double calcDeltaVMinimo(double v, double energiaFaltantePorKg);
double calcDistanciaMaxima(double energiaMecanicaPorKg, double rDesde);

#endif
