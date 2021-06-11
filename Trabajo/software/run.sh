#!/bin/bash

# Fichero que automatiza las ejecuciones de los algoritmos para generar las gráficas

#for i in {1..30..1}; do
#for s in {42..57..5}; do
#	echo "${i} ${s}"
#	echo "" > graphs_branch/dim30/${i}-${s}.txt
#	./branch ${i} ${s} >> graphs_branch/dim30/${i}-${s}.txt
#done
#done

for i in {1..30..1}; do
for s in {42..57..5}; do
	echo "${i} ${s}"
	echo "" > graphs_hybrid/dim30/${i}-${s}.txt
	./hybrid ${i} ${s} >> graphs_hybrid/dim30/${i}-${s}.txt
done
done
