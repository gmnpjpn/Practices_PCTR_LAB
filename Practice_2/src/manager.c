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
  int numTelefonos = 0;
  int numLineas = 0;

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
// @German: Se usan punteros en vez de los valores directamente porque sino solo se modificarian las variables locales.
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
  printf("[MANAGER] %d procesos %s creados.\n", numTelefonos, CLASE_TELEFONO);
  for (int i = 0; i < numTelefonos; i++)
  {
    lanzar_proceso_telefono(i);
  }

  printf("[MANAGER] %d procesos %s creados.\n", numLineas, CLASE_LINEA);
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
    if (execl(RUTA_TELEFONO, CLASE_TELEFONO, NULL) == -1)
    {
      fprintf(stderr, "[MANAGER] Error usando execl() en el proceso %s: %s.\n", CLASE_TELEFONO, strerror(errno));
      exit(EXIT_FAILURE);
    }
  }

  g_process_telefonos_table[indice_tabla].pid = pid;
  g_process_telefonos_table[indice_tabla].clase = CLASE_TELEFONO;
  g_telefonosProcesses++;
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
    // @German: Lo de clase es por convencion.
    if (execl(RUTA_LINEA, CLASE_LINEA, NULL) == -1)
    {
      fprintf(stderr, "[MANAGER] Error usando execl() en el proceso %s: %s.\n", CLASE_LINEA, strerror(errno));
      exit(EXIT_FAILURE);
    }
  }

  g_process_lineas_table[indice_tabla].pid = pid;
  g_process_lineas_table[indice_tabla].clase = CLASE_LINEA;
  g_lineasProcesses++;
}



// @German: Espera a que terminen las lineas (los telefonos se terminan en terminar_procesos_especificos).
void esperar_procesos()
{
  int nLin_processes = g_lineasProcesses;
  pid_t pid;

  while (nLin_processes > 0)
  {
    pid = wait(NULL);
    for (int i = 0; i < g_lineasProcesses; i++)
    {
      if (pid == g_process_lineas_table[i].pid)
      {
        printf("[MANAGER] Proceso %s terminado [%d]...\n", g_process_lineas_table[i].clase, g_process_lineas_table[i].pid);
        g_process_lineas_table[i].pid = 0;
        nLin_processes--;
        break;
      }
    }
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
      printf("[MANAGER] Terminando proceso %s [%d]...\n", process_table[i].clase, process_table[i].pid);
      if (kill(process_table[i].pid, SIGINT) == -1)
      {
        // @German: Esto ocurre cuando el proceso ya termino.
        fprintf(stderr, "[MANAGER] Error al usar kill() en proceso %d: %s.\n", process_table[i].pid, strerror(errno));
      }
    }
  }
}

// @German: Libero tablas, semaforos y variable de memoria compartida.
void liberar_recursos()
{
  printf("[MANAGER] Liberando recursos...\n");
  free(g_process_telefonos_table);
  free(g_process_lineas_table);

  destruir_sem(TELEFONOS);
  destruir_sem(LINEAS);
  destruir_sem(MUTEXESPERA);

  destruir_var(LLAMADASESPERA);
}