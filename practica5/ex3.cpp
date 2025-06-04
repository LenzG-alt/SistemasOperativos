#include <iostream>
#include <cstring>      // Para strcpy, strlen
#include <unistd.h>     // Para pipe, fork, read, write
#include <sys/wait.h>   // Para wait()
using namespace std;

#define READ 0
#define WRITE 1

int main() {
    int tuberia1[2], tuberia2[2]; // Dos tuberías para comunicación bidireccional
    pid_t pid;
    char buffer[100];

    // Crear las tuberías
    if (pipe(tuberia1) == -1 || pipe(tuberia2) == -1) {
        cerr << "Error al crear las tuberías" << endl;
        return 1;
    }

    // Crear proceso hijo
    pid = fork();

    if (pid < 0) {
        cerr << "Error al crear el proceso hijo" << endl;
        return 1;
    }

    // Proceso hijo
    if (pid == 0) {
        close(tuberia1[WRITE]); // Cierra escritura de tubería1
        close(tuberia2[READ]);  // Cierra lectura de tubería2

        // Leer del padre
        read(tuberia1[READ], buffer, sizeof(buffer));
        cout << "Hijo: Recibido del padre -> " << buffer << endl;

        // Enviar respuesta al padre
        const char *respuesta = "Respuesta del hijo";
        write(tuberia2[WRITE], respuesta, strlen(respuesta) + 1);
        close(tuberia2[WRITE]);
        close(tuberia1[READ]);

    } else { // Proceso padre
        close(tuberia1[READ]);  // Cierra lectura de tubería1
        close(tuberia2[WRITE]); // Cierra escritura de tubería2

        // Enviar mensaje al hijo
        const char *mensaje = "Hola hijo!";
        write(tuberia1[WRITE], mensaje, strlen(mensaje) + 1);
        close(tuberia1[WRITE]);

        // Recibir respuesta del hijo
        read(tuberia2[READ], buffer, sizeof(buffer));
        cout << "Padre: Recibido del hijo -> " << buffer << endl;

        close(tuberia2[READ]);
        wait(NULL); // Esperar a que el hijo termine
    }

    return 0;
}