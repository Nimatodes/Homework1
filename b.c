#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define BUFFERSIZE 10
sem_t mutex;
sem_t empty;
sem_t full;

void *producer(void *b);
void *consumer(void *b);
void produce(int *buffer, int i);
void consume(int *buffer, int i);


int main(int argc, char const *argv[])
{
	pthread_t producerThread, consumerThread;
	int *buffer = malloc(BUFFERSIZE * sizeof(int));
	for(int i=0; i<BUFFERSIZE; i++) {
		buffer[i] = 0;	//set buffer slots as "empty"
	}
	sem_init(&mutex, 0, 1);
	sem_init(&empty, 0, BUFFERSIZE);
	sem_init(&full, 0, 0);

	pthread_create(&producerThread, NULL, producer, buffer);
	pthread_create(&consumerThread, NULL, consumer, buffer);
	pthread_join(producerThread, NULL);
	pthread_join(consumerThread, NULL);
	free(buffer);
	return 0;
}

void *producer(void *b) {
	printf("PRODUCER THREAD\n");
	int *buffer = b;
	int item;
	for(int i=0; i<BUFFERSIZE; i++) {
		item = 1;	//produce item
		sem_wait(&empty);	//down empty, decrement it
		sem_wait(&mutex);	//down mutex, enter critical region
		produce(buffer, i);	//insert item to buffer
		sem_post(&mutex);	//up mutex, exit critical region
		sem_post(&full);	//up full, increment it
	}
	pthread_exit(0);
}
void *consumer(void *b) {
	printf("CONSUMER THREAD\n");
	int *buffer = b;
	int item;
	for(int i=0; i<BUFFERSIZE; i++) {
		sem_wait(&full);	//down full, decrement it
		sem_wait(&mutex);	//down mutex, enter critical region
		consume(buffer, i); //consume item from buffer
		sem_post(&mutex);	//up mutex, exit critical region
		sem_post(&empty);	//up empty, increment it
	}
	pthread_exit(0);
}

void produce(int *buffer, int i) {
	if(buffer[i] == 0) {
			buffer[i] = 1; 	//insert item to buffer
	}
	else {			
		printf("Producer error at index: %d\n", i);
		exit(1);
	}
	printf("Produced item at index: %d\n", i);
}
void consume(int *buffer, int i) {
	if(buffer[i] == 1) {
			buffer[i] = 0;	//consume item from buffer
	}
	else {
		printf("Consumer error at index: %d\n", i);			
		exit(1);
	}
	printf("Consumed item at index: %d\n", i);
}