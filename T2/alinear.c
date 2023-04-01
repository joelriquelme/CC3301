#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "alinear.h"

void alinear_der(char *str) {
  int largo = strlen(str);
  char *a = str + largo - 1;  
  char *b = a;
  int count = 0;
  for(int i = 0; i < largo; i++){
    char c = *b;
    if( c == ' '){
      b--;
      count += 1;
    }
    else{
      break;
    }
  }
  for(int i = count; i < largo; i++){
    char tmp = *a;
    *a = *b;
    *b = tmp;
    a--;
    b--;
  }  
}


char *alineacion_der(char *str){
  int largo = strlen(str);
  char *res= malloc(largo+1);
  char *a = str+largo-1;  
  char *b = a;
  char *d = res+largo;
  *d = '\0';
  d--;
  int count = 0;
  for(int i = 0; i < largo; i++){
    char c = *b;
    if( c == ' '){
      b--;
      count += 1;
    }
    else{
      break;
    }
  }
  for(int i = count; i < largo; i++){
    *d = *b;
    b--;
    d--;
  }
  for(int i = 0; i < count; i++){
    *d = ' ';
    d--;
  }
  return res;
}
