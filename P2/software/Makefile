# Definición de macros para definir las carpetas de trabajo
BIN=./bin
SRC=./fuentes

# Opciones de compilación
# -Wall muestra todas las advertencias
# -O2 para optimizacion
OPT=-Wall -O2

# Nombre de la práctica
PRJ=practica1

all: $(BIN)/AGG-uniforme $(BIN)/AGG-posicion $(BIN)/AGE-uniforme $(BIN)/AGE-posicion

# Compilamos
$(BIN)/AGG-uniforme : $(SRC)/AGG-uniforme.cpp
	g++ $(OPT) $^ -o $@

$(BIN)/AGG-posicion : $(SRC)/AGG-posicion.cpp
	g++ $(OPT) $^ -o $@

$(BIN)/AGE-uniforme : $(SRC)/AGE-uniforme.cpp
	g++ $(OPT) $^ -o $@

$(BIN)/AGE-posicion : $(SRC)/AGE-posicion.cpp
	g++ $(OPT) $^ -o $@

clean:
	rm -f $(BIN)/*