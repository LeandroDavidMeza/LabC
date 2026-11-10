# Variables de compilación
CC = gcc # Compilador
CFLAGS = -Wall -Wextra # Flags de compilación (habilitar advertencias) 
LDFLAGS = -lm # Flags de enlace (enlazar con la biblioteca matemática)
OBJS = main.o simulador.o artemis.o validaciones.o # Objetos a compilar
HEADERS = artemis.h simulador.h validaciones.h # Archivos de cabecera


# Detecta el sistema operativo
ifeq ($(OS),Windows_NT)
    TARGET = artemis.exe # Ejecutable para Windows
else
    TARGET = artemis # Ejecutable para otros sistemas
endif

# Regla por defecto
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Regla para compilar archivos .c a .o
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@


# Regla para limpiar archivos generados
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: clean # Indica que "clean" no es un archivo real, sino un objetivo de limpieza