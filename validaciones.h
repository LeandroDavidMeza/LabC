// validaciones.h
// Declaración de funciones dedicadas al ingreso seguro de
// enteros, flotantes y caracteres, y mensajes de advertencia

#ifndef VALIDACIONES_H
#define VALIDACIONES_H

int leerEnteroValido(const char *mensaje);
int leerEnteroEnRango(const char *mensaje, int min, int max);
void configurarConsola(void);
double leerDoubleValido(const char *mensaje);
double leerDoubleEnRango(const char *mensaje, double min, double max);
void pausarConsola(void);

#endif