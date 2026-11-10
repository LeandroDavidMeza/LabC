# Laboratorio de C
Este es el repositorio del proyecto final del laboratorio de C para la materia Algortimos y Estructuras de Datos, en la carrera de Ingeniería en Sistemas de Información de la UTN FRRe.

## Integrantes
* Alvarez, Daniel Agustin
* Brailovsky Chas, Ramiro Iván
* Kohler, Sofía Pilar
* Meza, Leandro David
* Sanz, Victoria

*Año: 2026*
*Comisión k1.2*

## Compilación y ejecución

### ¿Qué hace el Makefile?

El Makefile automatiza la compilación del proyecto. En lugar de escribir el comando completo de GCC con todos los archivos, se ejecuta un solo comando. Además:

- Compila cada módulo (main.c, simulador.c, artemis.c, validaciones.c) a un archivo objeto (.o) por separado.
- Une los archivos objeto en el ejecutable final.
- Recompila solo los módulos que cambiaron desde la última vez, y todos los que dependen de un .h modificado.
- Incluye el comando clean, que borra los archivos generados.
- Activa las advertencias del compilador (-Wall -Wextra).

### Requisitos e instalación

Se necesita el compilador **GCC** y la herramienta **make**.

| Sistema | Instalación de Make |
|---|---|
| Linux (Debian/Ubuntu) | sudo apt install build-essential |
| macOS | xcode-select --install |
| Windows con MSYS2 (UCRT64) | pacman -S mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-make |

### Comandos de uso

| Acción | Linux / macOS | Windows (MSYS2) |
|---|---|---|
| Compilar | make | mingw32-make |
| Ejecutar | ./artemis | ./artemis.exe (en bash) o .\artemis.exe (en PowerShell) |
| Limpiar | make clean | mingw32-make clean |

Los comandos se ejecutan desde la carpeta raíz del repositorio, donde está el Makefile. Compilar genera el ejecutable artemis (artemis.exe en Windows).

### Aclaraciones

- El proyecto fue probado en **Windows con MSYS2 (UCRT64)**.
- En Windows con MSYS2, el comando de make se llama mingw32-make.
- clean usa el comando rm, por lo que funciona en terminales tipo bash (Linux, macOS, MSYS2, Git Bash, WSL). En cmd o PowerShell hay que borrar los archivos .o y el ejecutable a mano.

### Compilación manual (sin Makefile)

```
gcc -Wall -Wextra main.c simulador.c artemis.c validaciones.c -o artemis
```