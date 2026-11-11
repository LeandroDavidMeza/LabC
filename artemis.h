// artemis.h
// Declaración de constantes físicas y de misión (#define), límites de los
// datos de entrada y prototipos de las funciones de cálculo físico.

#ifndef ARTEMIS_H
#define ARTEMIS_H

// Constantes oficiales del Escenario 3
#define MASA_NAVE_ORION_KG 26000.00           // kg
#define MASA_TIERRA_KG 5.972e24               // kg
#define MASA_LUNA_KG 7.342e22                 // kg
#define RADIO_TIERRA_M 6371000.0              // m
#define RADIO_LUNA_M 1737400.0                // m
#define ALTITUD_LEO_M 192000.0                // m (192 km)

#define VELOCIDAD_ESCAPE_INYECCION_KMS 10.9   // km/s
#define VELOCIDAD_ESCAPE_INYECCION_MS 10900.00 // m/s

#define DISTANCIA_MEDIA_TIERRA_LUNA_KM 384000.00 // km
#define DISTANCIA_MEDIA_TIERRA_LUNA_M 384000000.00 // m

#define CONSTANTE_G 6.67430e-11               // N*m²/kg²

// Límites aceptados para los datos ingresados por el usuario
#define MASA_MIN_KG 1000.0
#define MASA_MAX_KG 1000000.0
#define VELOCIDAD_MIN_MS 1000.0
#define VELOCIDAD_MAX_MS 50000.0
#define ALTITUD_MIN_M 100000.0
#define ALTITUD_MAX_M 2000000.0

// Funciones físicas puras
double calcEnergiaCinetica(double m, double v);
double calcEnergiaPotencial(double m, double r);
double calcEnergiaPotencialSistema(double m, double rTierra);
double calcPuntoEquilibrioGravitatorio(void);
double calcTrabajoPorUnidadMasa(double rInicial, double rFinal);
double calcVelocidadDesdeEnergia(double energiaCinetica, double m);

#endif
