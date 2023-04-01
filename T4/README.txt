==========================================================
Esta es la documentación para compilar y ejecutar su tarea
==========================================================

Se está ejecutando el comando: less README.txt

***************************
*** Para salir: tecla q ***
***************************

Para avanzar a una nueva página: tecla <page down>
Para retroceder a la página anterior: tecla <page up>
Para avanzar una sola línea: tecla <enter>
Para buscar un texto: tecla / seguido del texto (/...texto...)
         por ejemplo: /ddd

-----------------------------------------------

Ud. debe crear el archivo extraer.c y programar ahí la función:

  int main(int argc, char *argv[]) { ... }

No hay una plantilla para extraer.c.

Determine Ud. los include que se necesitan.  Si reclama por ejemplo por
el encabezado de la funcion exit, ingrese en un terminal de Debian el comando:

  man 3 exit

Le dirá que include se necesita.

Debe probar su tarea bajo Debian 11 de 64 bits nativo o virtualizado.
Queda excluido Windows WSL 1 para hacer las pruebas.  Sí puede usar WSL 2.
Estos son los requerimientos para aprobar su tarea:

+ make run-san debe felicitarlo y no debe reportar ningún problema.
+ make run-g debe felicitarlo.
+ make run debe felicitarlo por aprobar este modo de ejecución.  Esta
  prueba será rechazada si la función solicitada es 80% más lenta que
  la solución del profesor.

Si no cumple con estos 3 requerimientos su nota será 1.0.

Para depurar use: make ddd

Si falla alguno de los tests, no olvide que debe especificar el archivo
que contiene la cola al ejecutar con run en ddd:

run cola.txt

Además, antes de ejecutar probablemente necesite volver a la cola original
con este comando en el terminal de Debian:

cp cola-orig.txt cola.txt

Video con ejemplos de uso de ddd: https://youtu.be/FtHZy7UkTT4
Archivos con los ejemplos: https://www.u-cursos.cl/ingenieria/2020/2/CC3301/1/novedades/r/demo-ddd.zip

-----------------------------------------------

Entrega de la tarea

Invoque el comando make zip para ejecutar todos los tests y generar un
archivo extraer.zip que contiene extraer.c, con su solución,
y resultados.txt, con la salida de make run, make run-g y make run-san.

Ud. debe entregar por U-cursos el archivo extraer.zip.

A continuación es muy importante que descargue de U-cursos el mismo
archivo que subió, luego descargue nuevamente los archivos adjuntos y
vuelva a probar la tarea tal cual como la entregó.  Esto es para
evitar que Ud. reciba un 1.0 en su tarea porque entregó los archivos
equivocados.  Créame, sucede a menudo por ahorrarse esta verificación.

-----------------------------------------------

Limpieza de archivos

make clean

Hace limpieza borrando todos los archivos que se pueden volver
a reconstruir a partir de los fuentes: *.o binarios etc.

-----------------------------------------------

Acerca del comando make

El comando make sirve para automatizar el proceso de compilación asegurando
recompilar el archivo binario ejecutable cuando cambió uno de los archivos
fuentes de los cuales depende.

A veces es útil usar make con la opción -n para que solo muestre
exactamente qué comandos va a ejecutar, sin ejecutarlos de verdad.
Por ejemplo:

   make -n ddd

También es útil usar make con la opción -B para forzar la recompilación
de los fuentes a pesar de que no han cambiado desde la última compilación.
Por ejemplo:

   make -B run
