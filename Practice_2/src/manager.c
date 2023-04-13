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

// @German: Termina los procesos y libera la memoria.
void manejador_senhal(int sign)
{
  printf("\n[MANAGER] Terminacion del programa (Ctrl + C).\n");
  terminar_procesos();
  liberar_recursos();
  exit(EXIT_SUCCESS);
}

// @German: Genero tablas para telefonos y lineas, las lleno de ceros y actualizo variables globales.
void iniciar_tabla_procesos(int numTelefonos, int numLineas)
{
    g_process_lineas_table = malloc(numLineas * sizeof(struct TProcess_t));
    g_process_telefonos_table = malloc(numTelefonos * sizeof(struct TProcess_t));

    for (int i = 0; i <numLineas; i++)
    {
        g_lineasProcesses++;
        g_process_lineas_table[i].pid = 0;
    }

    for (int i = 0; i <numTelefonos; i++)
    {
        g_telefonosProcesses++;
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

// @German: Lanzar proceso de telefono.
void lanzar_proceso_telefono(const int indice_tabla)
{
  pid_t pid;

  switch (pid = fork())
  {
  case -1:
    fprintf(stderr, "[MANAGER] Error al lanzar proceso telefono: %s.\n", strerror(errno));
    terminar_procesos();
    liberar_recursos();
    exit(EXIT_FAILURE);
  case 0:
    // @German: No me vendria mal una explicacion de porque usar CLASE, o si funcionaria sin ello.
    if (execl(RUTA_TELEFONO, CLASE_TELEFONO, NULL) == -1)
    {
      fprintf(stderr, "[MANAGER] Error usando execl() en el proceso %s: %s.\n", CLASE_TELEFONO, strerror(errno));
      exit(EXIT_FAILURE);
    }
  }

  // @German: No acabo de entender las tablas.
  g_process_telefonos_table[indice_tabla].pid = pid;
  strcpy(g_process_telefonos_table[indice_tabla].clase , CLASE_TELEFONO); // @German: Me salta segmentation fault.
}

// @German: Lanzar proceso de linea.
void lanzar_proceso_linea(const int indice_tabla)
{
  pid_t pid;

  switch (pid = fork())
  {
  case -1:
    fprintf(stderr, "[MANAGER] Error al lanzar proceso linea: %s.\n", strerror(errno));
    terminar_procesos();
    liberar_recursos();
    exit(EXIT_FAILURE);
  case 0:
    // @German: No me vendria mal una explicacion de porque usar CLASE, o si funcionaria sin ello.
    if (execl(RUTA_TELEFONO, CLASE_LINEA, NULL) == -1)
    {
      fprintf(stderr, "[MANAGER] Error usando execl() en el proceso %s: %s.\n", CLASE_LINEA, strerror(errno));
      exit(EXIT_FAILURE);
    }
  }

  g_process_lineas_table[indice_tabla].pid = pid;
  strcpy(g_process_lineas_table[indice_tabla].clase , CLASE_LINEA);
}



// @German: Esperamos a que finalicen los procesos de las lineas y los telefonos.
void esperar_procesos()
{
    for (int i = 0; i < g_lineasProcesses; i++)
    {
        waitpid(g_process_lineas_table[i].pid, NULL, 0);
    }

    for (int i = 0; i < g_telefonosProcesses; i++)
    {
        waitpid(g_process_telefonos_table[i].pid, NULL, 0);
    }
}

// @German: Termina los procesos de las lineas y los telefonos.
void terminar_procesos()
{
    terminar_procesos_especificos(g_process_telefonos_table, g_telefonosProcesses);
    terminar_procesos_especificos(g_process_lineas_table, g_lineasProcesses);
}

// @German: Termina procesos del tipo que se le proporione.
void terminar_procesos_especificos(struct TProcess_t *process_table, int process_num)
{
    for (int i = 0; i < process_num; i++)
    {
        if (process_table[i].pid != 0)
        {
            kill(process_table[i].pid, SIGKILL);
        }
    }
}

void eliminar_sem(char *lineas)
{
    // @German: TODO
}

void eliminar_var(char *llamadasEspera)
{
    // @German: TODO
}



// @German: Libero tablas, semaforos y memoria compartida.
void liberar_recursos()
{
    free(g_process_telefonos_table);
    free(g_process_lineas_table);

    eliminar_sem(TELEFONOS);
    eliminar_sem(LINEAS);
    eliminar_sem(MUTEXESPERA);

    eliminar_var(LLAMADASESPERA);
}