// validaciones.h
// Declaración de funciones dedicadas al ingreso seguro de
// enteros, flotantes y caracteres, y mensajes de  advertencia

#ifndef VALIDACIONES_H
#define VALIDACIONES_H

// Acá las declaraciones de funciones del módulo validaciones

int leerEnteroValido(const char *mensaje);
void configurarConsola(void);
double leerDoubleValido(const char *mensaje);
double leerDoubleEnRango(const char *mensaje, double min, double max);

#endif