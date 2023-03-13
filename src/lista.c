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
  if(pLista->pPrimero == NULL){  // Si la lista esta vacia, el nuevo nodo se convierte en el primero
    pLista->pPrimero = nuevoNodo;
  
  } else {  // Si la lista no está vacía, creo ultimoNodo que avanza hasta el final y hago que apunte a nuevoNodo
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
  // TODO
}

// Elimina el primer elemento de la lista.
void eliminar(TLista *pLista)
{
  // TODO
}

void eliminarN(TLista *pLista, int index)
{
  // TODO
}

int getElementoN(TLista *pLista, int index)
{
  // TODO
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