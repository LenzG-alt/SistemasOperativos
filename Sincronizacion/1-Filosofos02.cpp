#include <iostream> // Librería estándar para entrada y salida de datos
#include <thread> // Librería para trabajar con hilos de ejecución
#include <mutex> // Librería para trabajar con mutex (exclusión mutua)
#include <chrono> // Librería para trabajar con tiempo y duraciones
#include <random> // Librería para generar números aleatorios
#include <vector> // Librería para trabajar con vectores

using namespace std;

// Función para limitar el número máximo de fideos
int LimitFideMax(int fide) {
    const int maxFi = 2147483647;
    if (fide > maxFi) {
        cout << "\n-------------------------------------------" << endl;
        cout << "¡Se produjo un error!, el número de fideos excede la cantidad máxima" << endl;
        exit(1);
    }
    return min(fide, maxFi);
}

// Clase que representa a un filósofo
class Filo {
public:
    Filo(int id, int& fide, mutex& mutexFide) : id(id), fide(fide), mutexFide(mutexFide), estmag(0), timPensa(0) {}

// Sobrecarga del operador de llamada para utilizar el objeto como función
    void operator()() {
        while (true) {
            pensa();
            tomarTenedores();
            come();
            dejarTenedores();
            if (fide <= 0) {
                break;
            }
        }
    }

    // Método para obtener el valor actual del estómago
    int getEstmag() const {
        return estmag;
    }

    // Método para obtener el tiempo total de pensamiento
    int getTimPensa() const {
        return timPensa;
    }

public:
    // El filósofo está pensando
    void pensa() {
        int timp = getTimAlea();
        cout << "Filosofo N°" << id << " está pensando (" << timp << "s)" << endl;
        this_thread::sleep_for(chrono::seconds(timp));
        timPensa += timp;
    }

    // El filósofo toma los tenedores
    void tomarTenedores() {
        int tenedorIzq = id - 1;
        int tenedorDer = id % 5;

        // Si el número del tenedor izquierdo es mayor al del derecho, se intercambian
        if (tenedorIzq > tenedorDer) {
            swap(tenedorIzq, tenedorDer);
        }

	// Accedemos a los mutex de los tenedores
        mutex& mutexTenedorIzq = tenedores[tenedorIzq];
        mutex& mutexTenedorDer = tenedores[tenedorDer];

	// Bloqueamos los mutex de los tenedores
        unique_lock<mutex> lockIzq(mutexTenedorIzq, defer_lock);
        unique_lock<mutex> lockDer(mutexTenedorDer, defer_lock);

        lock(lockIzq, lockDer);

        cout << "Filosofo N°" << id << " ha tomado los tenedores " << tenedorIzq + 1 << " y " << tenedorDer + 1 << endl;
    }

    // El filósofo está comiendo
    void come() {
        lock_guard<mutex> lock(mutexFide);

	// Si aún hay fideos disponibles
        if (fide > 0) {
            int porci = getTimAlea();

	    // Limitamos la cantidad de fideos a la cantidad disponible
            porci = min(porci, fide);
            fide -= porci;
            estmag += porci;
            cout << "Filosofo N° " << id << " está comiendo (" << porci << ")" << endl;
            cout << "Filosofo N° " << id << " incrementó el valor del estómago (" << estmag - porci << " + " << porci << " = " << estmag << ")" << endl;
        }
    }

    // El filósofo deja los tenedores
    void dejarTenedores() {
        int tenedorIzq = id - 1;
        int tenedorDer = id % 5;

	// Si el número del tenedor izquierdo es mayor al del derecho, se intercambian
        if (tenedorIzq > tenedorDer) {
            swap(tenedorIzq, tenedorDer);
        }

	// Accedemos a los mutex de los tenedores
        mutex& mutexTenedorIzq = tenedores[tenedorIzq];
        mutex& mutexTenedorDer = tenedores[tenedorDer];

	// Desbloqueamos los mutex de los tenedores
        mutexTenedorIzq.unlock();
        mutexTenedorDer.unlock();

        cout << "Filosofo N°" << id << " ha dejado los tenedores " << tenedorIzq + 1 << " y " << tenedorDer + 1 << endl;
    }

    // Método para generar un tiempo aleatorio de pensamiento
    int getTimAlea() const {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<int> dist(1, 5);
        return dist(gen);
    }

    int id;
    int& fide;
    mutex& mutexFide;
    int estmag;
    int timPensa;
    static vector<mutex> tenedores;
};

vector<mutex> Filo::tenedores(5);

int main() {
    int fide;
    int numFilos;

    cout << "\n--------SIMULADOR DE FILOSOFOS COMENSALES--------" << endl;
    cout << "\nBienvenido Usuario, Escriba los datos pedidos:" << endl;
    cout << "\n(Recuerda que el máximo de fideos es 2,147,483,647)" << endl;
    cout << "\n- Cuantos Fideos Pondras: ";
    cin >> fide;
    fide = LimitFideMax(fide);
    cout << "\n- Escriba la cantidad de filósofos que habrá: ";
    cin >> numFilos;
    cout << "\n-------------------------------------------" << endl;
    cout << "\nEjecutando procedimiento......." << endl;
    cout << "\n-------------------------------------------" << endl;

    mutex mutexFide;

    vector<Filo> filos;
    filos.reserve(numFilos);
    vector<thread> hilos;

    // Creación de los filósofos y los hilos de ejecución
    for (int i = 0; i < numFilos; ++i) {
        filos.emplace_back(i + 1, fide, mutexFide);
        hilos.emplace_back(ref(filos[i]));
    }

    // Espera a que todos los hilos terminen su ejecución
    for (int i = 0; i < numFilos; ++i) {
        hilos[i].join();
    }

    cout << "\n-------------------------------------------" << endl;
    cout << "\n....La ejecución del programa ha terminado...." << endl;
    cout << "\nMostrando datos finales........" << endl;
    cout << "\n---------------Estadísticas:----------------\n" << endl;
    int totalEstmag = 0;
    int totalTimPensa = 0;
    for (int i = 0; i < numFilos; ++i) {
        cout << "+ Filosofos N°(" << filos[i].id << ") :" << endl;
        cout << "\t* Tiempo pensando: " << filos[i].getTimPensa() << "s" << endl;
        cout << "\t* Estómago: " << filos[i].getEstmag() << endl;
        totalEstmag += filos[i].getEstmag();
        totalTimPensa += filos[i].getTimPensa();
        cout << endl;
    }
    cout << "\n---------------Datos finales---------------\n" << endl;
    cout << "1.- Total de comida consumida por los filósofos: " << totalEstmag << endl;
    cout << "2.- Fideos que restan: " << fide << endl;
    cout << "3.- Total de tiempo pensando: " << totalTimPensa << "s" << endl;
    cout << "\n-------------------------------------------" << endl;
    cout << "\t\t\n-----FIN DE LA EJECUCIÓN------" << endl;
    return 0;
}