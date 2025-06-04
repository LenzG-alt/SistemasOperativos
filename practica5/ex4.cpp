#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main() {
    int padre_a_hijo[2]; // Pipe 1
    int hijo_a_padre[2]; // Pipe 2
    pid_t pid;
    char buffer[100];

    // Crear los dos pipes
    if (pipe(padre_a_hijo) == -1 || pipe(hijo_a_padre) == -1) {
        perror("Error al crear los pipes");
        exit(1);
    }

    pid = fork();

    if (pid < 0) {
        perror("Error al crear el proceso hijo");
        exit(1);
    }

    if (pid == 0) {
        // PROCESO HIJO

        // Cerrar extremos innecesarios
        close(padre_a_hijo[1]); // No escribe al padre
        close(hijo_a_padre[0]); // No lee del padre

        // Leer mensaje del padre
        read(padre_a_hijo[0], buffer, sizeof(buffer));
        printf("Hijo recibió: %s\n", buffer);

        // Responder al padre
        strcpy(buffer, "Hola Padre");
        write(hijo_a_padre[1], buffer, strlen(buffer)+1);

        // Cerrar extremos usados
        close(padre_a_hijo[0]);
        close(hijo_a_padre[1]);

    } else {
        // PROCESO PADRE

        // Cerrar extremos innecesarios
        close(padre_a_hijo[0]); // No lee del hijo
        close(hijo_a_padre[1]); // No escribe al hijo

        // Enviar mensaje al hijo
        strcpy(buffer, "Hola Hijo");
        write(padre_a_hijo[1], buffer, strlen(buffer)+1);

        // Leer respuesta del hijo
        read(hijo_a_padre[0], buffer, sizeof(buffer));
        printf("Padre recibió: %s\n", buffer);

        // Cerrar extremos usados
        close(padre_a_hijo[1]);
        close(hijo_a_padre[0]);
    }

    return 0;
}
