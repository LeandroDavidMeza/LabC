// artemis.h
// Declaración de constantes físicas y de misión (#define), límites de los
// datos de entrada, tipos de datos estructurados (struct, typedef) y
// prototipos de las funciones de cálculo físico.

#ifndef ARTEMIS_H
#define ARTEMIS_H

// Constantes

#define MASA_NAVE_ORION_KG 26000.00 //kg
#define MASA_TIERRA_KG 5.972e24 //kg
#define RADIO_TIERRA_M 6371000.0 //m

#define VELOCIDAD_ESCAPE_INYECCION_KMS 10.9 //km/s
#define VELOCIDAD_ESCAPE_INYECCION_MS 10900.00 //m/s

#define DISTANCIA_MEDIA_TIERRA_LUNA_KM 384000.00 //km
#define DISTANCIA_MEDIA_TIERRA_LUNA_M 384000000.00 //m

#define CONSTANTE_G 6.67430e-11 // Constante de gravitación universal en N*m²/kg² o m³/(kg*s²) 

// Límites aceptados para los datos ingresados por el usuario
#define MASA_MIN_KG 1.0
#define MASA_MAX_KG 1000000.0
#define VELOCIDAD_MIN_MS 0.0
#define VELOCIDAD_MAX_MS 50000.0

// Funciones físicas
double calcEnergiaCinetica(double m, double v);
double calcEnergiaPotencial(double m, double r);

#endif