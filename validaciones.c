// validaciones.c
// Definición de funciones dedicadas al ingreso seguro de
// enteros, flotantes y caracteres, y mensajes de  advertencia

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "validaciones.h"
#ifdef _WIN32 // Si estamos en Windows, incluimos windows.h para configurar la consola
#include <windows.h>
#endif

// Acá se definen las funciones del módulo

// Función para leer un número entero válido desde la entrada estándar
int leerEnteroValido(const char *mensaje) {
    char input[100];
    int numero;
    char extra;

    while (1) {     //Repite hasta que el usuario ingrese un número entero válido
        printf("%s", mensaje);
        if (fgets(input, sizeof input, stdin) == NULL)
            exit(1);        //Lee la línea completa y sale del programa en caso de error en la lectura
        if (sscanf(input, "%d %c", &numero, &extra) == 1)
            return numero;  //Si la entrada es un número entero válido, lo devuelve
        printf("Error: debe ingresar un número entero válido.\n");
    }
}

// Configura la consola para mostrar correctamente tildes y caracteres especiales
void configurarConsola(void) {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);  // salida en UTF-8
    SetConsoleCP(CP_UTF8);        // entrada en UTF-8
#endif
}

// Lee un número real válido (acepta decimales, rechaza texto y nulo)
double leerDoubleValido(const char *mensaje) {
    char input[100];   // Guarda la línea completa que escribe el usuario
    double numero;     // Acá se guarda el número leído
    char extra;        // Detector de basura: si se llega a leer, había texto de más

    while (1) { // Repite hasta que el usuario ingrese un número real válido
        printf("%s", mensaje);
        if (fgets(input, sizeof input, stdin) == NULL)
            exit(1); // Si se cerró la entrada (Ctrl+D, fin de archivo), termina el programa
        // Válido solo si leyó exactamente 1 dato (el número, sin texto de más)
        // y además no es nan ni infinito
        if (sscanf(input, "%lf %c", &numero, &extra) == 1 && isfinite(numero))
            return numero; // Entrada válida: devuelve el número
        printf("Error: debe ingresar un número válido.\n");
    }
}

// Lee un número real y exige que esté entre min y max (inclusive)
double leerDoubleEnRango(const char *mensaje, double min, double max) {
    double n;

    while (1) { // Repite hasta que el número esté dentro del rango
        n = leerDoubleValido(mensaje); // Reutiliza la validación de formato
        if (n >= min && n <= max)
            return n; // Está dentro del rango: lo devuelve
        printf("Error: el valor debe estar entre %.2f y %.2f.\n", min, max);
    }
}