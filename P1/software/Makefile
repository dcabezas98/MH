# Definición de macros para definir las carpetas de trabajo
BIN=./bin
SRC=./fuentes

# Opciones de compilación
# -Wall muestra todas las advertencias
# -O2 para optimizacion
OPT=-Wall -O2

# Nombre de la práctica
PRJ=practica1

all: $(BIN)/greedy $(BIN)/localSearch

# Compilamos
$(BIN)/greedy : $(SRC)/greedy.cpp
		g++ $(OPT) $^ -o $@

$(BIN)/localSearch : $(SRC)/localSearch.cpp
	g++ $(OPT) $^ -o $@

clean:
	rm -f $(BIN)/*