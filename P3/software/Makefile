# Definición de macros para definir las carpetas de trabajo
BIN=./bin
SRC=./fuentes

# Opciones de compilación
# -Wall muestra todas las advertencias
# -O2 para optimizacion
OPT=-Wall -O2

# Nombre de la práctica
PRJ=practica3

all: $(BIN)/ES $(BIN)/ES-proporcional $(BIN)/BMB $(BIN)/ILS $(BIN)/ILS-ES $(BIN)/ILS-ES-proporcional

# Compilamos
$(BIN)/ES : $(SRC)/ES.cpp
	g++ $(OPT) $^ -o $@
	
$(BIN)/ES-proporcional : $(SRC)/ES-proporcional.cpp
	g++ $(OPT) $^ -o $@
	
$(BIN)/BMB : $(SRC)/BMB.cpp
	g++ $(OPT) $^ -o $@
	
$(BIN)/ILS : $(SRC)/ILS.cpp
	g++ $(OPT) $^ -o $@
	
$(BIN)/ILS-ES : $(SRC)/ILS-ES.cpp
	g++ $(OPT) $^ -o $@
	
$(BIN)/ILS-ES-proporcional : $(SRC)/ILS-ES-proporcional.cpp
	g++ $(OPT) $^ -o $@

clean:
	find bin/* ! -name LEEME.md -delete
