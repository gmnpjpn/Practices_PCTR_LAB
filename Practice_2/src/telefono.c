
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <definitions.h>
#include <memoriaI.h>
#include <semaforoI.h>

// Modulo principal
int main(int argc,char *argv[]){


	//TODO: Esquema según especificado en la práctica.

    // Define variables locales
    int pid;  // Identificador del proceso          
    int valorEspera; // Número de llamadas en espera
    srand(getpid());
    






    // Coge semáforos y memoria compartida
    crear_sem(MUTEXESPERA,1); 
    crear_sem(TELEFONOS,0);
    crear_sem(LINEAS,0);
    

    pid = getpid(); // Obtiene el identificador del proceso
    valorEspera = leer_var(LLAMADASESPERA); // Obtiene el número de llamadas en espera



    
    // Se pone en estado de libre incrementando el número de teléfonos libres
    while(1){

        // Mensaje de Espera
        printf("Teléfono [%d] en espera...\n",pid);
        

		//TODO: Aquí hay que realizar procesos
        // Espera en espera
        sleep(rand() % 10 + 10);

        // Mensaje de llamada
        printf("Teléfono [%d] recibe llamada...\n",pid);

    

        // Mensaje de en conversacion
        printf("Teléfono [%d] en conversacion... Nº Llamadas en espera: %d\n",pid,valorEspera);

        // Espera en conversación
        sleep(rand() % 10 + 10);
        
    }

    return EXIT_SUCCESS;
}