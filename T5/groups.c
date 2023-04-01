#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "groups.h"

int groups(unsigned int x) {
  unsigned int mask = ~(-1 << 1);
  unsigned int m;
  int i=0;
  int grupos = 0;
  int val = 0;
  while(i<32) {
    m = mask & x;
    if((val==0) && (m==1)){
      grupos++;
      val = 1;
    }
    if(val==1 && m==0){
      val = 0;
    }
    x = x >> 1;
    i++;
  }
  return grupos;
}
