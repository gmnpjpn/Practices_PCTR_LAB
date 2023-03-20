#define CLASE_PROCESADOR "PROCESADOR"
#define RUTA_PROCESADOR "./exec/procesador"
#define CLASE_CONTADOR "CONTADOR"
#define RUTA_CONTADOR "./exec/contador"
#define PATH_MAX 4096 // No sabemos muy bien para que se usa esto pero en algunos casos nos ha hecho falta que estuviera aqui definido

typedef struct TProcess_t
{
  char clase[PATH_MAX];
  pid_t pid;
  struct TProcess_t  *pSiguiente;
}TNodo;

typedef struct 
{
  TNodo *pPrimero;
} TLista;
void crear(TLista *pLista, char *clase);
void destruir(TLista *pLista);

// 'insertar' inserta al ppio de la lista.
void insertar(TLista *pLista, char *clase);
void insertarFinal(TLista *pLista, char *clase);
void insertarN(TLista *pLista, int index, char *clase);

// 'elimina' elimina el último de la lista.
void eliminar(TLista *pLista);
void eliminarN(TLista *pLista, int index);

char *getElementoN(TLista *pLista, int index);

void imprimir(TLista *pLista);
int longitud(TLista *pLista);