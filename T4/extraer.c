#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//definir la estructura de cada linea
typedef struct {
  char texto[10];
  char prio[6];
  char newline;
} Linea;

int main(int argc, char *argv[]) {
  //error al llamar a la funcion con más parametros
  if (argc!=2) {
    fprintf(stderr, "Uso: %s <arch-cola>", argv[0]);
    exit(1);
  }
  //declarar nombre del archivo
  char *nomArch= argv[1];
  
  //abri archivo para leer
  FILE *fileCola= fopen(nomArch, "r+");
  
  //error al abrir el archivo y este no existe
  if (fileCola==NULL) {
    perror(nomArch);
    exit(2);
  }
  
  //calcular cantidad de elementos en la cola
  fseek(fileCola, 0, SEEK_SET);
  char buf[6];
  int canElem;
  canElem= atoi(fgets(buf, 6, fileCola));

  //error cola vacia
  if (canElem==0){
    fprintf(stderr, "error: cola.txt esta vacia\n");
    exit(3);  
  }

  //recorer cola y encontrar fila con menor prioridad
  Linea linea1;
  fread(&linea1, sizeof(Linea), 1, fileCola);
  int menorPrio;
  menorPrio= atoi(linea1.prio);
  int numLinea= 0;
  for (int i=1; i<canElem; i++){
    fread(&linea1, sizeof(Linea), 1, fileCola);
    int x= atoi(linea1.prio);
    if (x<menorPrio){
        menorPrio=x;
        numLinea=i;
    }
  }

  //mostrar texto de la linea con menor prioridad
  //prioridad=menorPrio , linea = numLinea
  Linea lin;
  if (fseek(fileCola, 5+(numLinea*sizeof(Linea)), SEEK_SET)!=0) {
    perror(nomArch);
    exit(4);
  }
  if (fread(&lin, sizeof(Linea), 1, fileCola)!=1) {
    perror(nomArch);
    exit(5);
  }
  char salida[11];
  memcpy(salida,lin.texto,11);
  salida[10]=0;
  printf("%s\n", salida);

  //armar nueva primera linea y copiar ultima linea
  fseek(fileCola, 0, SEEK_SET);
  int n;
  n= canElem-1;
  char s[13];
  sprintf(s, "%-4d\n", n);
  
  fseek(fileCola, 5+(n*sizeof(Linea)), SEEK_SET);
  Linea lin2;
  fread(&lin2, sizeof(Linea), 1, fileCola);

  //reescribir primera linea y linea mostrada en pantalla
  fseek(fileCola, 0, SEEK_SET);
  fputs(s,fileCola);
  fseek(fileCola, 0, SEEK_SET);
  fseek(fileCola, 5+(numLinea*sizeof(Linea)), SEEK_SET);
  char test[17];
  memcpy(test,&lin2,17);
  test[16]=0;
  fputs(test,fileCola);

  fclose(fileCola);
  
  return 0;
}

