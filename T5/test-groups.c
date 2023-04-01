#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "groups.h"

void printBin(unsigned int x) {
  printf("0b");
  int bits[sizeof(unsigned int)*8];
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
}

void verificar(unsigned int x, int refer) {
  int res= groups(x);
  if (res!=refer) {
    printf("groups(");
    printBin(x);
    printf(") entrega %d.  Debio entregar %d\n", res, refer);
    exit(1);
  }
}

int main(int argc, char *argv[]) {
  verificar(0b0, 0);
  verificar(0b1, 1);
  verificar(0b111100, 1);
  verificar(0b11100001111000000000000, 2);
  verificar(0b110011000100111110011, 5);
  verificar(0b1010101010101010101, 10);
  verificar(0b11111111101111111110111111011111, 4);

  printf("Felicitaciones: funciona correctamente\n");

  return 0;
}
 
