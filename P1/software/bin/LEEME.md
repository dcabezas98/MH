# Ejecutables:

**Greedy:** Implementación del algoritmo Greedy
**LocalSearch:** Implementación del algoritmo de Búsqueda Local (Primer Mejor)
**LocalSearch2:** Implementación del algoritmo de Búsqueda Local (Mejor)

Todos devuelven la evaluación de la solución obtenida y el tiempo de 
ejecución por la salida estándar. Leen el fichero por entrada estándar, así
que es conveniente redirigirla.

Ejemplo:
```
bin/greedy < datos/MDG-a_1_n500_m50.txt >> salidas/greedy.txt
```

Los ejecutables **LocalSearchEvol** y **LocalSearch2Evol** son versiones
equivalentes, pero devuelven una lista con la fitness en cada iteración para
estudiar la evolución de la solución y obtener gráficas de convergencia.

Todos los archivos de búsqueda local reciben la semilla como parámetro.

Ejemplo:
```
bin/localSearch 197 < datos/MDG-a_1_n500_m50.txt >> salidas/localSearch.txt
```