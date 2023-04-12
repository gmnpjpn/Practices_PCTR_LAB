#include <errno.h>
#include <linux/limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

#include <definitions.h>
#include <memoriaI.h>
#include <semaforoI.h>


void procesar_argumentos(int argc, char *argv[], int *numTelefonos, int *numLineas);
void instalar_manejador_senhal();
void manejador_senhal(int sign);
void iniciar_tabla_procesos(int n_procesos_telefono, int n_procesos_linea);
void crear_procesos(int numTelefonos, int numLineas);
void lanzar_proceso_telefono(const int indice_tabla);
void lanzar_proceso_linea(const int indice_tabla);
void esperar_procesos();
void terminar_procesos();
void terminar_procesos_especificos(struct TProcess_t *process_table, int process_num);
void liberar_recursos();


// @German: Variables globales (por eso los prefijos g_)
int g_telefonosProcesses = 0;
int g_lineasProcesses = 0;
struct TProcess_t *g_process_telefonos_table;
struct TProcess_t *g_process_lineas_table;


int main(int argc, char *argv[])
{
    // Define variables locales
    int numTelefonos;
    int numLineas;

    // Procesa los argumentos y los guarda en las dos variables
    procesar_argumentos(argc,argv,&numTelefonos,&numLineas);

    // Creamos semáforos y memoria compartida
    crear_sem(MUTEXESPERA,1); crear_sem(TELEFONOS,0);crear_sem(LINEAS,0);
    crear_var(LLAMADASESPERA,0);  // No hay llamadas en espera

    // Manejador de Ctrl-C
    instalar_manejador_senhal();
 
    // Crea Tabla para almacenar los pids de los procesos
    iniciar_tabla_procesos(numTelefonos, numLineas);

    // Tenemos todo
    // Lanzamos los procesos
    crear_procesos(numTelefonos,numLineas);
  
    // Esperamos a que finalicen las lineas
    esperar_procesos();

    // Matamos los telefonos y cualquier otro proceso restante
    terminar_procesos();

    // Finalizamos Manager
    printf("\n[MANAGER] Terminacion del programa (todos los procesos terminados).\n");
    liberar_recursos();

    return EXIT_SUCCESS;
}

// @German: Comprueba que solo se le pasen dos argumentos y asigna esos argumentos a dos variables.
// @German: Se usan punteros en vez de los valores directamente porque sino solo se modificarían las variables locales.
void procesar_argumentos(int argc, char *argv[], int *numTelefonos, int *numLineas)
{
    if (argc != 3)
    {
        fprintf(stderr, "Error. Usa: ./exec/manager <numTelefonos> <numLineas>.\n");
        exit(EXIT_FAILURE);
    }

    *numTelefonos = atoi(argv[1]);
    *numLineas = atoi(argv[2]);
}

// @German: Crea un manejador de señal para la señal Ctrl + C, si hay algun error finaliza la ejecucion.
void instalar_manejador_senhal()
{
  if (signal(SIGINT, manejador_senhal) == SIG_ERR)
  {
    fprintf(stderr, "[MANAGER] Error al instalar el manejador de senhal: %s.\n", strerror(errno));
    exit(EXIT_FAILURE);
  }
}

// @German: Termina los procesos y libera la memoria
void manejador_senhal(int sign)
{
  printf("\n[MANAGER] Terminacion del programa (Ctrl + C).\n");
  terminar_procesos();
  liberar_recursos();
  exit(EXIT_SUCCESS);
}

// @German: Genera tablas de procesos para los telefonos y las lineas.
void iniciar_tabla_procesos(int numTelefonos, int numLineas)
{
    // @German: Asigno espacio de memoria a las tablas.
    g_process_lineas_table = malloc(numLineas * sizeof(struct TProcess_t));
    g_process_telefonos_table = malloc(numTelefonos * sizeof(struct TProcess_t));

    // @German: Lleno las tablas de ceros antes de la asignacion de PIDs "real".
    for (int i = 0; i <numLineas; i++)
    {
        g_process_lineas_table[i].pid = 0;
    }

    for (int i = 0; i <numTelefonos; i++)
    {
        g_process_telefonos_table[i].pid = 0;
    }
}

// @German: Lanza la cantidad de procesos especificados en los argumentos de entrada.
void crear_procesos(int numTelefonos, int numLineas)
{
    for (int i = 0; i < numTelefonos; i++)
    {
        lanzar_proceso_telefono(i);
    }

    for (int i = 0; i < numLineas; i++)
    {
        lanzar_proceso_linea(i);
    }
}

void esperar_procesos()
{
    //@German: TODO   
}

void terminar_procesos()
{
    // @German: TODO
}


