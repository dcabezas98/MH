#!/bin/bash

# Fichero que automatiza las ejecuciones de los algoritmos

# AGG uniforme

seeds=($(seq 13 6 187))
i=0

echo "" > salidas/AGG-uniforme.txt
for filename in data/*.txt; do
	echo $filename >> salidas/AGG-uniforme.txt#
	bin/AGG-uniforme ${seeds[$i]} < $filename >> salidas/AGG-uniforme.txt
	i=$(( $i + 1 ))
done

# AGG posición

seeds=($(seq 13 6 187))
i=0

echo "" > salidas/AGG-posicion.txt
for filename in data/*.txt; do
	echo $filename >> salidas/AGG-posicion.txt
	bin/AGG-posicion ${seeds[$i]} < $filename >> salidas/AGG-posicion.txt
	i=$(( $i + 1 ))
done

# AGE uniforme

seeds=($(seq 13 6 187))
i=0

echo "" > salidas/AGE-uniforme.txt
for filename in data/*.txt; do
	echo $filename >> salidas/AGE-uniforme.txt
	bin/AGE-uniforme ${seeds[$i]} < $filename >> salidas/AGE-uniforme.txt
	i=$(( $i + 1 ))
done

# AGE posición

seeds=($(seq 13 6 187))
i=0

echo "" > salidas/AGE-posicion.txt
for filename in data/*.txt; do
	echo $filename >> salidas/AGE-posicion.txt
	bin/AGE-posicion ${seeds[$i]} < $filename >> salidas/AGE-posicion.txt
	i=$(( $i + 1 ))
done

# AM 10, 1.0

seeds=($(seq 13 6 187))
i=0

echo "" > salidas/AM-10-1.txt
for filename in data/*.txt; do
	echo $filename >> salidas/AM-10-1.txt
	bin/AM-10-1 ${seeds[$i]} < $filename >> salidas/AM-10-1.txt
	i=$(( $i + 1 ))
done

# AM 10, 0.1

seeds=($(seq 13 6 187))
i=0

echo "" > salidas/AM-10-01.txt
for filename in data/*.txt; do
	echo $filename >> salidas/AM-10-01.txt
	bin/AM-10-01 ${seeds[$i]} < $filename >> salidas/AM-10-01.txt
	i=$(( $i + 1 ))
done

# AM 10, 0.1mej

seeds=($(seq 13 6 187))
i=0

echo "" > salidas/AM-10-01mej.txt
for filename in data/*.txt; do
	echo $filename >> salidas/AM-10-01mej.txt
	bin/AM-10-01mej ${seeds[$i]} < $filename >> salidas/AM-10-01mej.txt
	i=$(( $i + 1 ))
done
