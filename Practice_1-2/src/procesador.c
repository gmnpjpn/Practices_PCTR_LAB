#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <linux/limits.h>
#include <unistd.h> // Incluido para evitar warnings


void procesador(const char *nomFich, const char *patron);
void procesar_linea(char *linea,const char *patron,int nlinea);

int main(int argc, char *argv[])
{
 
procesador(argv[1],argv[2]); // Se encarga de buscar los patrones en cada linea recibida

  return EXIT_SUCCESS;
}
void procesar_linea(char *linea,const char *patron,int nlinea ){  // Es la funcion que realmente procesa la linea
                                                                  // Recibe la linea, el patron y el numero de linea
  
  char *key = strtok(linea, " "); // strtok() separa cada palabra de la linea con un delimitador, en este caso " "

  while (key != NULL)
  {
    if(strcmp(key,patron)==0){  // strcmp() compara cada palabra cortada con strtok() con el patron
      printf("[PROCESADOR %d] Patron '%s' encontrado en la linea %d \n", getpid(), patron,nlinea);
    }
    key = strtok(NULL, " ");
  }
  
}
void procesador(const char *nombre_fichero, const char *patron){  // Abre el fichero y lee linea a linea.
                                                                  // Cuando tiene una linea completa se la pasa a otra funcion

  FILE *fp;
  char linea[PATH_MAX];
  int nlinea=0;
  int operaciones = atoi(patron);
  for (int i = 0; i <=operaciones; i++)
  {
    if ((fp = fopen(nombre_fichero, "r")) == NULL)  // Si no puede abrir el fichero en modo lectura imprime error y finaliza
    {
      fprintf(stderr, "Error de apertura del archivo %s\n", nombre_fichero);
      exit(EXIT_FAILURE);
    }
    
    while(fgets(linea,sizeof(linea),fp)!=NULL)  // Mientras haya lineas no vacias, procesa
    {
      procesar_linea(linea,patron,nlinea);  // Funcion que realmente procesa la linea
      ++nlinea;
    }

  }
  
  fclose(fp);
}