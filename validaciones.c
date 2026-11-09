// validaciones.c
// Definición de funciones dedicadas al ingreso seguro de
// enteros, flotantes y caracteres, y mensajes de  advertencia

#include <stdio.h>
#include <stdlib.h>
#include "validaciones.h"

// Acá se definen las funciones del módulo

// Función para leer un número entero válido desde la entrada estándar
int leerEnteroValido(const char *mensaje) {
    char input[100];
    int numero;
    char extra;

    while (1) { //Repite hasta que el usuario ingrese un número entero válido
        printf("%s", mensaje);
        if (fgets(input, sizeof input, stdin) == NULL)
            exit(1); //Lee la línea completa y sale del programa en caso de error en la lectura
        if (sscanf(input, "%d %c", &numero, &extra) == 1)
            return numero; //Si la entrada es un número entero válido, lo devuelve
        printf("Error: debe ingresar un número entero válido.\n");
    }
}
