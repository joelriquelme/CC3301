#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "sort.h"

// El arreglo de nombres de personas

void printBin(uint x) {
  printf("0b");
  int bits[sizeof(uint)*8];
  int i= 0;
  for (;;) {
    bits[i]= x&1;
    x >>= 1;
    if (x==0)
      break;
    i++;
  }
  do {
    printf("%d", bits[i--]);
  } while (i>=0);
  printf("\n");
}

void resultados(uint x, uint y) {
  printf("Resultado: ");
  printBin(x);
  printf(" ");
  printBin(y);
  printf("\n");
}

void help(int line, int i, int j) {
  printf("Para depurar use estos comandos:\n");
  printf("make sort-rv-groups.ddd\n");
  printf("o tambien make sort-c-groups.ddd\n");
  printf("Ingrese estos comandos en el panel de comandos de ddd:\n");
  printf("break test-sort.c:%d\n", line);
  printf("cond 3 i==%d && j==%d\n", i, j);
  printf("continue\n");
  printf("Abra el panel de assembler seleccionando View -> "
         "Machine Code Window\n");
  printf("Avance con stepi hasta entrar a sort-rv-groups.s\n");
  printf("Enseguida depure con stepi para avanzar lentamente.\n");
  printf("O con step para entrar a sort-c-groups.c\n");
  printf("El arreglo tiene solo 2 elementos, no va a demorarse mucho\n");
  exit(1);
}

#define N 12

unsigned int data[]= {
               0b1,                         // 1
               0b0,                         // 0
               0b1111,                      // 1
               0b11111110000,               // 1
               0b101,                       // 2
               0b11100001111,               // 2
               0b10101010101010101010101010101010, // 16
               0b110011000100111110011,     // 5
               0b111111110001110001,        // 3
               0b11111111101111111110111111011111, // 4
               0b1010101010101010101,       // 10
               0b010111111111000100100101,  // 6
             };

unsigned int refs[]= {
               0b0,                         // 0
               0b1,                         // 1
               0b1111,                      // 1
               0b11111110000,               // 1
               0b101,                       // 2
               0b11100001111,               // 2
               0b111111110001110001,        // 3
               0b11111111101111111110111111011111, // 4
               0b110011000100111110011,     // 5
               0b010111111111000100100101,  // 6
               0b1010101010101010101,       // 10
               0b10101010101010101010101010101010, // 16
             };

int groupsMap[]= {
               1,
               0,
               1,
               1,
               2,
               2,
               16,
               5,
               3,
               4,
               10,
               6
             };
  
int groups(unsigned int x) {
  for (int i= 0; i<N; i++) {
    if (x==data[i])
      return groupsMap[i];
  }
  printf("El valor ");
  printBin(x);
  printf(" no fue utilizado\n");
  exit(1);
  return 0;
}

int main(int argc, char *argv[]) {

  printf("Multiples tests con arreglos de solo 2 numeros\n");

  for (int i= 0; i<N-1; i++) {
    for (int j= i+1; j<N; j++) {
      uint nums[]={data[i], data[j]};
      sort(nums, 2);
      int groups0= groups(nums[0]);
      int groups1= groups(nums[1]);
      if (groups0>groups1) {
        printf("El ordenamiento falla para el arreglo:\n");
        printBin(refs[i]);
        printBin(refs[j]);
        printf("El resultado fue:\n");
        printBin(nums[0]);
        printBin(nums[1]);
        help(117, i, j);
      }
      uint nums2[]={data[j], data[i]};
      sort(nums2, 2);
      groups0= groups(nums[0]);
      groups1= groups(nums[1]);
      if (groups0>groups1) {
        printf("El ordenamiento falla para el arreglo:\n");
        printBin(refs[j]);
        printBin(refs[i]);
        printf("El resultado fue:\n");
        printBin(nums2[0]);
        printBin(nums2[1]);
        help(130, i, j);
      }
    }
  }

  printf("Aprobado\n");

  printf("Ahora un arreglo de 12 numeros\n");

  printf("El arreglo desordenado es:\n");
  printf("--------------------------\n");
  unsigned int nums[N];
  for (int i= 0; i<12; i++) {
    nums[i]= data[i];
    printBin(nums[i]);
  }
  sort(nums, 12);
  printf("\n");
  printf("El arreglo ordenado es:\n");
  printf("-----------------------\n");
  for (int i= 0; i<12; i++) {
    printBin(nums[i]);
  }
  for (int i= 1; i<12; i++) {
    if (groups(nums[i-1])>groups(nums[i])) {
      fprintf(stderr, "El %d-esimo elemento es incorrecto\n", i);
      exit(1);
    }
  }

  printf("Felicitaciones: todos los tests aprobados\n");
  return 0;
}
