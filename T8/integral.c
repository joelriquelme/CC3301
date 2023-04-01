#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "integral.h"
typedef long double ld;
double integral_par(Funcion f, void *ptr, double xi, double xf, int n, int p) {
  
  
  int pids[p];
  int fds[p][2];

  ld tam_interval = (xi-xf+1)/p;

  for(int i=0; i<p; i++){
    pipe(fds[i]);
    pids[i] = fork();

    if(pids[i]==0){ //trabajo con el hijo
      close(fds[i][0]);
      ld izq = xi+tam_interval*i;
      ld der = xi+tam_interval*(i+1)-1;
      if(i == p-1) der = xf;
      ld res = integral(f, ptr, izq, der, n/p);
      write(fds[i][1], &res, sizeof(ld));
      exit(0);
    }
    else{ //trabajo con el hijo
      close(fds[i][1]);
    }
  }
  ld res = 0;
  for(int i=0; i<p; i++){
    ld res_hijo;
    read(fds[i][0], &res_hijo, sizeof(ld));
    close(fds[i][0]);
    waitpid(pids[i], NULL, 0);
    if(res_hijo != 0){
      res = res_hijo;
    } 
  }
  return res;
}

