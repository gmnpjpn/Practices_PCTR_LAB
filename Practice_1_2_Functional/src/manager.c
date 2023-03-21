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

void procesar_argumentos(int argc, char *argv[], char **filename, char **pattern, int *lines);
void instalar_manejador_senhal(); // Crea un manejador de señal para la señal SIGINT (Ctrl + c), si hay algun error finaliza la ejecucion
void manejador_senhal(int sign);  // Termina los procesos y libera la memoria
void procesar_patrones(const char *fichero_patrones); // Comprueba que el fichero de patrones sea accesible
void procesar_linea(char *linea); // Corta cada patron y lo aniade en una lista enlazada
void iniciar_tabla_procesos(int n_procesos_contador, int n_procesos_procesador);  // Se crea una tabla que almacenara los PIDs de la cantidad necesaria de procesos
void crear_procesos(const char *nombre_fichero);  // Se cuenta el numero de concidencias por linea, se imprime y se aniaden los procesos a la tabla
void lanzar_proceso_contador(const int indice_tabla, const char *linea, const char *numero_linea_str);  // Creacion de procesos que cuentan las coincidencias
void lanzar_proceso_procesador(const int indice_tabla, const char *patron, const char *nombre_fichero); // Lanza procesos hijo de los que luego almacenará su ID y clase en la tabla 
void esperar_procesos();  // Espera a que terminen los procesos para eliminarlos de la tabla
void terminar_procesos(void); // Termina procesos mediante kill()
void liberar_recursos();  // Se liberan los recursos destinados a la tabla y a los patrones

int g_nProcesses;
struct TProcess_t *g_process_table;
TLista *patrones;

int main(int argc, char *argv[])
{
  char *nombre_fichero = NULL, *fichero_patrones = NULL;  // Inicializa las variables de los ficheros a NULL
  int lineas = 0;
  patrones = malloc(sizeof(TLista));  // Reserva espacio en memoria para la lista de patrones

  crear(patrones, "PATRONES");  // Crea una lista enlazada en la que el primero elemento contiene el valor "PATRONES"

  procesar_argumentos(argc, argv, &nombre_fichero, &fichero_patrones, &lineas); // Controla que los argumentos sean correctos y cuenta lineas del fichero principal

  instalar_manejador_senhal();  // Crea un manejador de señal para la señal SIGINT (Ctrl + c), si hay algun error finaliza la ejecucion

  procesar_patrones(fichero_patrones);  // Comprueba que el fichero de patrones sea accesible

  iniciar_tabla_procesos(lineas, longitud(patrones) - 1); // Se crea una tabla que almacenara los PIDs de la cantidad necesaria de procesos

  crear_procesos(nombre_fichero); // Se cuenta el numero de concidencias por linea, se imprime y se aniaden los procesos a la tabla

  esperar_procesos(); // Espera a que terminen los procesos para eliminarlos de la tabla

  printf("\n[MANAGER] Terminacion del programa (todos los procesos terminados).\n");  // Mensaje de despedida

  liberar_recursos(); // Se liberan los recursos destinados a la tabla y a los patrones

  return EXIT_SUCCESS;  // Finaliza la ejecucion
}

void procesar_argumentos(int argc, char *argv[], char **nombrefichero, char **fichero_patrones, int *lineas)  // Controla que los argumentos sean correctos y cuenta lineas del fichero principal
{
  FILE *fp;
  int ch;

  if (argc != 3)  // Si el numero de argumentos es distinto de los esperados, muestra mensaje y finaliza la ejecucion
  {
    fprintf(stderr, "Error. Usa: ./exec/manager <fichero> <fichero_patrones>.\n");
    exit(EXIT_FAILURE);
  }

  *nombrefichero = argv[1]; // Asigna las rutas de los ficheros
  *fichero_patrones = argv[2];  // Asigna las rutas de los ficheros

  if ((fp = fopen(*nombrefichero, "r")) == NULL)  // Si no se puede abrir el fichero en modo lectura imprime error y finaliza la ejecucion
  {
    fprintf(stderr, "Error al abrir el fichero %s\n", *nombrefichero);
    exit(EXIT_FAILURE);
  }

  while ((ch = fgetc(fp)) != EOF) // Busca "\n" (saltos de linea) en el documento para contar las lineas, finaliza cuando llega al final del fichero (EOF)
  {
    if (ch == '\n')
    {
      ++*lineas;
    }
  }

  fclose(fp);
}

void instalar_manejador_senhal()  // Crea un manejador de señal para la señal SIGINT (Ctrl + c), si hay algun error finaliza la ejecucion
{
  if (signal(SIGINT, manejador_senhal) == SIG_ERR)
  {
    fprintf(stderr, "[MANAGER] Error al instalar el manejador de senhal: %s.\n", strerror(errno));
    exit(EXIT_FAILURE);
  }
}

void manejador_senhal(int sign) // Termina los procesos y libera la memoria
{
  printf("\n[MANAGER] Terminacion del programa (Ctrl + C).\n");
  terminar_procesos();
  liberar_recursos();
  exit(EXIT_SUCCESS);
}

void procesar_patrones(const char *fichero_patrones)  // Comprueba que el fichero de patrones sea accesible
{
  FILE *fp;
  char linea[PATH_MAX];

  if ((fp = fopen(fichero_patrones, "r")) == NULL)  // Intenta abrir en modo lectura, si no se puede muestra error y finaliza la ejecucion
  {
    fprintf(stderr, "Error al abrir el fichero %s\n", fichero_patrones);
    exit(EXIT_FAILURE);
  }

  while (fgets(linea, sizeof(linea), fp) != NULL) // Mientras haya lineas que busque patrones en ellas
  {
    
    procesar_linea(linea);
  }

  fclose(fp);
}

void procesar_linea(char *linea)  // Corta cada patron (usando " " como delimitadores) a buscar y lo inserta en una lista enlazada
{
  char *token;

  token = strtok(linea, " ");
  
  while (token != NULL)
  {
    
    insertarFinal(patrones, token);
    
    token = strtok(NULL, " ");
  }
  
}

void iniciar_tabla_procesos(int n_procesos_contador, int n_procesos_procesador) // Se crea una tabla que almacenara los PIDs de la cantidad necesaria de procesos
                                                                                // Recibe esos enteros como parametros para saber cuanta memoria asignar a la tabla
{
  g_nProcesses = n_procesos_contador + n_procesos_procesador;  // Total de procesos
  g_process_table = malloc(g_nProcesses * sizeof(struct TProcess_t)); // Asignacion de memoria para la tabla

  for (int i = 0; i < g_nProcesses; i++)  // Llena la tabla de 0 antes de la asignacion de PIDs "real"
  {
    g_process_table[i].pid = 0;
  }
}

void crear_procesos(const char *nombre_fichero) // Se lanza un proceso contador que cuenta el num de veces que aparece cada
                                                // patrón por línea.
                                                // Se lanzan procesos procesadores para escribir el resultado.
                                                // Cada proceso se almacena en la tabla de procesos.
{
  FILE *fp;
  char linea[PATH_MAX], numero_linea_str[3];
  int indice_tabla = 0;
  
  if ((fp = fopen(nombre_fichero, "r")) == NULL)
  {
    fprintf(stderr, "Error al abrir el fichero %s\n", nombre_fichero);
    exit(EXIT_FAILURE);
  }

  while (fgets(linea, sizeof(linea), fp) != NULL)
  {
    sprintf(numero_linea_str, "%d", indice_tabla);
    lanzar_proceso_contador(indice_tabla, linea, numero_linea_str);

    indice_tabla++;
  }

  for (int i = 2; i <= longitud(patrones); i++)
  {
  
    lanzar_proceso_procesador(indice_tabla, getElementoN(patrones, i), nombre_fichero);
    indice_tabla++;
    
  }

  printf("[MANAGER] %d procesos creados.\n", indice_tabla);
  sleep(1);

  fclose(fp);
}

void lanzar_proceso_contador(const int indice_tabla, const char *linea, const char *numero_linea_str) // Creacion de procesos que cuentan las coincidencias
{
  pid_t pid;

  switch (pid = fork())
  {
  case -1:
    fprintf(stderr, "[MANAGER] Error al lanzar proceso contador: %s.\n", strerror(errno));
    terminar_procesos();
    liberar_recursos();
    exit(EXIT_FAILURE);
  case 0:
  
    if (execl(RUTA_CONTADOR, CLASE_CONTADOR, linea, numero_linea_str, NULL) == -1)
    {
      fprintf(stderr, "[MANAGER] Error usando execl() en el poceso %s: %s.\n", CLASE_CONTADOR, strerror(errno));
      exit(EXIT_FAILURE);
    }
  }

  g_process_table[indice_tabla].pid = pid;
  strcpy(g_process_table[indice_tabla].clase , CLASE_CONTADOR);
}

void lanzar_proceso_procesador(const int indice_tabla, const char *patron, const char *nombre_fichero)  // Lanza procesos hijo de los que luego almacenará su ID y clase en la tabla 
{
  pid_t pid;

  switch (pid = fork())
  {
  case -1:
    fprintf(stderr, "[MANAGER] Error al lanzar proceso procesador: %s.\n", strerror(errno));
    terminar_procesos();
    liberar_recursos();
    exit(EXIT_FAILURE);
  case 0:
    if (execl(RUTA_PROCESADOR, CLASE_PROCESADOR, nombre_fichero, patron, NULL) == -1)
    {
      fprintf(stderr, "[MANAGER] Error usando execl() en el proceso %s: %s.\n", CLASE_PROCESADOR, strerror(errno));
      exit(EXIT_FAILURE);
    }
  }

  g_process_table[indice_tabla].pid = pid;
  strcpy(g_process_table[indice_tabla].clase , CLASE_PROCESADOR);
}

void esperar_procesos() // Espera a que terminen los procesos para eliminarlos de la tabla
{
  int i, n_processes = g_nProcesses;
  pid_t pid;

  while (n_processes > 0)
  {
    pid = wait(NULL);
    for (i = 0; i < g_nProcesses; i++)
    {
      if (pid == g_process_table[i].pid)
      {
        printf("[MANAGER] Proceso %s terminado [%d]...\n", g_process_table[i].clase, g_process_table[i].pid);
        g_process_table[i].pid = 0;
        n_processes--;
        break;
      }
    }
  }
}

void liberar_recursos() // Se liberan los recursos destinados a la tabla y a los patrones
{
  free(g_process_table);
  destruir(patrones);
}

void terminar_procesos(void)  // Termina procesos mediante kill()
{
  int i;

  printf("\n----- [MANAGER] Terminar con los procesos hijos ejecutándose ----- \n");
  for (i = 0; i < g_nProcesses; i++)
  {
    if (g_process_table[i].pid != 0)
    {
      printf("[MANAGER] Terminando proceso %s [%d]...\n", g_process_table[i].clase, g_process_table[i].pid);
      if (kill(g_process_table[i].pid, SIGINT) == -1)
      {
        fprintf(stderr, "[MANAGER] Error al usar kill() en proceso %d: %s.\n", g_process_table[i].pid, strerror(errno));
      }
    }
  }
}
