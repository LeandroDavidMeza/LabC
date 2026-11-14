// validaciones.c
// Definición de funciones dedicadas al ingreso seguro de
// enteros, flotantes y caracteres, y mensajes de advertencia
//
// Idea general: nunca se lee directamente con scanf. Primero se lee la línea
// completa como texto (fgets) y después se revisa ese texto con sscanf.
// Así, si el usuario escribe letras o cualquier cosa rara, el programa no se
// rompe: muestra un error y vuelve a preguntar.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "validaciones.h"
#ifdef _WIN32
#include <windows.h>
#endif

// Función auxiliar que usan todas las demás.
// Lee una línea del teclado y la guarda en "linea" (sin el ENTER del final).
// Devuelve 1 si la línea entró completa, o 0 si el usuario escribió demasiados
// caracteres (en ese caso descarta lo que sobró, para que no se mezcle con la
// próxima pregunta).
int leerLinea(char linea[]) {
    // fgets devuelve NULL si se cerró la entrada (Ctrl+Z en Windows, Ctrl+D en Linux)
    if (fgets(linea, TAM_LINEA, stdin) == NULL) {
        printf("\nSe cerró la entrada de datos. Fin del programa.\n");
        exit(0);
    }

    int largo = strlen(linea);
    if (largo > 0 && linea[largo - 1] == '\n') {
        linea[largo - 1] = '\0'; // se borra el ENTER del final
        return 1;
    }

    // No llegó el ENTER: quedaron caracteres sin leer. Se descartan uno por uno.
    int c = getchar();
    while (c != '\n' && c != EOF) {
        c = getchar();
    }
    return 0;
}

int leerEnteroEnRango(char mensaje[], int min, int max) {
    char linea[TAM_LINEA];
    double numero = 0; // se lee como real para poder detectar decimales y números enormes
    char extra;
    int valido = 0;

    while (valido == 0) {
        printf("%s [%d - %d]: ", mensaje, min, max);
        if (leerLinea(linea) == 0) {
            printf("Error: escribió demasiados caracteres.\n");
        }
        // sscanf intenta leer un número y después un carácter más.
        // Devuelve 1 solo si encontró el número y NADA más (ej: "3").
        // Si hay letras después (ej: "3abc") devuelve 2; si no hay número, 0 o -1.
        else if (sscanf(linea, "%lf %c", &numero, &extra) != 1) {
            printf("Error: debe ingresar un número entero (sin letras ni símbolos).\n");
        }
        // Se pide que esté en el rango y que no tenga decimales: (int)numero le
        // quita los decimales, así que si es igual a numero, no tenía decimales.
        else if (numero >= min && numero <= max && numero == (int)numero) {
            valido = 1;
        } else {
            printf("Error: debe ingresar un número entero entre %d y %d.\n", min, max);
        }
    }
    return (int)numero;
}

double leerRealEnRango(char mensaje[], double min, double max) {
    char linea[TAM_LINEA];
    double numero = 0;
    char extra;
    int valido = 0;

    while (valido == 0) {
        printf("%s [%.1f - %.1f]: ", mensaje, min, max);
        if (leerLinea(linea) == 0) {
            printf("Error: escribió demasiados caracteres.\n");
        } else if (sscanf(linea, "%lf %c", &numero, &extra) != 1) {
            printf("Error: debe ingresar un número (use punto para los decimales, ej: 10900.5).\n");
        } else if (numero >= min && numero <= max) {
            valido = 1;
        } else {
            printf("Error: el valor debe estar entre %.1f y %.1f.\n", min, max);
        }
    }
    return numero;
}

char leerSiNo(char mensaje[]) {
    char linea[TAM_LINEA];
    char letra = ' ';
    char extra;
    int valido = 0;

    while (valido == 0) {
        printf("%s (S/N): ", mensaje);
        // " %c %c": salta espacios, lee una letra e intenta leer otra.
        // Devuelve 1 solo si se escribió UNA sola letra.
        if (leerLinea(linea) == 1 && sscanf(linea, " %c %c", &letra, &extra) == 1) {
            if (letra == 's') letra = 'S'; // se aceptan minúsculas
            if (letra == 'n') letra = 'N';
            if (letra == 'S' || letra == 'N') valido = 1;
        }
        if (valido == 0) {
            printf("Error: responda solamente S o N.\n");
        }
    }
    return letra;
}

void configurarConsola(void) {
#ifdef _WIN32
    // En Windows, hace que la consola muestre bien los acentos y la ñ
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
}

void pausarConsola(void) {
    char linea[TAM_LINEA];
    printf("\nPresione ENTER para continuar...");
    leerLinea(linea); // lo que se escriba antes del ENTER se descarta
}
