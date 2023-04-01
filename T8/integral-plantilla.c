#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "integral.h"

double integral_par(Funcion f, void *ptr, double xi, double xf, int n, int p) {
  // ... programe aca la solucion de su tarea ...
  // esto no cumple el speed up solicitado
  return integral(f, ptr, xi, xf, n);
}

