// validaciones.c
// Definición de funciones dedicadas al ingreso seguro de
// enteros, flotantes y caracteres, y mensajes de advertencia

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "validaciones.h"
#ifdef _WIN32
#include <windows.h>
#endif

int leerEnteroValido(const char *mensaje) {
    char input[100];
    int numero;
    char extra;

    while (1) {
        printf("%s", mensaje);
        if (fgets(input, sizeof input, stdin) == NULL)
            exit(1);
        if (sscanf(input, "%d %c", &numero, &extra) == 1)
            return numero;
        printf("Error: debe ingresar un número entero válido.\n");
    }
}

int leerEnteroEnRango(const char *mensaje, int min, int max) {
    int n;
    while (1) {
        n = leerEnteroValido(mensaje);
        if (n >= min && n <= max)
            return n;
        printf("Error: el valor debe estar entre %d y %d.\n", min, max);
    }
}

void configurarConsola(void) {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
}

double leerDoubleValido(const char *mensaje) {
    char input[100];
    double numero;
    char extra;

    while (1) {
        printf("%s", mensaje);
        if (fgets(input, sizeof input, stdin) == NULL)
            exit(1);
        if (sscanf(input, "%lf %c", &numero, &extra) == 1 && isfinite(numero))
            return numero;
        printf("Error: debe ingresar un número válido.\n");
    }
}

double leerDoubleEnRango(const char *mensaje, double min, double max) {
    double n;

    while (1) {
        n = leerDoubleValido(mensaje);
        if (n >= min && n <= max)
            return n;
        printf("Error: el valor debe estar entre %.2f y %.2f.\n", min, max);
    }
}

void pausarConsola(void) {
    char buffer[16];
    printf("\nPresione ENTER para continuar...");
    fgets(buffer, sizeof buffer, stdin);
}