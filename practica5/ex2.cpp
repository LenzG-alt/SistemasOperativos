#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#define LEER 0
#define ESCRIBIR 1

// Usamos const char* para evitar el warning
const char *frase = "Envia esto a traves de un tubo o pipe";

int main() {
    int fd[2], bytesLeidos;
    char mensaje[100];
    int control;

    // Crear la tubería
    control = pipe(fd);
    if (control != 0) {
        perror("pipe:");
        exit(errno);
    }

    // Crear proceso hijo
    control = fork();
    switch (control) {
        case -1:
            perror("fork:");
            exit(errno);

        case 0: // Proceso hijo - Escribe en la tubería
            close(fd[LEER]); // Cierra extremo de lectura
            write(fd[ESCRIBIR], frase, strlen(frase) + 1); // +1 para incluir el '\0'
            close(fd[ESCRIBIR]);
            exit(0);

        default: // Proceso padre - Lee desde la tubería
            close(fd[ESCRIBIR]); // Cierra extremo de escritura
            bytesLeidos = read(fd[LEER], mensaje, sizeof(mensaje));
            printf("Leidos %d bytes: %s\n", bytesLeidos, mensaje);
            close(fd[LEER]);
    }

    return 0;
}