# CSC415 HW5
BY Cheng Li 916422876

### Part1
Update the code for homework 4 to use a one-dimensional global array to hold all of the character counts, so that all the threads increment the same array entries without using any synchronization. Then create a second version that protects the increment of the global array using a mutex. Run both versions on the same file and compare the character counts. Which are larger? Why? 

####*Race condition without mutex：*
```c
void *ascii_counting (void* thread){
    char current_char;
    int t,thread_begin_location;
    thread_info *td= (thread_info*) thread;

    t = td->thread_id;
    thread_begin_location= t * td->partition_bounds;
   
    for (int i = 0; i < td->partition_bounds; ++i)
    {
        /* code */
        current_char = char_buffer[thread_begin_location+i];
        ascii_num[(int)current_char]++;
    }
    pthread_exit(0);
}
```
####*Sync condition with mutex:*
```c
void *ascii_counting (void* thread){
    char current_char;
    int t,thread_begin_location;

    thread_info *td= (thread_info*) thread;

    t = td->thread_id;
    thread_begin_location= t * td->partition_bounds;
    for (int i = 0; i < td->partition_bounds; ++i)
    {
        /* code */
        current_char = char_buffer[thread_begin_location+i];
        pthread_mutex_lock(&lock);
        ascii_num[(int)current_char]++;
        pthread_mutex_unlock(&lock);
    }
    pthread_exit(0);
}
```

####*Result comparision*
```
Race condition                       Sync condition                HW4 result   
506550 occurrences of 0              523510 occurrences of 0       523510 occurrences of 0
505743 occurrences of 0x1            524012 occurrences of 0x1     524012 occurrences of 0x1
505458 occurrences of 0x2            524743 occurrences of 0x2     524743 occurrences of 0x2
506301 occurrences of 0x3            524402 occurrences of 0x3     524402 occurrences of 0x3
509364 occurrences of 0x4            524016 occurrences of 0x4     524016 occurrences of 0x4
505830 occurrences of 0x5            524122 occurrences of 0x5     524122 occurrences of 0x5
501179 occurrences of 0x6            525724 occurrences of 0x6     525724 occurrences of 0x6
507930 occurrences of 0x7            523666 occurrences of 0x7     523666 occurrences of 0x7
502330 occurrences of 0x8            525189 occurrences of 0x8     525189 occurrences of 0x8
495590 occurrences of 0x9            523542 occurrences of 0x9     523542 occurrences of 0x9
508890 occurrences of 0xa            524596 occurrences of 0xa     524596 occurrences of 0xa
510811 occurrences of 0xb            523862 occurrences of 0xb     523862 occurrences of 0xb
511822 occurrences of 0xc            524856 occurrences of 0xc     524856 occurrences of 0xc
512844 occurrences of 0xd            524868 occurrences of 0xd     524868 occurrences of 0xd
```
Race condition will make the result smaller, because race condition will lose the update of the mutual accessed buffer, making some counting result lost.This will make the final count smaller.

### Part2
####*Consumer producer problem*

- Use condition variable `ready_to_read` and `ready_to_write` and mutex lock.
- Buffer is a cycle array, with size of 16.
- The number of producers, the number of consumers, and the number of items each producer produces should be specified by their binary log as command-line parameters. In the following example, there are 2 producers, with each produce 16 items, and 4 consumers,each consuming 16*2/4=8 items.

        ./run 1 2 4

- Producer produce integer numbers, from **thread_number * num_produced** to **thread_number * num_produced+ total_to_be_produced**
- Consumer consume the numbers by simply print them out.

Code is shown as an appendix.
####*Compilation*
    cc -pthread -Wall -std=c99 consumer_producer.c -o run -lm

####*Test case*
1 producer + 2 consumer + 32 production each: `./run 0 1 5`
```
1 producers, each produce 32 items.
2 consumers, each consume 16 items.
#0 producer is producing 0! bufferlength is 1 
#0 producer is producing 1! bufferlength is 2 
#0 producer is producing 2! bufferlength is 3 
#0 producer is producing 3! bufferlength is 4 
#0 producer is producing 4! bufferlength is 5 
#0 producer is producing 5! bufferlength is 6 
#0 producer is producing 6! bufferlength is 7 
#0 producer is producing 7! bufferlength is 8 
#0 producer is producing 8! bufferlength is 9 
#0 producer is producing 9! bufferlength is 10 
#0 producer is producing 10! bufferlength is 11 
#0 producer is producing 11! bufferlength is 12 
#0 producer is producing 12! bufferlength is 13 
#0 producer is producing 13! bufferlength is 14 
#0 producer is producing 14! bufferlength is 15 
#0 producer is producing 15! bufferlength is 16 
#0 consumer is consuming 0! bufferlength is 15 
#0 consumer is consuming 1! bufferlength is 14 
#0 consumer is consuming 2! bufferlength is 13 
#0 consumer is consuming 3! bufferlength is 12 
#0 consumer is consuming 4! bufferlength is 11 
#0 consumer is consuming 5! bufferlength is 10 
#0 consumer is consuming 6! bufferlength is 9 
#0 consumer is consuming 7! bufferlength is 8 
#0 consumer is consuming 8! bufferlength is 7 
#0 consumer is consuming 9! bufferlength is 6 
#0 consumer is consuming 10! bufferlength is 5 
#0 consumer is consuming 11! bufferlength is 4 
#1 consumer is consuming 12! bufferlength is 3 
#1 consumer is consuming 13! bufferlength is 2 
#1 consumer is consuming 14! bufferlength is 1 
#1 consumer is consuming 15! bufferlength is 0 
#0 producer is producing 16! bufferlength is 1 
#0 producer is producing 17! bufferlength is 2 
#0 producer is producing 18! bufferlength is 3 
#0 producer is producing 19! bufferlength is 4 
#0 producer is producing 20! bufferlength is 5 
#0 producer is producing 21! bufferlength is 6 
#0 producer is producing 22! bufferlength is 7 
#0 producer is producing 23! bufferlength is 8 
#0 producer is producing 24! bufferlength is 9 
#0 producer is producing 25! bufferlength is 10 
#0 producer is producing 26! bufferlength is 11 
#0 producer is producing 27! bufferlength is 12 
#0 producer is producing 28! bufferlength is 13 
#0 producer is producing 29! bufferlength is 14 
#0 producer is producing 30! bufferlength is 15 
#0 producer is producing 31! bufferlength is 16 

#1 consumer is consuming 16! bufferlength is 15 
#1 consumer is consuming 17! bufferlength is 14 
#1 consumer is consuming 18! bufferlength is 13 
#1 consumer is consuming 19! bufferlength is 12 
#1 consumer is consuming 20! bufferlength is 11 
#1 consumer is consuming 21! bufferlength is 10 
#1 consumer is consuming 22! bufferlength is 9 
#1 consumer is consuming 23! bufferlength is 8 
#1 consumer is consuming 24! bufferlength is 7 
#1 consumer is consuming 25! bufferlength is 6 
#1 consumer is consuming 26! bufferlength is 5 
#1 consumer is consuming 27! bufferlength is 4 
#1 consumer done!

#0 consumer is consuming 28! bufferlength is 3 
#0 consumer is consuming 29! bufferlength is 2 
#0 consumer is consuming 30! bufferlength is 1 
#0 consumer is consuming 31! bufferlength is 0 
#0 consumer done!

Program finished! 
```
2 producer + 1 consumer + 32 production each: `./run 1 0 5`
```
2 producers, each produce 32 items.
1 consumers, each consume 64 items.
#1 producer is producing 32! bufferlength is 1 
#0 producer is producing 0! bufferlength is 2 
#0 producer is producing 1! bufferlength is 3 
#0 producer is producing 2! bufferlength is 4 
#0 producer is producing 3! bufferlength is 5 
#0 producer is producing 4! bufferlength is 6 
#0 producer is producing 5! bufferlength is 7 
#0 producer is producing 6! bufferlength is 8 
#0 producer is producing 7! bufferlength is 9 
#0 producer is producing 8! bufferlength is 10 
#0 producer is producing 9! bufferlength is 11 
#0 producer is producing 10! bufferlength is 12 
#0 producer is producing 11! bufferlength is 13 
#1 producer is producing 33! bufferlength is 14 
#1 producer is producing 34! bufferlength is 15 
#1 producer is producing 35! bufferlength is 16 
#0 consumer is consuming 32! bufferlength is 15 
#0 consumer is consuming 0! bufferlength is 14 
#0 consumer is consuming 1! bufferlength is 13 
#0 consumer is consuming 2! bufferlength is 12 
#0 consumer is consuming 3! bufferlength is 11 
#0 consumer is consuming 4! bufferlength is 10 
#0 consumer is consuming 5! bufferlength is 9 
#0 consumer is consuming 6! bufferlength is 8 
#0 consumer is consuming 7! bufferlength is 7 
#0 consumer is consuming 8! bufferlength is 6 
#0 consumer is consuming 9! bufferlength is 5 
#0 consumer is consuming 10! bufferlength is 4 
#0 consumer is consuming 11! bufferlength is 3 
#0 consumer is consuming 33! bufferlength is 2 
#0 consumer is consuming 34! bufferlength is 1 
#0 consumer is consuming 35! bufferlength is 0 
#1 producer is producing 36! bufferlength is 1 
#1 producer is producing 37! bufferlength is 2 
#1 producer is producing 38! bufferlength is 3 
#1 producer is producing 39! bufferlength is 4 
#1 producer is producing 40! bufferlength is 5 
#1 producer is producing 41! bufferlength is 6 
#1 producer is producing 42! bufferlength is 7 
#1 producer is producing 43! bufferlength is 8 
#1 producer is producing 44! bufferlength is 9 
#1 producer is producing 45! bufferlength is 10 
#1 producer is producing 46! bufferlength is 11 
#1 producer is producing 47! bufferlength is 12 
#1 producer is producing 48! bufferlength is 13 
#0 consumer is consuming 36! bufferlength is 12 
#0 producer is producing 12! bufferlength is 13 
#0 producer is producing 13! bufferlength is 14 
#0 producer is producing 14! bufferlength is 15 
#0 producer is producing 15! bufferlength is 16 
#0 consumer is consuming 37! bufferlength is 15 
#0 producer is producing 16! bufferlength is 16 
#0 consumer is consuming 38! bufferlength is 15 
#1 producer is producing 49! bufferlength is 16 
#0 consumer is consuming 39! bufferlength is 15 
#0 producer is producing 17! bufferlength is 16 
#0 consumer is consuming 40! bufferlength is 15 
#1 producer is producing 50! bufferlength is 16 
#0 consumer is consuming 41! bufferlength is 15 
#0 producer is producing 18! bufferlength is 16 
#0 consumer is consuming 42! bufferlength is 15 
#1 producer is producing 51! bufferlength is 16 
#0 consumer is consuming 43! bufferlength is 15 
#0 producer is producing 19! bufferlength is 16 
#0 consumer is consuming 44! bufferlength is 15 
#1 producer is producing 52! bufferlength is 16 
#0 consumer is consuming 45! bufferlength is 15 
#0 producer is producing 20! bufferlength is 16 
#0 consumer is consuming 46! bufferlength is 15 
#1 producer is producing 53! bufferlength is 16 
#0 consumer is consuming 47! bufferlength is 15 
#0 producer is producing 21! bufferlength is 16 
#0 consumer is consuming 48! bufferlength is 15 
#1 producer is producing 54! bufferlength is 16 
#0 consumer is consuming 12! bufferlength is 15 
#0 producer is producing 22! bufferlength is 16 
#0 consumer is consuming 13! bufferlength is 15 
#1 producer is producing 55! bufferlength is 16 
#0 consumer is consuming 14! bufferlength is 15 
#0 producer is producing 23! bufferlength is 16 
#0 consumer is consuming 15! bufferlength is 15 
#1 producer is producing 56! bufferlength is 16 
#0 consumer is consuming 16! bufferlength is 15 
#0 producer is producing 24! bufferlength is 16 
#0 consumer is consuming 49! bufferlength is 15 
#0 consumer is consuming 17! bufferlength is 14 
#0 consumer is consuming 50! bufferlength is 13 
#0 consumer is consuming 18! bufferlength is 12 
#0 consumer is consuming 51! bufferlength is 11 
#0 consumer is consuming 19! bufferlength is 10 
#0 consumer is consuming 52! bufferlength is 9 
#0 consumer is consuming 20! bufferlength is 8 
#0 consumer is consuming 53! bufferlength is 7 
#0 consumer is consuming 21! bufferlength is 6 
#0 consumer is consuming 54! bufferlength is 5 
#0 consumer is consuming 22! bufferlength is 4 
#0 consumer is consuming 55! bufferlength is 3 
#0 consumer is consuming 23! bufferlength is 2 
#0 consumer is consuming 56! bufferlength is 1 
#0 consumer is consuming 24! bufferlength is 0 
#1 producer is producing 57! bufferlength is 1 
#1 producer is producing 58! bufferlength is 2 
#1 producer is producing 59! bufferlength is 3 
#1 producer is producing 60! bufferlength is 4 
#1 producer is producing 61! bufferlength is 5 
#1 producer is producing 62! bufferlength is 6 
#1 producer is producing 63! bufferlength is 7 

#0 consumer is consuming 57! bufferlength is 6 
#0 consumer is consuming 58! bufferlength is 5 
#0 consumer is consuming 59! bufferlength is 4 
#0 consumer is consuming 60! bufferlength is 3 
#0 consumer is consuming 61! bufferlength is 2 
#0 consumer is consuming 62! bufferlength is 1 
#0 consumer is consuming 63! bufferlength is 0 
#0 producer is producing 25! bufferlength is 1 
#0 producer is producing 26! bufferlength is 2 
#0 producer is producing 27! bufferlength is 3 
#0 producer is producing 28! bufferlength is 4 
#0 producer is producing 29! bufferlength is 5 
#0 producer is producing 30! bufferlength is 6 
#0 producer is producing 31! bufferlength is 7 

#0 consumer is consuming 25! bufferlength is 6 
#0 consumer is consuming 26! bufferlength is 5 
#0 consumer is consuming 27! bufferlength is 4 
#0 consumer is consuming 28! bufferlength is 3 
#0 consumer is consuming 29! bufferlength is 2 
#0 consumer is consuming 30! bufferlength is 1 
#0 consumer is consuming 31! bufferlength is 0 
#0 consumer done!

Program finished! 

```

2 producer + 4 consumer + 32 production each: `./run 1 2 5`
```
2 producers, each produce 32 items.
4 consumers, each consume 16 items.
#0 producer is producing 0! bufferlength is 1 
#0 producer is producing 1! bufferlength is 2 
#0 producer is producing 2! bufferlength is 3 
#0 producer is producing 3! bufferlength is 4 
#0 producer is producing 4! bufferlength is 5 
#0 producer is producing 5! bufferlength is 6 
#0 producer is producing 6! bufferlength is 7 
#0 producer is producing 7! bufferlength is 8 
#0 producer is producing 8! bufferlength is 9 
#0 producer is producing 9! bufferlength is 10 
#0 producer is producing 10! bufferlength is 11 
#0 producer is producing 11! bufferlength is 12 
#0 producer is producing 12! bufferlength is 13 
#0 producer is producing 13! bufferlength is 14 
#0 producer is producing 14! bufferlength is 15 
#0 producer is producing 15! bufferlength is 16 
#0 consumer is consuming 0! bufferlength is 15 
#0 consumer is consuming 1! bufferlength is 14 
#0 consumer is consuming 2! bufferlength is 13 
#0 consumer is consuming 3! bufferlength is 12 
#0 consumer is consuming 4! bufferlength is 11 
#0 consumer is consuming 5! bufferlength is 10 
#0 consumer is consuming 6! bufferlength is 9 
#0 consumer is consuming 7! bufferlength is 8 
#0 consumer is consuming 8! bufferlength is 7 
#0 consumer is consuming 9! bufferlength is 6 
#0 consumer is consuming 10! bufferlength is 5 
#0 consumer is consuming 11! bufferlength is 4 
#0 consumer is consuming 12! bufferlength is 3 
#0 consumer is consuming 13! bufferlength is 2 
#0 consumer is consuming 14! bufferlength is 1 
#0 consumer is consuming 15! bufferlength is 0 
#0 consumer done!

#1 producer is producing 32! bufferlength is 1 
#1 producer is producing 33! bufferlength is 2 
#1 producer is producing 34! bufferlength is 3 
#1 producer is producing 35! bufferlength is 4 
#1 producer is producing 36! bufferlength is 5 
#1 producer is producing 37! bufferlength is 6 
#1 producer is producing 38! bufferlength is 7 
#1 producer is producing 39! bufferlength is 8 
#1 producer is producing 40! bufferlength is 9 
#1 producer is producing 41! bufferlength is 10 
#1 producer is producing 42! bufferlength is 11 
#1 producer is producing 43! bufferlength is 12 
#1 producer is producing 44! bufferlength is 13 
#1 producer is producing 45! bufferlength is 14 
#1 producer is producing 46! bufferlength is 15 
#1 producer is producing 47! bufferlength is 16 
#1 consumer is consuming 32! bufferlength is 15 
#1 consumer is consuming 33! bufferlength is 14 
#1 consumer is consuming 34! bufferlength is 13 
#1 consumer is consuming 35! bufferlength is 12 
#1 consumer is consuming 36! bufferlength is 11 
#1 consumer is consuming 37! bufferlength is 10 
#1 consumer is consuming 38! bufferlength is 9 
#1 consumer is consuming 39! bufferlength is 8 
#1 consumer is consuming 40! bufferlength is 7 
#1 consumer is consuming 41! bufferlength is 6 
#1 consumer is consuming 42! bufferlength is 5 
#1 consumer is consuming 43! bufferlength is 4 
#1 consumer is consuming 44! bufferlength is 3 
#1 consumer is consuming 45! bufferlength is 2 
#1 consumer is consuming 46! bufferlength is 1 
#1 consumer is consuming 47! bufferlength is 0 
#1 consumer done!

#1 producer is producing 48! bufferlength is 1 
#1 producer is producing 49! bufferlength is 2 
#1 producer is producing 50! bufferlength is 3 
#1 producer is producing 51! bufferlength is 4 
#1 producer is producing 52! bufferlength is 5 
#1 producer is producing 53! bufferlength is 6 
#1 producer is producing 54! bufferlength is 7 
#1 producer is producing 55! bufferlength is 8 
#1 producer is producing 56! bufferlength is 9 
#1 producer is producing 57! bufferlength is 10 
#1 producer is producing 58! bufferlength is 11 
#1 producer is producing 59! bufferlength is 12 
#1 producer is producing 60! bufferlength is 13 
#1 producer is producing 61! bufferlength is 14 
#1 producer is producing 62! bufferlength is 15 
#1 producer is producing 63! bufferlength is 16 

#2 consumer is consuming 48! bufferlength is 15 
#2 consumer is consuming 49! bufferlength is 14 
#2 consumer is consuming 50! bufferlength is 13 
#2 consumer is consuming 51! bufferlength is 12 
#2 consumer is consuming 52! bufferlength is 11 
#2 consumer is consuming 53! bufferlength is 10 
#2 consumer is consuming 54! bufferlength is 9 
#2 consumer is consuming 55! bufferlength is 8 
#2 consumer is consuming 56! bufferlength is 7 
#2 consumer is consuming 57! bufferlength is 6 
#2 consumer is consuming 58! bufferlength is 5 
#2 consumer is consuming 59! bufferlength is 4 
#2 consumer is consuming 60! bufferlength is 3 
#2 consumer is consuming 61! bufferlength is 2 
#2 consumer is consuming 62! bufferlength is 1 
#2 consumer is consuming 63! bufferlength is 0 
#2 consumer done!

#0 producer is producing 16! bufferlength is 1 
#0 producer is producing 17! bufferlength is 2 
#0 producer is producing 18! bufferlength is 3 
#0 producer is producing 19! bufferlength is 4 
#0 producer is producing 20! bufferlength is 5 
#0 producer is producing 21! bufferlength is 6 
#0 producer is producing 22! bufferlength is 7 
#0 producer is producing 23! bufferlength is 8 
#0 producer is producing 24! bufferlength is 9 
#0 producer is producing 25! bufferlength is 10 
#0 producer is producing 26! bufferlength is 11 
#0 producer is producing 27! bufferlength is 12 
#0 producer is producing 28! bufferlength is 13 
#0 producer is producing 29! bufferlength is 14 
#0 producer is producing 30! bufferlength is 15 
#0 producer is producing 31! bufferlength is 16 

#3 consumer is consuming 16! bufferlength is 15 
#3 consumer is consuming 17! bufferlength is 14 
#3 consumer is consuming 18! bufferlength is 13 
#3 consumer is consuming 19! bufferlength is 12 
#3 consumer is consuming 20! bufferlength is 11 
#3 consumer is consuming 21! bufferlength is 10 
#3 consumer is consuming 22! bufferlength is 9 
#3 consumer is consuming 23! bufferlength is 8 
#3 consumer is consuming 24! bufferlength is 7 
#3 consumer is consuming 25! bufferlength is 6 
#3 consumer is consuming 26! bufferlength is 5 
#3 consumer is consuming 27! bufferlength is 4 
#3 consumer is consuming 28! bufferlength is 3 
#3 consumer is consuming 29! bufferlength is 2 
#3 consumer is consuming 30! bufferlength is 1 
#3 consumer is consuming 31! bufferlength is 0 
#3 consumer done!

Program finished! 

```

####Appendix ####
```c
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
    int capacity = producer->producer_capability;
    int id = producer -> producer_id;

    while(count < capacity){
        //Produce
        pthread_mutex_lock(&lock);
        while (actual_length >= BUFFER_SIZE)
            pthread_cond_wait(&ready_to_write, &lock); // wait on lenght >= Buffersize

        buffer[(head_index + actual_length)%BUFFER_SIZE]= id*capacity + count;  
        printf("#%d producer is producing %d! ", id, id*capacity + count);  
        count++;
        actual_length++;
        printf("bufferlength is %d \n", actual_length);  

        pthread_mutex_unlock(&lock);
        pthread_cond_signal(&ready_to_read);
    }
    printf("\n");
    return (NULL);
}

void *consume (void* cm)
{
    int count=0;
    consumers *consumer = (consumers*) cm;
    int capacity = consumer->consumer_capability;
    int id = consumer -> consumer_id;

    while(count < capacity){
        //Consume
        pthread_mutex_lock(&lock);
        while (actual_length <= 0)
            pthread_cond_wait(&ready_to_read, &lock); // wait on lenght >= Buffersize

        printf("#%d consumer is consuming %d! ", id, buffer[head_index]);  
        count++;
        actual_length--;
        head_index=(head_index+1) % BUFFER_SIZE;
        printf("bufferlength is %d \n", actual_length);  

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

    printf("%d producers, each produce %d items.\n", num_producers,num_items);
    printf("%d consumers, each consume %d items.\n", num_consumers,num_items * num_producers / num_consumers);

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
        producer_info[i].producer_capability = num_items;
        error = pthread_create(&producer_thread[i], NULL, produce, &producer_info[i]);
        if (error) {
            errno = error;
            err(1, "pthread_produce_create");
        }
    }
    for (int i = 0; i < num_consumers; ++i)
    {
        consumer_info[i].consumer_id = i;
        consumer_info[i].consumer_capability = num_items * num_producers / num_consumers;
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
```


