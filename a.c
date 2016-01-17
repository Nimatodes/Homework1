#include <stdio.h>
#include <stdlib.h>

#define BUFFERSIZE 10
void producer();
void consumer();
void produce(int *buffer, int i);
void consume(int *buffer, int i);

int main(int argc, char const *argv[]) {
	int *buffer = malloc(BUFFERSIZE * sizeof(int));
	for(int i=0; i<BUFFERSIZE; i++) {
		buffer[i] = 0;	//set buffer slots as "empty"
	}
	producer(buffer, BUFFERSIZE, 0);
	consumer(buffer, BUFFERSIZE, BUFFERSIZE);
	free(buffer);
	printf("DONE\n");
	return 0;
}

void producer(int *buffer, int bufferSize) {
	for(int i=0; i<bufferSize; i++) {
		produce(buffer, i);	//insert item to buffer
	}
	printf("Producer finished\n");
}
void consumer(int *buffer, int bufferSize) {
	for(int i=0; i<bufferSize; i++) {
		consume(buffer, i);	//consume item from buffer
	}
	printf("Consumer finished\n");
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