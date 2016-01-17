#include <stdio.h>
#include <stdlib.h>

#define CYCLES 3
#define BUFFERSIZE 10
void producer();
void consumer();

int main(int argc, char const *argv[]) {
	int *buffer = malloc(BUFFERSIZE * sizeof(int));
	for(int i=0; i<CYCLES; i++) {
		producer(buffer, BUFFERSIZE, 0);
		consumer(buffer, BUFFERSIZE, BUFFERSIZE);
		printf("Finished cycle: %d\n\n", i+1);
	}
	free(buffer);
	printf("DONE\n");
	return 0;
}

void producer(int *buffer, int bufferSize) {
	int item;
	for(int i=0; i<bufferSize; i++) {
		item = 1;	//produce item
		buffer[i] = item; 	//insert item to buffer
		printf("Produced item at index: %d\n", i);
	}
	printf("Producer finished\n");
}

void consumer(int *buffer, int bufferSize) {
	for(int i=0; i<bufferSize; i++) {
		buffer[i] = NULL;	//consume item
		printf("Consumed item at index: %d\n", i);
	}
	printf("Consumer finished\n");
}