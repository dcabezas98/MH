# Ejecutables:

**Greedy:** Implementación del algoritmo Greedy.
**LocalSearch:** Implementación del algoritmo de Búsqueda Local (Primer Mejor).
**LocalSearch2:** Implementación del algoritmo de Búsqueda Local (Mejor).
**LocalSearch2nolim:** Implementación del algoritmo de Búsqueda Local (Mejor) sin límite de evaluaciones.

Todos ellos devuelven la evaluación de la solución obtenida y el tiempo de ejecución por salida estándar.
Leen el fichero por entrada estándar, así que es conveniente redirigirla.

Ejemplo:
```
bin/greedy < datos/MDG-a_1_n500_m50.txt >> salidas/greedy.txt
```

Los programas correspondientes a la búsqueda local también devuelven el número de llamadas a la función de evaluación, para
comparar cómo de cerca se quedan del límite.

Los ejecutables **LocalSearchEvol** y **LocalSearch2Evol** son versiones
equivalentes a **LocalSearch** y **LocalSearch2**, pero con la diferencia de que devuelven una lista con la fitness de la solución
en cada iteración. Los usamos para estudiar la evolución de la solución a lo largo del proceso y obtener gráficas de convergencia.

Todos los archivos de búsqueda local reciben la semilla como parámetro.

Ejemplo:
```
bin/localSearch 197 < datos/MDG-a_1_n500_m50.txt >> salidas/localSearch.txt
```

En la carpeta `software` se incluye el script usado para lanzar todas las ejecuciones, `run.sh`. También se incluye
el `Makefile` que compila los ejecutables.