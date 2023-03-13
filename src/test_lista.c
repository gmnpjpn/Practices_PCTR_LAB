#include <stdio.h>
#include <stdlib.h>
#include <lista.h>

int main(int argc, char *argv[])
{
  TLista *pLista;

  pLista = malloc(sizeof(TLista));

  crear(pLista, 7);

  // TODO (probar el resto de operaciones).

  insertar(pLista, 2);
  insertarFinal(pLista, 34);

  imprimir(pLista);

  longitud(pLista);

  destruir(pLista);

  return 0;
}
