#include <stdio.h>
#include <stdlib.h>
#include <definitions.h>
#include <string.h>
// Crea una lista con un nodo.
void crear(TLista *pLista, char *clase)
{
  pLista->pPrimero = malloc(sizeof(TNodo));
  strcpy(pLista->pPrimero->clase ,clase); 
  pLista->pPrimero->pSiguiente=NULL;
   
}

void destruir(TLista *pLista)
{
  TNodo *pAux1;
  struct TProcess_t *pAux2;

  for (pAux1 = pLista->pPrimero; pAux1 != NULL;)
  {
    pAux2 = pAux1->pSiguiente;
    free(pAux1);
    pAux1 = pAux2;
  }

  free(pLista);
}

// Inserta al ppio de la lista.
void insertar(TLista *pLista, char *clase)
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

void insertarFinal(TLista *pLista, char *clase)
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
void insertarN(TLista *pLista, int index, char *clase)
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

// Elimina el ultimo elemento de la lista.
void eliminar(TLista *pLista)
{
  TNodo *pAux1;
  struct TProcess_t *pAux2;
  pAux1 = pLista->pPrimero; 
 
  do{
     
    pAux2=pAux1->pSiguiente;
   
   if(pAux2->pSiguiente==NULL){
     
     free(pAux2);

pAux1->pSiguiente=NULL;
   break;
    }
    pAux1=pAux2;
    
    }while(pAux1!=NULL);
 
    
}


void eliminarN(TLista *pLista, int index)
{
  TNodo *pAux1;
  struct TProcess_t *pAux2;
  struct TProcess_t *pAux3;
  
  int i=1;
   pAux1 = pLista->pPrimero;
   
   do{
    i++;
    pAux2 = pAux1->pSiguiente;
    if(i==index){

pAux3=pAux2->pSiguiente;
      free(pAux2);
      
      pAux1->pSiguiente=pAux3;
    
    }
    pAux1 = pAux2;
    

   
  }while(pAux2!=NULL);
}

char *getElementoN(TLista *pLista, int index)
{
  TNodo *pAux1;
  struct TProcess_t *pAux2;
int i=1;

  for(pAux1 = pLista->pPrimero; pAux1 != NULL;)
  {
    
    pAux2 = pAux1->pSiguiente;
    if(index==i){
return pAux1->clase;

     }
    pAux1 = pAux2;
    
   i++;
  }
  

  return 0;
}

void imprimir(TLista *pLista)
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


int longitud(TLista *pLista)
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