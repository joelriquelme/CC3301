#include <stdio.h>
#include <stdlib.h>

#include "calzar.h"

int calzar(Nodo *a, Nodo **ppat){
  if (*ppat == NULL){
    *ppat = a;
    return 1;
  }
  if (a==NULL){
    return 0;
  }
  if (a->x!=(*ppat)->x){
    return 0;
  }
  if ((calzar(a->izq,&((*ppat)->izq))) && (calzar(a->der,&((*ppat)->der)))){
    return 1;
  }
  else {
    return 0;
  }
}
