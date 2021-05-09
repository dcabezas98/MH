# Ejecutables:

**AGG-uniforme:** Implementación del algoritmo genético generacional con cruce uniforme. 
**AGG-posicion:** Implementación del algoritmo genético generacional con cruce basado en posición.
**AGE-uniforme:** Implementación del algoritmo genético estacionario con cruce uniforme. 
**AGE-posicion:** Implementación del algoritmo genético estacionario con cruce basado en posición.
**AM-10-1:** Implementación del algoritmo memético (genético generacional con cruce uniforme combinado con búsqueda local) que aplica búsqueda local a todos los cromosomas cada 10 iteraciones.
**AM-10-01:** Implementación del algoritmo memético que aplica búsqueda local a cada cromosoma con probabilidad 0.1 cada 10 iteraciones.
**AM-10-01mej:** Implementación del algoritmo memético que aplica búsqueda local a los "0.1 \* número de cromosomas" mejores cromosomas cada 10 iteraciones.

Todos ellos devuelven la evaluación de la solución obtenida y el tiempo de ejecución por salida estándar.
Leen el fichero por entrada estándar, así que es conveniente redirigirla.
Todos los archivos reciben la semilla como parámetro.

Ejemplo:
```
bin/AGG-uniforme 197 < datos/MDG-a_1_n500_m50.txt >> salidas/AGG-uniforme.txt
```

En la carpeta `software` se incluye el script usado para lanzar todas las ejecuciones, `run.sh`. También se incluye
el `Makefile` que compila los ejecutables.