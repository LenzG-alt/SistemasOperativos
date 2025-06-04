#include<sys/types.h>
#include<wait.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(){
    pid_t idProceso;
    int estadoHijo;
    int descriptorTuberia[2];
    char buffer[100];

    if(pipe(descriptorTuberia)== -1){
        perror("No se puede crear Tuberia");
        exit(-1);
    }

    idProceso = fork();

    if(idProceso == -1){
        perror("No se puede crear proceso");
        exit(-1);
    }

    if(idProceso == 0){
        close(descriptorTuberia[1]); // Cierra el extremo de escritura en el hijo
        read(descriptorTuberia[0], buffer, 9); // Lee 9 bytes del padre
        printf("Hijo: Recibido \"%s\"\n", buffer);
        exit(0);
    }

    if(idProceso > 0){
        close(descriptorTuberia[0]); // Cierra el extremo de lectura en el padre
        printf("Padre: Envio \"Sistemas\"\n");
        strcpy(buffer,"Sistemas");
        write(descriptorTuberia[1], buffer, strlen(buffer)+1); // Envia "Sistemas" al hijo

        wait(&estadoHijo); // Espera a que el hijo termine
        exit(0);
    }

    return(1);
}