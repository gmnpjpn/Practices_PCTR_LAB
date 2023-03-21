#include <stdio.h>
#include <stdlib.h>
#include <definitions.h>
#include <string.h>

// Crea una lista con un nodo.
void crearLista(TLista *pLista, char *clase)
{
  pLista->pPrimero = malloc(sizeof(TNodo)); // Se reserva espacio en memoria
  strcpy(pLista->pPrimero->clase ,clase); // Copia clase en pPrimero
  pLista->pPrimero->pSiguiente=NULL;  // Apunto el pSiguiente a NULL
}

void destruirLista(TLista *pLista) // Recorre la lista nodo a nodo y la vacia, al acabar libera la memoria de la lista
{
  TNodo *aux1;
  struct TProcess_t *aux2;

  for (aux1 = pLista->pPrimero; aux1 != NULL;)
  {
    aux2 = aux1->pSiguiente;
    free(aux1);
    aux1 = aux2;
  }

  free(pLista);
}

void insertPpioLista(TLista *pLista, char *clase) // Inserta un nodo al principio de la lista
                                                  // Se desplazan los valores de los nodos a los auxiliares para introducir
                                                  // la clase que pasemos al inicio
{
 TNodo *pAux1;
 struct TProcess_t *pAux2;
  TNodo *pAux3;

  pAux1= pLista->pPrimero;
  pAux2= pAux1->pSiguiente;
  pAux3=malloc(sizeof(TNodo));
 
  strcpy(pAux3->clase,clase);
  pLista->pPrimero=pAux3;
  pAux3->pSiguiente=pAux1;
  pAux1->pSiguiente=pAux2;
}

void insertarFinal(TLista *pLista, char *clase) // Recorre  la lista hasta llegar al final (busca el pSiguiente que apunte a NULL),
                                                // Introduce la clase al final y deja de recorrer
{
  struct TProcess_t *pAux1;
  struct TProcess_t *pAux2;
  pAux1 = pLista->pPrimero; 
 
  do{
     
    pAux2=pAux1->pSiguiente;
   
   if(pAux1->pSiguiente==NULL){
pAux1->pSiguiente= malloc(sizeof(TNodo));

   strcpy(pAux1->pSiguiente->clase ,clase);
  pAux1->pSiguiente->pSiguiente=NULL;
    }
    pAux1=pAux2;
    
    }while(pAux1!=NULL);
}

// Suponemos n = 1, 2, ...
void insertarN(TLista *pLista, int index, char *clase)   // Se recorre la lista buscando el index deseado con i, se comprueba
                                                         // que sea una posicion valida e introduce la clase, si lo es
                                                         // Si no es valida muestra un error
{
  TNodo *pAux1;
  struct TProcess_t *pAux2;
  struct TProcess_t *pAux3;
int i=1;
pAux1 = pLista->pPrimero; 
  do{
    i++;
    pAux2 = pAux1->pSiguiente;

    if(i==index && pAux1!=NULL){
      pAux3=malloc(sizeof(TNodo));
      strcpy(pAux3->clase,clase);
      pAux1->pSiguiente=pAux3;
      pAux3->pSiguiente=pAux2;
      } if(pAux1==NULL){
      printf("No se ha podido meter el valor ya que no existe el nodo %d \n",i);
    }
    pAux1 = pAux2;

  }while(pAux2!=NULL);
  
}

void eliminarPrimero(TLista *pLista) // Elimina el primer elemento de la lista
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


void eliminarN(TLista *pLista, int index) // Eliminar el nodo de la posicion "index" que le paso
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

char *getElementoN(TLista *pLista, int index) // Obtiene el nodo de la posicion "index" que le paso
{
  if (pLista->pPrimero != NULL) // Entro si la lista contiene algo
  {
    TNodo *nodoActual = pLista->pPrimero;
    int i;

    for (i = 0; i < index && nodoActual != NULL; i++) // Recorre la lista buscando la posicion anterior al "index" y hace "get" del "pSiguiente" del nodo anterior a "index"
    {
      nodoActual = nodoActual->pSiguiente;
    }

    if (i == index)
    {
      // printf("El elemento %d de la lista es: %s\n", index, nodoActual->valor);
      return nodoActual->clase;
    }
  }

  return 0;
}

void imprimir(TLista *pLista) // Recorre la lista e imprime el valor junto a su posicion
{
  TNodo *pAux1;
  struct TProcess_t *pAux2;
int i;
  for(i=1,pAux1 = pLista->pPrimero; pAux1 != NULL;i++)
  {
    
    pAux2 = pAux1->pSiguiente;
     printf("Index = %d \t Valor = %s\n", i,pAux1->clase); 
    pAux1 = pAux2;
    
  }
}

int longitud(TLista *pLista)  // Recorre la lista incrementando un contador cada vez que encuentra elementos
{
  TNodo *pAux1;
  struct TProcess_t *pAux2;
int i=0;
  for(pAux1 = pLista->pPrimero; pAux1 != NULL;)
  {
    
    pAux2 = pAux1->pSiguiente;
     
    pAux1 = pAux2;
    i++;
  }
  
  return i;
}