// validaciones.h
// Declaración de funciones dedicadas al ingreso seguro de
// enteros, flotantes y caracteres, y mensajes de advertencia

#ifndef VALIDACIONES_H
#define VALIDACIONES_H

#define TAM_LINEA 100  // cantidad máxima de caracteres que se leen por respuesta

// Pide un número entero hasta que el usuario ingrese uno válido entre min y max
int leerEnteroEnRango(char mensaje[], int min, int max);

// Pide un número real (con decimales) hasta que esté entre min y max
double leerRealEnRango(char mensaje[], double min, double max);

// Pide una respuesta S o N (acepta también s o n) y devuelve 'S' o 'N'
char leerSiNo(char mensaje[]);

void configurarConsola(void);
void pausarConsola(void);

#endif
