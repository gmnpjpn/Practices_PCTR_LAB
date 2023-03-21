#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <linux/limits.h>
#include <unistd.h> // Incluido para evitar warnings


void procesador(const char *nombre_fichero, const char *patron);
void procesar_linea(char *linea,const char *patron,int nlinea);

int main(int argc, char *argv[])
{
 
procesador(argv[1],argv[2]);

  return EXIT_SUCCESS;
}
void procesar_linea(char *linea,const char *patron,int nlinea ){
  
char *token;
  token = strtok(linea, " ");

  while (token != NULL)
  {
  
    if(strcmp(token,patron)==0){

  printf("[PROCESADOR %d] Patron '%s' encontrado en la linea %d \n", getpid(), patron,nlinea);

}
  
    token = strtok(NULL, " ");
    
  }
  
}
void procesador(const char *nombre_fichero, const char *patron){

FILE *fp;
char linea[PATH_MAX];
int nlinea=0;
int operaciones = atoi(patron);
for (int i = 0; i <=operaciones; i++)
  {
if ((fp = fopen(nombre_fichero, "r")) == NULL)
    {
      fprintf(stderr, "Error de apertura del archivo %s\n", nombre_fichero);
      exit(EXIT_FAILURE);
    }
  


 while(fgets(linea,sizeof(linea),fp)!=NULL){

procesar_linea(linea,patron,nlinea);
++nlinea;


 }

  }
  fclose(fp);
}