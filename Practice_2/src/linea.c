
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <sys/mman.h> // @German: No se si es necesario, lo he usado para el mmap de la variable de memoria compartida.

#include <definitions.h>
#include <memoriaI.h>
#include <semaforoI.h>

// Modulo principal
int main(int argc,char *argv[]){
    
    // Define variables locales
    pid_t pid = getpid();
    int valorEspera;
	
    // Coge semáforos y memoria compartida
    sem_t* semaforoMutexEspera = get_sem(MUTEXESPERA);
    sem_t* semaforoTelefonos = get_sem(TELEFONOS);
    sem_t* semaforoLineas = get_sem(LINEAS);

    int descriptorMemCompartida = obtener_var(LLAMADASESPERA);

    // Realiza una espera entre 1..60 segundos
    printf("Linea [%d] esperando llamada...\n",pid);
    sleep(rand() % 30 + 1);

    //Aumenta las llamadas en espera
    wait_sem(semaforoMutexEspera);
    consultar_var(descriptorMemCompartida,&valorEspera);
    modificar_var(descriptorMemCompartida,++valorEspera);
    signal_sem(semaforoMutexEspera);
    
    signal_sem(semaforoLineas);
    

    // Incrementar variable de memoria compartida

    // Espera telefono libre
    printf("Linea [%d] esperando telefono libre...Nº Llamadas en espera: %d\n",pid,valorEspera);

    // Lanza la llamada
    printf("Linea [%d] desviando llamada a un telefono...\n", pid);
    wait_sem(semaforoTelefonos);


    return EXIT_SUCCESS;
}