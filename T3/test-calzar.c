#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>

#include "calzar.h"

#define TOLERANCIA 80
#define NINTENTOS 5

// Tamanno del arbol para el ejemplo final y el benchmark
#ifdef OPT
#define N 10000000
#else
#define N 300000
#endif

#pragma GCC diagnostic ignored "-Wunused-function"

// ----------------------------------------------------
// Funcion que entrega el tiempo transcurrido desde el lanzamiento del
// programa en milisegundos

static long long time0= 0;

static long long getTime0() {
    struct rusage ru;
    int rc= getrusage(RUSAGE_SELF, &ru);
    if (rc!=0) {
      perror("getTime");
      exit(1);
    }
    return (long long)ru.ru_utime.tv_sec*1000000+ru.ru_utime.tv_usec;
}

static void resetTime() {
  time0= getTime0();
}

static int getTime() {
  return (getTime0()-time0+500)/1000;
}

// ----------------------------------------------------
// Funcion que construye un arbol

static Nodo *abb(int x, Nodo *izq, Nodo *der) {
  Nodo *a= malloc(sizeof(*a));
  a->x= x;
  a->izq= izq;
  a->der= der;
  return a;
}

// ----------------------------------------------------
// Funcion que construye un arbol constituido de una sola hoja

static Nodo *hoja(int x) {
  return abb(x, NULL, NULL);
}

// ----------------------------------------------------
// Crea una copia de un arbol binario

static Nodo *copia(Nodo *a) {
  if (a==NULL)
    return NULL;

  Nodo *an= malloc(sizeof(Nodo));
  an->x= a->x;
  an->izq= copia(a->izq);
  an->der= copia(a->der);
  return an;
}

// ----------------------------------------------------
// Crea un arbol binario de busqueda bien equilibrado en donde 
// los x van de i a j

static Nodo *equilibrado(int i, int j) {
  if (i>j)
    return NULL;
  int k= (i+j+1)/2;
  Nodo *a= malloc(sizeof(Nodo));
  a->x= k;
  a->izq= equilibrado(i, k-1);
  a->der= equilibrado(k+1, j);
  return a;
}

// ----------------------------------------------------
// Funcion que verifica que 2 arboles binarios son iguales

static void revisar_abb(Nodo *a, Nodo *b) {
  if (a==NULL) {
    if (b==NULL)
      return;
    fprintf(stderr, "Arboles distintos: a es NULL, b tiene etiqueta %d\n",
                    b->x);
    exit(1);
  }
  else if (b==NULL) {
    fprintf(stderr, "Arboles distintos: b es NULL, a tiene etiqueta %d\n",
                    a->x);
    exit(1);
  }
  else {
    if (a->x!=b->x) {
      fprintf(stderr, "Etiquetas de arboles son distintas: %d != %d\n",
                      a->x, b->x);
      exit(1);
    }
    revisar_abb(a->izq, b->izq);
    revisar_abb(a->der, b->der);
  }
}

// ----------------------------------------------------
// Liberar un arbol binario

static void liberar(Nodo *a) {
  if (a==NULL)
    return;
  liberar(a->izq);
  liberar(a->der);
  free(a);
}

// ----------------------------------------------------
// Podar las hojas de un arbol (nodos sin hijos)

void podar(Nodo **pa, int liberar) {
  Nodo *a= *pa;
  if (a==NULL)
    return;
  if (a->izq==NULL && a->der==NULL) {
    if (liberar)
      free(a);
    *pa= NULL;
    return;
  }
  podar(&a->izq, liberar);
  podar(&a->der, liberar);
}

void mostrar(Nodo *a, int lvl) {
  int i;
  for (i= 0; i<lvl; i++)
    putchar(' ');

  if (a==NULL) {
    printf("NULL\n");
  }
  else {
    printf("%d\n", a->x);
    mostrar(a->izq, lvl+2);
    mostrar(a->der, lvl+2);
  }
}

int main(int argc, char *argv[]) {
  int bench= 0;
  int tiempo_calzar_prof= 0;
  if (argc==2) {
    bench= 1;
    tiempo_calzar_prof= atoi(argv[1]);
  }

  {
    printf("Ejemplo 1\n");
    Nodo *a= hoja(1);
    Nodo *b= NULL;
    int res= calzar(a, &b);
    if (!res || b!=a) {
      fprintf(stderr, "Ejemplo 1 falla\n");
      exit(1);
    }
    free(a);
    printf("Aprobado\n");
  }

  {
    printf("Ejemplo 2\n");
    Nodo *a= NULL;
    Nodo *b= hoja(1);
    int res= calzar(a, &b);
    if (res) {
      fprintf(stderr, "Ejemplo 2 falla\n");
      exit(1);
    }
    free(b);
    printf("Aprobado\n");
  }

  {
    printf("Ejemplo 3\n");
    Nodo *a= hoja(1);
    Nodo *b= hoja(2);
    int res= calzar(a, &b);
    if (res) {
      fprintf(stderr, "Ejemplo 3 falla\n");
      exit(1);
    }
    free(a);
    free(b);
    printf("Aprobado\n");
  }

  {
    printf("Ejemplo 4\n");
    Nodo *a= abb(4, abb(2, hoja(1), hoja(3)), hoja(5));
    Nodo *b= abb(4, hoja(2), NULL);
    int res= calzar(a, &b);
    if (!res) {
      fprintf(stderr, "Ejemplo 4 falla\n");
      exit(1);
    }
    if (b->izq->izq!=a->izq->izq || b->izq->der!=a->izq->der ||
        b->der!=a->der) {
      fprintf(stderr, "Ejemplo 4 falla\n");
      exit(1);
    }
    liberar(a);
    free(b->izq);
    free(b);
    printf("Aprobado\n");
  }

  {
    printf("Ejemplo 5\n");
    Nodo *a= abb(4, abb(2, hoja(1), hoja(3)), NULL);
    Nodo *b= abb(4, NULL, hoja(5));
    int res= calzar(a, &b);
    if (res) {
      fprintf(stderr, "Ejemplo 5 falla\n");
      exit(1);
    }
    if (b->izq!=a->izq) {
      fprintf(stderr, "Ejemplo 5 falla\n");
      exit(1);
    }
    liberar(a);
    free(b->der);
    free(b);
    printf("Aprobado\n");
  }

  int n= bench ? NINTENTOS : 1;
  for (int i= 0; i<n; i++) {
    printf("Ejemplo grande con un arbol de %d nodos\n", N);
    Nodo *a= equilibrado(1, N);
    Nodo *b= copia(a);
    podar(&b, 1); // liberar los nodos hojas de b (no tienen ningun hijo)
    int res= calzar(a, &b);
    if (!res) {
      fprintf(stderr, "Ejemplo grande falla\n");
      exit(1);
    }
    revisar_abb(a, b);
    podar(&b, 0); // quita los nodos hojas de b, sin liberar los nodos
    liberar(a);
    liberar(b);
    printf("Aprobado\n");
  }

  if (bench) {
    printf("-------------------\n");
    printf("Benchmark de calzar\n");
    printf("Se haran %d intentos\n", NINTENTOS);
    printf("-------------------\n");
    printf("Tiempo maximo para la version del profesor= %d milisegundos\n",
           tiempo_calzar_prof);
    int tiempo_calzar= 0;
    int tiempo_calzar_max= 0;
    int intento= 1;
    while (intento<=NINTENTOS) {
      printf("Intento= %d\n", intento);
      Nodo *a= equilibrado(1, N);
      Nodo *b= copia(a);
      podar(&b, 1); // liberar los nodos hojas de b (no tienen ningun hijo)
      resetTime();
      int res= calzar(a, &b);
      tiempo_calzar= getTime();
      if (tiempo_calzar>tiempo_calzar_max)
        tiempo_calzar_max= tiempo_calzar;
      if (!res) {
        fprintf(stderr, "Ejemplo grande falla\n");
        exit(1);
      }
      revisar_abb(a, b);
      podar(&b, 0); // quita los nodos hojas de b, sin liberar los nodos
      liberar(a);
      liberar(b);
      printf("Tiempo para su version= %d milisegundos\n", tiempo_calzar);
      if (tiempo_calzar_prof!=0 &&
          tiempo_calzar<=(100+TOLERANCIA)/100.0*tiempo_calzar_prof) {
        break;
      }
      printf("Excede en mas del %d %% la version del profesor\n", TOLERANCIA);
      if (intento<NINTENTOS)
        printf("Se hara un nuevo intento\n");
      intento++;
    }
    if (tiempo_calzar_prof!=0 && intento>NINTENTOS) {
      fprintf(stderr, "Lo siento: Despues de 5 intentos no satisface "
                      "la eficiencia requerida\n");
      fprintf(stderr,
              "Coloque su computador en modo alto rendimiento, porque el\n"
              "economizador de bateria puede alterar los resultados.\n"
              "No ejecute este programa junto a otros programas que hagan\n"
              "un uso intensivo de la CPU.  En windows puede lanzar el\n"
              "administrador de tareas para verificar que el uso de CPU\n"
              "sea bajo.\n");
      exit(1);
    }
    printf("Aprobado\n");

    if (tiempo_calzar_prof==0) {
      FILE *out= fopen("tiempo-ref.txt", "w");
      if (out==NULL) {
        perror("tiempo-ref.txt");
        exit(1);
      }
      fprintf(out, "%d\n", tiempo_calzar_max);
      fclose(out);
    }
  }

  printf("Felicitaciones, todos los tests funcionan correctamente\n");
  return 0;
}
