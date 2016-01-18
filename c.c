#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define BUFFERSIZE 10
#define CYCLES 100
pthread_mutex_t mutex;			//declare mutex mutex
pthread_cond_t condProducer;	//declare condition condProducer
pthread_cond_t condConsumer;	//declare condition condConsumer

void *producer(void *b);
void *consumer(void *b);
void produce(int *buffer, int i);
void consume(int *buffer, int i);

int main(int argc, char const *argv[]){
	pthread_t producerThread, consumerThread;
	int *buffer = malloc(BUFFERSIZE * sizeof(int));
	for(int i=0; i<BUFFERSIZE; i++) {
		buffer[i] = 0;	//set buffer slots as "empty"
	}
	pthread_mutex_init(&mutex, NULL);		//initialize mutex
	pthread_cond_init(&condProducer, NULL);	//initialize condProducer
	pthread_cond_init(&condConsumer, NULL);	//initialize condConsumer

	pthread_create(&producerThread, NULL, producer, buffer);
	pthread_create(&consumerThread, NULL, consumer, buffer);
	pthread_join(producerThread, NULL);
	pthread_join(consumerThread, NULL);
	
	pthread_cond_destroy(&condProducer);	//destroy condProducer
	pthread_cond_destroy(&condConsumer);	//destroy condConsumer
	pthread_mutex_destroy(&mutex);			//destroy mutex
	free(buffer);
	printf("DONE\n");
	return 0;
}

void *producer(void *b) {
	printf("PRODUCER THREAD\n");
	int *buffer = b;
	int i=0;
	int cyclesCompleted = 0;
	while(cyclesCompleted<CYCLES) {
		pthread_mutex_lock(&mutex);	//ensures exclusive access to buffer
		while(buffer[i] == 1) pthread_cond_wait(&condProducer, &mutex);	//wait until consumer has emptied slot i, unlock mutex while waiting
		produce(buffer, i);	//insert item to buffer
		pthread_cond_signal(&condConsumer);	//wake up consumer if it was waiting
		pthread_mutex_unlock(&mutex);	//release buffer access
		i = (i+1)%BUFFERSIZE;
		if(i==0)
			cyclesCompleted++;
	}
	pthread_exit(0);
}
void *consumer(void *b) {
	printf("CONSUMER THREAD\n");
	int *buffer = b;
	int i = 0;
	int cyclesCompleted = 0;
	while(cyclesCompleted<CYCLES) {
		pthread_mutex_lock(&mutex);	//ensures exclusive access to buffer
		while(buffer[i] == 0) pthread_cond_wait(&condConsumer, &mutex);	//wait until producer has filled slot i, unlock mutex while waiting
		consume(buffer, i); //consume item from buffer
		pthread_cond_signal(&condProducer);	//wake up producer if it was waiting
		pthread_mutex_unlock(&mutex);	//release buffer access
		i = (i+1)%BUFFERSIZE;
		if(i==0)
			cyclesCompleted++;
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
	if(i==BUFFERSIZE-1)
		printf("Producer finished cycle\n");
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
	if(i==BUFFERSIZE-1)
		printf("Consumer finished cycle\n");
}