
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include <mqueue.h>
#include <definitions.h>

// Modulo principal
int main(int argc,char *argv[]){

    // Define variables locales
    int pid = getpid();
    mqd_t qHandlerLlamadas;
    mqd_t qHandlerLinea;
    char buffer[TAMANO_MENSAJES];
    
    srand(pid);

    // @German: Verifico que pueda abrir el buzón de llamadas.
    if((qHandlerLlamadas = mq_open(BUZON_LLAMADAS, O_RDWR)) == -1){
        fprintf(stderr, "Error. No se pudo abrir el buzón de llamadas. %s\n", strerror(errno));
    }

    while (1)
    {
        printf("Telefono %d: Esperando llamada...\n", pid);
        if ((mq_receive(qHandlerLlamadas, buffer, sizeof(buffer), NULL)) == -1)
        {
            fprintf(stderr, "Error. No se pudo recibir llamada. %s\n", strerror(errno));
        }

        printf("Telefono %d conversando mediante la linea %s\n", pid, buffer);
        sleep((rand() % 10) + 1);

        if ((qHandlerLinea = mq_open(buffer, O_RDWR)) == -1)
        {
            fprintf(stderr, "Error. No se pudo abrir el buzón de linea. %s\n", strerror(errno));
        }

        printf("Telefono %d: Finalizando llamada...\n", pid);
        if ((mq_send(qHandlerLinea, buffer, sizeof(buffer), 0)) == -1)
        {
            fprintf(stderr, "Error. No se pudo enviar mensaje. %s\n", strerror(errno));
        }
    }
    return EXIT_SUCCESS;
}