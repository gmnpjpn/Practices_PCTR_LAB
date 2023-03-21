#define CLASE_PROCESADOR "PROCESADOR"
#define RUTA_PROCESADOR "./exec/procesador"
#define CLASE_CONTADOR "CONTADOR"
#define RUTA_CONTADOR "./exec/contador"
#define PATH_MAX 4096 // No sabemos muy bien para que se usa pero sin aniadirlo da error 

typedef struct TProcess_t // Estructura para los procesos
{
  char clase[PATH_MAX]; // Array para el nombre de la clase
  pid_t pid;  // PID
  struct TProcess_t  *pSiguiente; // Puntero al siguiente proceso
}TNodo;

typedef struct 
{
  TNodo *pPrimero;  // Puntero al primer proceso
} TLista;

// metodos que utilizan lista.c para los procesos
////////////////////////////////////////////////////////////
void crearLista(TLista *pLista, char *clase);
void destruirLista(TLista *pLista);

// 'insertar' inserta al ppio de la lista.
void insertPpioLista(TLista *pLista, char *clase);
void insertarFinal(TLista *pLista, char *clase);
void insertarN(TLista *pLista, int index, char *clase);

// 'elimina' elimina el último de la lista.
void eliminar(TLista *pLista);
void eliminarN(TLista *pLista, int index);

char *getElementoN(TLista *pLista, int index);

void imprimir(TLista *pLista);
int longitud(TLista *pLista);
////////////////////////////////////////////////////////////