#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

void contarElementos(char *line,  int line_number);

int main(int argc, char *argv[])
{
int numLinea=strtol(argv[2], NULL, 10); // Crea un long con el numero de linea en el que se encuentra
  
  contarElementos(argv[1],numLinea);

  return EXIT_SUCCESS;
}

void contarElementos(char *linea, int numLinea) // Detecta si esta dentro o fuera de una palabra y las va contando
{
  
  int numPalabras = 0;
  bool dentroPalabra = false; 
  const char *iteracion = linea;

  do
  {
    switch (*iteracion)
    {

    case '\r':  // En caso de que encuentre un retorno de carro
      if (dentroPalabra)  // Si dentroPalabra esta a 1 (true) cuenta la palabra, pone a 0 dentroPalabra y continua ejecutando
      {
        dentroPalabra = false;
        numPalabras++;
      }
      break;  // Obliga a volver al do while
    default:
      dentroPalabra = true;
    }
  } while (*iteracion++); // Mientras haya iteraciones posteriores se continua ejecutando

  printf("[CONTADOR %d] La linea '%d' tiene %d palabras\n", getpid(), numLinea, numPalabras);
}