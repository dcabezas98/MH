# Ejecutables:

**ES:** Enfriamiento Simulado con Cauchy Modificado.
**ES-proporcional:** Enfriamiento Simulado con enfriamiento proporcional.
**BMB:** Búsqueda Multiarranque Básica.
**ILS:** Búsqueda Local Reiterada.
**ILS-ES:** Hibridación de ILS y Enfriamiento Simulado con Cauchy Modificado.
**ILS-ES-proporcional:** Hibridación de ILS y Enfriamiento Simulado con enfriamiento proporcional. 

Todos ellos devuelven la evaluación de la solución obtenida y el tiempo de ejecución por salida estándar.
Leen el fichero por entrada estándar, así que es conveniente redirigirla.
Todos los archivos reciben la semilla como parámetro.

Ejemplo:
```
bin/ES 197 < datos/MDG-a_1_n500_m50.txt >> salidas/ES.txt
```

En la carpeta `software` se incluye el script usado para lanzar todas las ejecuciones, `run.sh`. También se incluye
el `Makefile` que compila los ejecutables.
