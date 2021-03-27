#!/bin/bash

# Fichero que automatiza las ejecuciones de los algoritmos

# Greedy

#echo "" > salidas/greedy.txt
#for filename in data/*.txt; do
#	echo $filename >> salidas/greedy.txt
#	bin/greedy < $filename >> salidas/greedy.txt
#done

# Local Search

seeds=($(seq 13 6 187))
i=0

echo "" > salidas/localSearch.txt
for filename in data/*.txt; do
	echo $filename >> salidas/localSearch.txt
	bin/localSearch ${seeds[$i]} < $filename >> salidas/localSearch.txt
	i=$(( $i + 1 ))
done

# Evol
i=0

#echo "" > salidas/localSearchEvol.txt
#for filename in data/*.txt; do
#	echo $filename >> salidas/localSearchEvol.txt
#	bin/localSearchEvol ${seeds[$i]} < $filename >> salidas/localSearchEvol.txt
#	i=$(( $i + 1 ))
#done

# Local Search2 (explora todo el entorno en lugar de primero mejor)
i=0

#echo "" > salidas/localSearch2.txt
#for filename in data/*.txt; do
#	echo $filename >> salidas/localSearch2.txt
#	bin/localSearch2 ${seeds[$i]} < $filename >> salidas/localSearch2.txt
#	i=$(( $i + 1 ))
#done

# Evol
i=0

#echo "" > salidas/localSearch2Evol.txt
#for filename in data/*.txt; do
#	echo $filename >> salidas/localSearch2Evol.txt
#	bin/localSearch2Evol ${seeds[$i]} < $filename >> salidas/localSearch2Evol.txt
#	i=$(( $i + 1 ))
#done