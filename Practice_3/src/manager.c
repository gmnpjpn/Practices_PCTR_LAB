#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

#include <mqueue.h>
#include <definitions.h>

void crear_buzones();
void instalar_manejador_senhal();
void manejador_senhal(int sign);
void iniciar_tabla_procesos(int n_procesos_telefono, int n_procesos_linea);
void crear_procesos(int numTelefonos, int numLineas);
void lanzar_proceso_telefono(const int indice_tabla);
void lanzar_proceso_linea(const int indice_tabla);
void esperar_procesos();
void terminar_procesos(void);
void terminar_procesos_especificos(struct TProcess_t *process_table, int process_num);
void liberar_recursos();

int g_telefonosProcesses = 0;
int g_lineasProcesses = 0;
struct TProcess_t *g_process_telefonos_table;
struct TProcess_t *g_process_lineas_table;
mqd_t qHandlerLlamadas;
mqd_t qHandlerLineas[NUMLINEAS];
char queue_name[TAMANO_MENSAJES + 1];


int main(int argc, char *argv[])
{
    // Define variables locales

    // Creamos los buzones
    crear_buzones();

    // Manejador de Ctrl-C
    instalar_manejador_senhal();
 
    // Crea Tabla para almacenar los pids de los procesos
    iniciar_tabla_procesos(NUMTELEFONOS,NUMLINEAS);

    // Tenemos todo
    // Lanzamos los procesos
    crear_procesos(NUMTELEFONOS,NUMLINEAS);
  
    // Esperamos a que finalicen las lineas
    esperar_procesos();

    // Matamos los telefonos y cualquier otro proceso restante
    terminar_procesos();


    // Finalizamos Manager
    printf("\n[MANAGER] Terminacion del programa (todos los procesos terminados).\n");
    liberar_recursos();

    return EXIT_SUCCESS;
}


void crear_buzones()
{
  // @German: Atributos comunes a todos los buzones
  struct mq_attr attr;
  attr.mq_maxmsg = NUMLINEAS;
  attr.mq_msgsize = TAMANO_MENSAJES;

  // @German: Creo el buzon para las llamadas
  qHandlerLlamadas = mq_open(BUZON_LLAMADAS, O_WRONLY | O_CREAT, S_IWUSR | S_IRUSR, &attr);
  if (qHandlerLlamadas == -1) {
    perror("Error al crear buzón para llamadas");
    exit(EXIT_FAILURE);
  }

  // @German: Creo los buzones para las lineas
  int i;

  for (i = 0; i < NUMLINEAS; i++) {
    sprintf(queue_name, "%s%d", BUZON_LINEAS, i); // @German: Almaceno en queue_name el nombre del buzon para la linea i
    qHandlerLineas[i] = mq_open(queue_name, O_WRONLY | O_CREAT, S_IWUSR | S_IRUSR, &attr);  // @German: Creo el buzon para la linea i
    if (qHandlerLineas[i] == -1) {
      perror("Error al crear buzón para línea");
      exit(EXIT_FAILURE);
    }
  }
}

// @German: Comprueba que solo se le pasen dos argumentos y asigna esos argumentos a dos variables.
// @German: Se usan punteros en vez de los valores directamente porque sino solo se modificarian las variables locales.
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
void iniciar_tabla_procesos(int n_procesos_telefono, int n_procesos_linea)
{
  g_process_lineas_table = malloc(n_procesos_linea * sizeof(struct TProcess_t));
  g_process_telefonos_table = malloc(n_procesos_telefono * sizeof(struct TProcess_t));

  for (int i = 0; i <n_procesos_linea; i++)
  {
    g_process_lineas_table[i].pid = 0;
  }

  for (int i = 0; i <n_procesos_telefono; i++)
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
  printf("[MANAGER] %d procesos %s creados.\n", numTelefonos, CLASE_TELEFONO);

  sleep(1); // @German: Espero un segundo para que los telefonos se creen antes que las lineas.

  for (int i = 0; i < numLineas; i++)
  {
    lanzar_proceso_linea(i);
  }
  printf("[MANAGER] %d procesos %s creados.\n", numLineas, CLASE_LINEA);

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
    sprintf(queue_name, "%s%d", BUZON_LINEAS, indice_tabla);
    if (execl(RUTA_LINEA, CLASE_LINEA,queue_name, NULL) == -1)
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
/* void esperar_procesos()
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
} */

// @German: Es una version modificada y mas eficiente de esperar_procesos() subida al foro, la autoria es de Paula Castillejo.
void esperar_procesos(){
  int i;
  for (i = 0; i < NUMLINEAS; i++)
  {
    waitpid(g_process_lineas_table[i].pid, 0, 0);
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

  for (int i = 0; i < NUMLINEAS; i++)
  {
    sprintf(queue_name, "%s%d", BUZON_LINEAS, i);
    mq_close(qHandlerLineas[i]);
    mq_unlink(queue_name);
    mq_unlink(BUZON_LINEAS);
  }
  mq_close(qHandlerLlamadas);
  mq_unlink(BUZON_LLAMADAS);  

  // @German: Dijo en clase algo de que se guardaban los buzones tras cada ejecucion y habria que hacer reset.
}