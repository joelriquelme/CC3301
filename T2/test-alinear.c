// _DEFAULT_SOURCE se necesita para usar random
#define _DEFAULT_SOURCE 1

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <sys/resource.h>

#include "alinear.h"

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
// Verifica que el resultado coincida con el valor de referencia
//

static int check_alinear_der(char *s, char *ref) {
  char *copia= malloc(strlen(s)+1);
  strcpy(copia, s);
  resetTime();
  alinear_der(copia);
  int tiempo= getTime();
  if (strcmp(copia, ref)!=0) {
    fflush(stdout);
    fprintf(stderr, "Valor incorrecto para s=\"%s\"\n"
                    "el resultado fue \"%s\"\n"
                    "el resultado debio ser \"%s\"\n", s, copia, ref);
    exit(1);
  }
  free(copia);
  return tiempo;
}

static int check_alineacion_der(char *s, char *ref) {
  resetTime();
  char *result= alineacion_der(s);
  int tiempo= getTime();
  if (strcmp(result, ref)!=0) {
    fflush(stdout);
    fprintf(stderr, "Valor incorrecto para s=\"%s\"\n"
                    "el resultado fue \"%s\"\n"
                    "el resultado debio ser \"%s\"\n", s, result, ref);
    exit(1);
  }
  free(result);
  return tiempo;
}

// ----------------------------------------------------
// Construye string enorme
//

// #define TAM 20
#define TAM (100*1024*1024)
#define TOLERANCIA 1.8

static char *enorme(void) {
  char *s= malloc(2*TAM+1);
  char *top= &s[2*TAM];
  *top--= 0;
  while (top>=s)
    *top--= ' ';
  return s;
}

// ----------------------------------------------------
// Com string enorme
//

int main(int argc, char *argv[]) {
  int bench= 0;
  int tiempo_alinear_prof= 0;
  int tiempo_alineacion_prof= 0;
  if (argc==3) {
    bench= 1;
    tiempo_alinear_prof= atoi(argv[1]);
    tiempo_alineacion_prof= atoi(argv[2]);
  }

  printf("Tests unitarios de alinear_der\n");
  check_alinear_der("", "");
  check_alinear_der(" ", " ");
  check_alinear_der("   ", "   ");
  check_alinear_der("a", "a");
  check_alinear_der("b ", " b");
  check_alinear_der("1   ", "   1");
  check_alinear_der(" hola   que tal   ", "    hola   que tal");
  printf("Aprobado\n");

  printf("Tests unitarios de alineacion_der\n");
  check_alineacion_der("", "");
  check_alineacion_der(" ", " ");
  check_alineacion_der("   ", "   ");
  check_alineacion_der("a", "a");
  check_alineacion_der("b ", " b");
  check_alineacion_der("1   ", "   1");
  check_alineacion_der(" hola   que tal   ", "    hola   que tal");
  printf("Aprobado\n");

  {
    printf("Test de alinear_der con string enorme\n");
    char *s= enorme();
    s[TAM-1]= '*';
    char *ref= enorme();
    ref[2*TAM-1]= '*';
    check_alinear_der(s, ref);
    free(s);
    free(ref);
    printf("Aprobado\n");
  }

  {
    printf("Test de alineacion_der con string enorme\n");
    char *s= enorme();
    s[TAM-1]= '*';
    char *ref= enorme();
    ref[2*TAM-1]= '*';
    check_alineacion_der(s, ref);
    free(s);
    free(ref);
    printf("Aprobado\n");
  }

  if (bench) {
    int tiempo_alinear= 0;
    int tiempo_alineacion= 0;
    {
      printf("------------------------\n");
      printf("Benchmark de alinear_der\n");
      printf("------------------------\n");
      printf("Tiempo para la version del profesor= %d milisegundos\n",
             tiempo_alinear_prof);
      int intento= 1;
      while (intento<=5) {
        printf("Intento= %d\n", intento);
        char *s= enorme();
        s[TAM-1]= '*';
        char *ref= enorme();
        ref[2*TAM-1]= '*';
        int tiempo= check_alinear_der(s, ref);
        free(s);
        free(ref);
        printf("Tiempo para su version= %d milisegundos\n", tiempo);
        if (tiempo_alinear_prof==0 || tiempo<=TOLERANCIA*tiempo_alinear_prof) {
          tiempo_alinear= tiempo;
          break;
        }
        printf("Excede en mas del 50 %% la version del profesor\n");
        if (intento<5)
          printf("Se hara un nuevo intento\n");
        intento++;
      }
      if (intento>5) {
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
    }

    {
      printf("---------------------------\n");
      printf("Benchmark de alineacion_der\n");
      printf("---------------------------\n");
      printf("Tiempo para la version del profesor= %d milisegundos\n",
             tiempo_alineacion_prof);
      int intento= 1;
      while (intento<=5) {
        printf("Intento= %d\n", intento);
        char *s= enorme();
        s[TAM-1]= '*';
        char *ref= enorme();
        ref[2*TAM-1]= '*';
        int tiempo= check_alineacion_der(s, ref);
        free(s);
        free(ref);
        printf("Tiempo para su version= %d milisegundos\n", tiempo);
        if (tiempo_alineacion_prof==0 ||
            tiempo<=TOLERANCIA*tiempo_alineacion_prof) {
          tiempo_alineacion= tiempo;
          break;
        }
        printf("Excede en mas del 50 %% la version del profesor\n");
        if (intento<5)
          printf("Se hara un nuevo intento\n");
        intento++;
      }
      if (intento>5) {
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
    }

    if (tiempo_alinear_prof==0) {
      FILE *out= fopen("tiempo-ref.txt", "w");
      if (out==NULL) {
        perror("tiempo-ref.txt");
        exit(1);
      }
      fprintf(out, "%d %d\n", tiempo_alinear, tiempo_alineacion);
      fclose(out);
    }
  }

  printf("Felicitaciones, todos los tests funcionan correctamente\n");
  return 0;
}
