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
    char buzonLinea[TAMANO_MENSAJES];
    char buffer[TAMANO_MENSAJES+1]; // @German: Si no me equivoco, el +1 es para para el '\0'

    // Inicia Random
    srand(pid);

    // Verifica los parámetros
    if (argc != 2)
    {
        fprintf(stderr, "Error. Usa: ./exec/linea <cola_linea_llamante>.\n");
        exit(EXIT_FAILURE);
    }

    // @German: Obtengo el nombre del buzón de la línea
    sprintf(buzonLinea,"%s",argv[1]);

    // @German: Verifico que pueda abrir el buzón de linea.
    if ((qHandlerLinea = mq_open(buzonLinea, O_RDWR)) == -1)
    {
        fprintf(stderr, "Error. No se pudo abrir el buzón de linea. %s\n", strerror(errno));
    }
    
    qHandlerLlamadas = mq_open(BUZON_LLAMADAS, O_RDWR);

    while (1)
    {
        printf("Linea %d: Esperando llamada...\n", pid);
        sleep((rand() % 30) + 1);

        // Envia mensaje de llamada
        mq_send(qHandlerLlamadas, buzonLinea, sizeof(buzonLinea), 0);
        printf("Linea %d: Llamada recibida (%s).\n", pid, buzonLinea);

        printf("Linea %d: Esperando a fin de conversacion...\n", pid);
        if (mq_receive(qHandlerLinea, buffer, sizeof(buffer), 0) == -1)
        {
            fprintf(stderr, "Error. No se pudo recibir el mensaje. %s\n", strerror(errno));
        }

        printf("Linea %d: Fin de conversacion.\n", pid);
    }
    
    return EXIT_SUCCESS;
}