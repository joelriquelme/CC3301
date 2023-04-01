// _DEFAULT_SOURCE se necesita para usar random
#define _DEFAULT_SOURCE 1

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <sys/resource.h>

#include "concat.h"

// ----------------------------------------------------
// Funcion que entrega el tiempo de CPU usado desde
// que se invoco resetTime()

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
// La misma funcionalidad de concat pero con
// multiplicaciones, divisiones y modulo
//

static uint concat_lento(uint x, int n, uint y, int m) {
  unsigned long long dosEm= pow(2, m);
  unsigned long long dosEn= pow(2, n);
  return (x % dosEn) * dosEm + y % dosEm;
}

// ----------------------------------------------------
// Conversión a binario

static char *to_binary(uint x) {
  int len= 0;
  uint z= x;
  do {
    z >>= 1;
    len++;
  } while (z!=0);
  char *s= malloc(len+1);
  char *end = s+len;
  *end-- = 0;
  z= x;
  do {
    *end-- = '0' + (z&1);
    z >>= 1;
  } while (z!=0);
  return s;
}

// ----------------------------------------------------
// Benchmark

typedef struct {
  uint x, y, ref;
  int n, m;
} Param;

static void check(uint x, int n, uint y, int m, uint ref) {
  uint result= concat_bits(x, n, y, m);
  if (result != ref) {
    fflush(stdout);
    char *x_bin= to_binary(x);
    char *y_bin= to_binary(y);
    char *result_bin= to_binary(result);
    char *ref_bin= to_binary(ref);
    fprintf(stderr, "Valor incorrecto para x=0b%s n=%d\ny=0b%s m=%d\n",
            x_bin, n, y_bin, m);
    fprintf(stderr, "concat_bits retorna 0b%s\ncuando debio ser 0b%s\n",
            result_bin, ref_bin);
    free(x_bin);
    free(y_bin);
    free(result_bin);
    free(ref_bin);
    exit(1);
  }
}

// ----------------------------------------------------
// Funcion main
//

#define NINTENTOS 5

#ifdef SAN

#define NTESTS 100

#else

#ifdef OPT
#define NTESTS 50000
#else
#define NTESTS 1000
#endif

#endif

int main(int argc, char **argv) {
  int tiempo_ref= 0;
  int bench= 0;       // si se hace o no el benchmark
  int ntests= NTESTS; // numero de tests por par (n,m)
  const int bits_uint= sizeof(uint)*8;
  int tot_tests= (bits_uint-1)*bits_uint/2*ntests;

  if (argc==2) {
    bench= 1;
    if (strcmp(argv[1], "ref")!=0)
      tiempo_ref= atoi(argv[1]);
  }

  printf("Tests unitarios\n");
  check(0b1, 1, 0b1, 1, 0b11);
  check(0b0, 1, 0b0, 1, 0b0);
  check(0b10, 2, 0b1, 1, 0b101);
  check(0b10, 2, 0b10, 2, 0b1010);
  check(0b101, 3, 0b11001, 5, 0b10111001);
  check(0b11011, 3, 0b10101, 2, 0b01101);
  check(0b1111111111111111111111111111111, 31, 0b1, 1,
        0b11111111111111111111111111111111);
  check(0b1, 1, 0b1111111111111111111111111111111, 31,
        0b11111111111111111111111111111111);
  printf("Aprobado\n");

  srandom(1000);

  printf("Test exhaustivo con %d invocaciones de concat_bits\n", tot_tests);

  Param *params= NULL, *p= NULL;
  Param *params_top= NULL;
  if (bench) {
    const size_t size= (size_t)tot_tests*sizeof(Param);
    const size_t MB= 1024*1024;
    int size_mb= (size+MB/2)/MB;
    printf("Reservando %d MB para los benchmarks\n", size_mb);
    params= malloc(size);
    p= params;
    params_top= &params[tot_tests];
  }

  for (int n=1; n<=bits_uint; n++) {
    for (int m= 1; m<=bits_uint-n; m++) {
      for (int k= 0; k<ntests; k++) {
        uint x = (random()<<1) + (random()&1);
        uint y = (random()<<1) + (random()&1);
        uint ref = concat_lento(x, n, y, m);
        check(x, n, y, m, ref);
        if (bench) {
          p->x= x; p->y= y;
          p->n= n; p->m= m;
          p->ref= ref;
          p++;
        }
      }
    }
  }

  if (p!=params_top) {
    fprintf(stderr, "*** error interno, contacte al profesor ***\n");
    exit(1);
  }

  printf("Aprobado\n");

  if (bench) {
    int i= 1;
    while (i<=NINTENTOS) {
      int niter= 10;
      printf("Corriendo benchmark (intento= %d)\n", i);
      printf("Tiempo de cpu usado por la version del profesor: "
               "%d milisegundos\n", tiempo_ref);
      if (strcmp(argv[1],"ref")!=0) {
        resetTime();
          for (p= params; p<params_top; p++) {
            uint result= concat_lento(p->x, p->n, p->y, p->m);
            if (result!=p->ref) {
              fflush(stdout);
              fprintf(stderr, "Los resultados son inconsistentes: con los "
                      " mismos\nargumentos se entregan resultados distintos\n");
              exit(1);
            }
          }
        int tiempo_lento= getTime()*niter;
        printf("Tiempo para la version sin operadores de bits (estimado): "
                  "%d milisegundos\n", tiempo_lento);
      }
      resetTime();
        for (int k= 0; k<niter; k++) {
          for (p= params; p<params_top; p++) {
            uint result= concat_bits(p->x, p->n, p->y, p->m);
            if (result!=p->ref) {
              fflush(stdout);
              fprintf(stderr, "Los resultados son inconsistentes: con los "
                      " mismos\nargumentos se entregan resultados distintos\n");
              exit(1);
            }
          }
        }
      int tiempo= getTime();
      printf("Tiempo de cpu usado por su version: %d milisegundos\n", tiempo);
      if (argc==2) {
        if (tiempo_ref!=0) {
          double q= (double)tiempo/(double)tiempo_ref;
          int porciento= (q-1.)*100;
          printf("Porcentaje de sobrecosto: %d %%\n", porciento);
          if (porciento<=80) {
            break;
          }
          else {
            fprintf(stderr, "*** Su solucion es demasiado lenta ***\n");
            if (i<NINTENTOS) {
              fprintf(stderr, "Volveremos a intentar\n");
            }
            else {
              fprintf(stderr, "Lo siento: despues de %d intentos no alcanza "
                              "el rendimiento solicitado\n", NINTENTOS);
              fprintf(stderr, 
                "Coloque su computador en modo alto rendimiento, porque el\n"
                "economizador de bateria puede alterar los resultados.\n"
                "No ejecute este programa junto a otros programas que hagan\n"
                "un uso intensivo de la CPU.  En windows puede lanzar el\n"
                "administrador de tareas para verificar que el uso de CPU\n"
                "sea bajo.\n");
              exit(1);
            }
            i++;
          }
        }
        else {
          FILE *out= fopen("tiempo-ref.txt", "w");
          if (out==NULL) {
            perror("tiempo-ref.txt");
            exit(1);
          }
          fprintf(out, "%d\n", tiempo);
          fclose(out);
          break;
        }
      }
    }
    printf("Aprobado\n");
  }

  free(params);

  printf("Felicitaciones!  Todos los tests aprobados.\n");

  return 0;
}
