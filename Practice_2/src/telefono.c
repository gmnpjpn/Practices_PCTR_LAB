#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <definitions.h>
#include <memoriaI.h>
#include <semaforoI.h>

// Modulo principal
int main(int argc,char *argv[]){
    // Define variables locales
    int valorEspera;

    // Coge semáforos y memoria compartida
    sem_t *semaforoMutexEspera = get_sem(MUTEXESPERA);
    sem_t *semaforoTelefonos = get_sem(TELEFONOS);
    sem_t *semaforoLineas = get_sem(LINEAS);

    int descriptorMemCompartida = obtener_var(LLAMADASESPERA);

    // Se pone en estado de libre incrementando el número de teléfonos libres
    while(1){

        //obtener pid proceso actual
        pid_t pid = getpid();

        // Mensaje de Espera
        printf("Teléfono [%d] en espera...\n",pid);

        signal_sem(semaforoTelefonos);
        wait_sem(semaforoLineas);
        
        wait_sem(semaforoMutexEspera);
        consultar_var(descriptorMemCompartida, &valorEspera);
        modificar_var(descriptorMemCompartida,--valorEspera);
        signal_sem(semaforoMutexEspera);

        // Mensaje de en conversacion
        printf("Teléfono [%d] en conversacion... Nº Llamadas en espera: %d\n",pid,valorEspera);

        // Espera en conversación
        sleep(rand() % 10 + 10);
    }

    return EXIT_SUCCESS;
}