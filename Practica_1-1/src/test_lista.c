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
  insertarN(pLista, 1, 100);
  eliminar(pLista);
  eliminarN(pLista, 0);

  imprimir(pLista);

  longitud(pLista);
  getElementoN(pLista, 0);


  destruir(pLista);

  return 0;
}
