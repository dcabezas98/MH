# Definición de macros para definir las carpetas de trabajo
BIN=./bin
SRC=./fuentes

# Opciones de compilación
# -Wall muestra todas las advertencias
# -O2 para optimizacion
OPT=-Wall -O2

# Nombre de la práctica
PRJ=practica1

all: $(BIN)/AGG-uniforme

# Compilamos
$(BIN)/AGG-uniforme : $(SRC)/AGG-uniforme.cpp
	g++ $(OPT) $^ -o $@

clean:
	rm -f $(BIN)/*