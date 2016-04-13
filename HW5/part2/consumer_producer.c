#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <err.h>
#include <math.h>

#define BUFFER_SIZE 16
int buffer[BUFFER_SIZE];
int actual_length = 0;
int head_index=0;
pthread_mutex_t lock;
pthread_cond_t ready_to_read, ready_to_write;


typedef struct producer_class{
	int producer_id;
	int producer_capability;
} producers;

typedef struct consumer_class{
	int consumer_id;
	int consumer_capability;
} consumers;

void *produce (void* pd)
{
	int count=0;
	producers *producer = (producers*) pd;
	int	capacity = producer->producer_capability;
	int id = producer -> producer_id;

	while(count < capacity){
		//Produce
		pthread_mutex_lock(&lock);
		while (actual_length >= BUFFER_SIZE)
			pthread_cond_wait(&ready_to_write, &lock); // wait on lenght >= Buffersize

		buffer[(head_index + actual_length)%BUFFER_SIZE]= id*capacity + count;  
		printf("#%d producer is producing %d ", id, id*capacity + count);  
		count++;
		actual_length++;
		printf("Count is #%d, length is %d \n", count, actual_length);  

		pthread_mutex_unlock(&lock);
		pthread_cond_signal(&ready_to_read);
	}

	return (NULL);
}

void *consume (void* cm)
{
	int count=0;
	consumers *consumer = (consumers*) cm;
	int	capacity = consumer->consumer_capability;
	int id = consumer -> consumer_id;

	while(count < capacity){
		//Consume
		pthread_mutex_lock(&lock);
		while (actual_length <= 0)
			pthread_cond_wait(&ready_to_read, &lock); // wait on lenght >= Buffersize

		printf("#%d consumer is consuming %d\n", id, buffer[head_index]);  
		
		count++;
		actual_length--;
		head_index=(head_index+1) % BUFFER_SIZE;

		pthread_mutex_unlock(&lock);
		pthread_cond_signal(&ready_to_write);
	}
	printf("#%d consumer done!\n",id);
	printf("\n");
	return (NULL);
}

int main(int argc, char const *argv[])
{
	int num_producers,num_consumers,num_items;
	int argv1,argv2,argv3;
	int error;
	pthread_t *producer_thread;
	pthread_t *consumer_thread;
	producers *producer_info;
	consumers *consumer_info;

	if(argc!=4){
		printf("Usage uncorrect!\n");
		return (1);
	}

	argv1= atoi(argv[1]);
	argv2= atoi(argv[2]);
	argv3= atoi(argv[3]);

	num_producers = pow(2, argv1);
	num_consumers = pow(2, argv2);
	num_items = pow(2,argv3);

	producer_thread = (pthread_t *) malloc(num_producers * sizeof(pthread_t));
	consumer_thread = (pthread_t *) malloc(num_consumers * sizeof(pthread_t));
	producer_info = (producers*) malloc(num_producers * sizeof(producers));
	consumer_info = (consumers*) malloc(num_consumers * sizeof(consumers));

	error = pthread_mutex_init(&lock, NULL);
	if (error) {
		errno = error;
		err(1, "pthread_mutex_init");
	}
	error = pthread_cond_init(&ready_to_write, NULL);
	if (error) {
		errno = error;
		err(1, "pthread_cond_init(ready_to_write)");
	}
	error = pthread_cond_init(&ready_to_read, NULL);
	if (error) {
		errno = error;
		err(1, "pthread_cond_init(ready_to_read)");
	}

	for (int i = 0; i < num_producers; ++i)
	{
		producer_info[i].producer_id = i;
		producer_info[i].producer_capability = num_items / num_producers;
		error = pthread_create(&producer_thread[i], NULL, produce, &producer_info[i]);
		if (error) {
			errno = error;
			err(1, "pthread_produce_create");
		}
	}
	for (int i = 0; i < num_consumers; ++i)
	{
		consumer_info[i].consumer_id = i;
		consumer_info[i].consumer_capability = num_items / num_consumers;
		error = pthread_create(&consumer_thread[i], NULL, consume, &consumer_info[i]);
		if (error) {
			errno = error;
			err(1, "pthread_consume_create");
		}
	}
	
	for (int i = 0; i < num_producers; ++i)
	{
		pthread_join(producer_thread[i],NULL);
	}

	for (int i = 0; i < num_consumers; ++i)
	{
		pthread_join(consumer_thread[i],NULL);
	}	
	
	free(producer_thread);
	free(producer_info);
	free(consumer_thread);
	free(consumer_info);
	pthread_mutex_destroy(&lock);
	pthread_cond_destroy(&ready_to_read);
	pthread_cond_destroy(&ready_to_write);

	printf("Program finished! \n");
	return 0;
}