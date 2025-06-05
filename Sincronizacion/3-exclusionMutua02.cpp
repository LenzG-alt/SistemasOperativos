#include<stdio.h>
#include<pthread.h>
#include<assert.h>
// gcc exclusionMutua02.cpp -pthread

#define NUM_FILOSOFOS 5

void *filosofo (void *arg);

int variable = 0;
int tope = 10;

pthread_mutex_t flag; /* controlar la actualización de la variable (= PTHREAD_MUTEX_INITIALIZER;)*/

int main(void)
{
	pthread_t filosofos[NUM_FILOSOFOS];
	int identificadores[NUM_FILOSOFOS];

	int i;
	int result;

	printf ("Ingrese tope : ");
	scanf ("%d", &tope);

	printf ("variable = %d\n",variable);

	pthread_mutex_init(&flag, NULL);

	for (i=0; i<NUM_FILOSOFOS; i++)	{
		identificadores[i] = i;
		result = pthread_create (&filosofos[i], NULL, &filosofo, &identificadores[i]);
		assert(!result);
	}

	for (i=0; i<NUM_FILOSOFOS; i++)	{
		result = pthread_join(filosofos[i], NULL);
		assert(!result);
	}

	pthread_mutex_destroy(&flag);
	
	printf ("variable = %d\n",variable);

	return 0;
}

void *filosofo (void *arg) 
{
	int arg2 = *((int *)arg);
	for (int i=1; i<=tope; i++) {
		pthread_mutex_lock(&flag);
		variable = variable + 1;
		pthread_mutex_unlock(&flag);
		//printf ("Filósofo-%d estoy comiendo : %d. variable = %d \n", arg2, i, variable);
	}
	return NULL;
}

