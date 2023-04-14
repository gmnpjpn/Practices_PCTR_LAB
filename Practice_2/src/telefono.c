
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <sys/mman.h> // @German: No se si es necesario, lo he usado para el mmap de la variable de memoria compartida.

#include <definitions.h>
#include <memoriaI.h>
#include <semaforoI.h>

// Modulo principal
int main(int argc,char *argv[]){


	//TODO: Esquema según especificado en la práctica.

    // Define variables locales



    // Coge semáforos y memoria compartida
    sem_t* semaforoMutexEspera = crear_sem(MUTEXESPERA,1);
    sem_t* semaforoTelefonos = crear_sem(TELEFONOS,0);
    sem_t* semaforoLineas = crear_sem(LINEAS,0);
    
    // @German: Obtengo el descriptor variable de memoria compartida.
    int descriptorMemCompartida = obtener_var(LLAMADASESPERA);

    // @German: Mapeo la variable de memoria compartida en la memoria del proceso hijo y apunto al valor.
    int* punteroValor = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, descriptorMemCompartida, 0);

    // @German: Asigno a una variable local el valor de la variable de memoria compartida.
    int valorEspera = *punteroValor;


    // Se pone en estado de libre incrementando el número de teléfonos libres
    while(1){

        //obtener pid proceso actual
        pid_t pid = getpid();

        // Mensaje de Espera
        printf("Teléfono [%d] en espera...\n",pid);

		//TODO: Aquí hay que realizar procesos


        // Mensaje de en conversacion
        printf("Teléfono [%d] en conversacion... Nº Llamadas en espera: %d\n",pid,valorEspera);

        // Espera en conversación
        sleep(rand() % 10 + 10);
    }

    return EXIT_SUCCESS;
}