# Definición de macros para definir las carpetas de trabajo
BIN=./bin
SRC=./fuentes

# Opciones de compilación
# -Wall muestra todas las advertencias
# -O2 para optimizacion
OPT=-Wall -O2

# Nombre de la práctica
PRJ=practica1

all: $(BIN)/greedy $(BIN)/localSearch $(BIN)/localSearch2 $(BIN)/localSearch2nolim $(BIN)/localSearchEvol $(BIN)/localSearch2Evol

# Compilamos
$(BIN)/greedy : $(SRC)/greedy.cpp
	g++ $(OPT) $^ -o $@

$(BIN)/localSearch : $(SRC)/localSearch.cpp
	g++ $(OPT) $^ -o $@

$(BIN)/localSearch2 : $(SRC)/localSearch2.cpp
	g++ $(OPT) $^ -o $@

$(BIN)/localSearch2nolim : $(SRC)/localSearch2nolim.cpp
	g++ $(OPT) $^ -o $@

$(BIN)/localSearchEvol : $(SRC)/localSearchEvol.cpp
	g++ $(OPT) $^ -o $@	

$(BIN)/localSearch2Evol : $(SRC)/localSearch2Evol.cpp
	g++ $(OPT) $^ -o $@	

clean:
	rm -f $(BIN)/*