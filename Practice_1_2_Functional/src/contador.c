#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void contarElementos(char *line,  int line_number);

int main(int argc, char *argv[])
{
int numLinea=strtol(argv[2], NULL, 10); // Crea un long con 
  
  contarElementos(argv[1],numLinea);

  return EXIT_SUCCESS;
}

void contarElementos(char *linea, int numero_linea)
{
  
  int n_palabras = 0, dentro_palabra = 0;
  const char *it = linea;

  do
  {
    switch (*it)
    {
    case '\0':
    case ' ':
    case '\t':
    case '\n':
    case '\r':
      if (dentro_palabra)
      {
        dentro_palabra = 0;
        n_palabras++;
      }
      break;
    default:
      dentro_palabra = 1;
    }
  } while (*it++);

  printf("[CONTADOR %d] La linea '%d' tiene %d palabras\n", getpid(), numero_linea, n_palabras);
}