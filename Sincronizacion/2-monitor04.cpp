#include<iostream>
#include<thread>
#include<mutex>

using namespace std;

// g++ monitor04.cpp -lpthread

#define NUM_HILOS 10

int cont=0;
mutex flag;

class Monitor {
	public:
	void inc(){
		flag.lock();	// LOCK
		for(int i=0; i<10; i++){
			cont++;
		}
		cout<<cont<<endl;	// PRINT
		flag.unlock();	// UNLOCK		
	}
};

class Hilo {
	private:
		Monitor* monitor;
		thread t;
		void run_thread(){			
			monitor->inc();		//	RUN
		}
			
	public:
		Hilo(Monitor* mon){
			monitor = mon;
			t = thread(&Hilo::run_thread, this);
		}
		void join_thread(){
			t.join();	// START
		}
		
};

int main() {

	Hilo* hilos[NUM_HILOS];
	Monitor* mo;
	
	int i;
	for(i=0; i<NUM_HILOS; i++) {
		hilos[i] = new Hilo(mo);
	}
	
	for(i=0; i<NUM_HILOS; i++) {
		hilos[i]->join_thread();	//	START
	}

	return 0;
}







