#include <stdio.h>
#include <stdlib.h>
#include <lista.h>

// Crea una lista con un nodo.
void crear(TLista *pLista, int valor)
{
  pLista->pPrimero = malloc(sizeof(TNodo));
  pLista->pPrimero->valor = valor;
  pLista->pPrimero->pSiguiente = NULL;
}

void destruir(TLista *pLista)
{
  TNodo *pAux1;
  struct Nodo *pAux2;

  for (pAux1 = pLista->pPrimero; pAux1 != NULL;)
  {
    pAux2 = pAux1->pSiguiente;
    free(pAux1);
    pAux1 = pAux2;
  }

  free(pLista);
}

// Inserta al ppio de la lista.
void insertar(TLista *pLista, int valor)
{
  // Creo el nuevo nodo
  TNodo *nuevoNodo = malloc(sizeof(TNodo));

  // Asigno valor y apunto al nodo que ya habia
  nuevoNodo->valor = valor;
  nuevoNodo->pSiguiente = pLista->pPrimero;

  // La lista establece como primero, el nuevo nodo
  pLista->pPrimero = nuevoNodo;
}

void insertarFinal(TLista *pLista, int valor)
{
  // Creo el nuevo nodo
  TNodo *nuevoNodo = malloc(sizeof(TNodo));

  // Asigno valor y apunto a NULL
  nuevoNodo->valor = valor;
  nuevoNodo->pSiguiente = NULL;

  // Ahora empieza a evaluar posibles situaciones
  if(pLista->pPrimero == NULL)  // Si la lista esta vacia, el nuevo nodo se convierte en el primero
  {
    pLista->pPrimero = nuevoNodo;
  
  }
  else  // Si la lista no está vacía, creo ultimoNodo que avanza hasta el final y hago que apunte a nuevoNodo
  {
    TNodo *ultimoNodo = pLista->pPrimero;

    while (ultimoNodo->pSiguiente != NULL)
    {
      ultimoNodo = ultimoNodo->pSiguiente;
    }

    ultimoNodo->pSiguiente = nuevoNodo;
  }

}

// Suponemos n = 1, 2, ...
void insertarN(TLista *pLista, int index, int valor)
{
  // Creo el nuevo nodo y asigno valor
  TNodo *nuevoNodo = malloc(sizeof(TNodo));
  nuevoNodo->valor = valor;

  if (index == 0) // Si el index es 0, el nuevo nodo se convierte en el primero
  {
    nuevoNodo->pSiguiente = pLista->pPrimero;
    pLista->pPrimero = nuevoNodo;
  }
  else  //Si index > 0, se crea nodoActual y se avanza hasta el nodo anterior al index
  {
    TNodo *nodoActual = pLista->pPrimero;
    int i = 0;

    while (i < (index-1) && nodoActual->pSiguiente != NULL)
    {
      nodoActual = nodoActual->pSiguiente;
      i++;
    }

    // Estas dos lineas no acabo de verlas claras
    nuevoNodo->pSiguiente = nodoActual->pSiguiente; // Establecer el siguiente nodo del nuevo nodo al nodo que antes estaba en la posición n
    nodoActual->pSiguiente = nuevoNodo; // Establecer el siguiente nodo del nodo actual en la posición n-1 al nuevo nodo
  }

}

// Elimina el primer elemento de la lista.
void eliminar(TLista *pLista)
{
  //Con el if, evito "eliminar" algo de una lista vacia
  if (pLista->pPrimero != NULL)
  {
    // Uso nodoEliminar como un aux para copiar datos y sobreescribir pPrimero
    TNodo *nodoEliminar = pLista->pPrimero;
    pLista->pPrimero = nodoEliminar->pSiguiente;
    free(nodoEliminar);
  }
}


// Este me lio un poco,deberia repasarlo
void eliminarN(TLista *pLista, int index)
{
  if (pLista->pPrimero != NULL) // Entro si la lista contiene algo
  {
    if (index == 0) // En caso de que se quiera borrar el primer elemento
    {
      TNodo *nodoEliminar = pLista->pPrimero;
      pLista->pPrimero = nodoEliminar->pSiguiente;
      free(nodoEliminar);
    }
  
    else  // En caso de que se quiera borrar cualquier elemento que no sea el primero
    {
      TNodo *nodoAnterior = pLista->pPrimero;
      int i;

      for (i = 0; i < (index-1); i++)
      {
        nodoAnterior = nodoAnterior->pSiguiente;
      }

      if (i == (index-1) && nodoAnterior->pSiguiente != NULL)
      {
        TNodo *nodoEliminar = nodoAnterior->pSiguiente;
        nodoAnterior->pSiguiente = nodoEliminar->pSiguiente;

        free(nodoEliminar);
      }
    }
  }
}

// Este me lio un poco,deberia repasarlo
int getElementoN(TLista *pLista, int index)
{
  if (pLista->pPrimero != NULL) // Entro si la lista contiene algo
  {
    TNodo *nodoActual = pLista->pPrimero;
    int i;

    for (i = 0; i < index && nodoActual != NULL; i++)
    {
      nodoActual = nodoActual->pSiguiente;
    }

    if (i == index && nodoActual != NULL)
    {
      printf("El elemento %d de la lista es: %d\n", index, nodoActual->valor);
      return nodoActual->valor;
    }
  }

  return 0;
}

void imprimir(TLista *pLista)
{
  TNodo *nodoActual = pLista->pPrimero;

  // Recorro la lista hasta llegar al final
  while (nodoActual != NULL){
    // Imprimo el valor del nodo actual
    printf("%d", nodoActual->valor);

    //Avanzo al siguiente nodo
    nodoActual = nodoActual->pSiguiente;

    printf("\n");
  }
  
}

int longitud(TLista *pLista)
{
  TNodo *nodoActual = pLista->pPrimero;
  int contador = 0;

  // Recorro la lista hasta llegar al final
  while (nodoActual != NULL){
    //Incremento contador
    contador++;

    // Avanzo al nodo siguiente
    nodoActual = nodoActual->pSiguiente;
  }

  printf("El número de elementos en la lista es: %d\n", contador);

  return contador;
}