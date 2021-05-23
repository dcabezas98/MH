#!/bin/bash

# Fichero que automatiza las ejecuciones de los algoritmos

# ES
seeds=($(seq 13 6 187))
i=0

echo "" > salidas/ES.txt
for filename in data/*.txt; do
	echo $filename >> salidas/ES.txt
	bin/ES ${seeds[$i]} < $filename >> salidas/ES.txt
	i=$(( $i + 1 ))
done

# ES-proporcional
seeds=($(seq 13 6 187))
i=0

echo "" > salidas/ES-proporcional.txt
for filename in data/*.txt; do
	echo $filename >> salidas/ES-proporcional.txt
	bin/ES-proporcional ${seeds[$i]} < $filename >> salidas/ES-proporcional.txt
	i=$(( $i + 1 ))
done

# BMB
seeds=($(seq 13 6 187))
i=0

echo "" > salidas/BMB.txt
for filename in data/*.txt; do
	echo $filename >> salidas/BMB.txt
	bin/BMB ${seeds[$i]} < $filename >> salidas/BMB.txt
	i=$(( $i + 1 ))
done

# ILS
seeds=($(seq 13 6 187))
i=0

echo "" > salidas/ILS.txt
for filename in data/*.txt; do
	echo $filename >> salidas/ILS.txt
	bin/ILS ${seeds[$i]} < $filename >> salidas/ILS.txt
	i=$(( $i + 1 ))
done

# ILS-ES
seeds=($(seq 13 6 187))
i=0

echo "" > salidas/ILS-ES.txt
for filename in data/*.txt; do
	echo $filename >> salidas/ILS-ES.txt
	bin/ILS-ES ${seeds[$i]} < $filename >> salidas/ILS-ES.txt
	i=$(( $i + 1 ))
done

# ILS-ES-proporcional
seeds=($(seq 13 6 187))
i=0

echo "" > salidas/ILS-ES-proporcional.txt
for filename in data/*.txt; do
	echo $filename >> salidas/ILS-ES-proporcional.txt
	bin/ILS-ES-proporcional ${seeds[$i]} < $filename >> salidas/ILS-ES-proporcional.txt
	i=$(( $i + 1 ))
done
