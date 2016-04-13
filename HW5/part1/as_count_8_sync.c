#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>

#define BUFFER_SIZE 67108864
#define THREADS_NUM 8

char char_buffer[BUFFER_SIZE];
int ascii_num[128];
pthread_mutex_t lock;	

typedef struct thread_class{
	int thread_id;
	int partition_bounds;
} thread_info;

void *ascii_counting (void* thread){
	char current_char;
	int t,thread_begin_location;

	thread_info *td= (thread_info*) thread;

	t = td->thread_id;
	thread_begin_location= t * td->partition_bounds;
	// printf("I'm now in thread %d\n",t);

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

int main(int argc, char const *argv[]){
	
	int fd, length_per_thread;
	pthread_t threads[THREADS_NUM];
	thread_info t_info[THREADS_NUM];
	pthread_attr_t thread_attr;
	ssize_t bytes_read =0;

	if(argc !=2){
		printf("Incorrect cmd format, please check!");
		exit (1);
	}

	fd=open(argv[1],O_RDONLY);
	if (fd<0)
	{
		perror("Open file fail: ");
		exit(1);
	}
	
	pthread_attr_init(&thread_attr);
	 if(pthread_mutex_init(&lock,NULL)!=0)
	 {
	 	printf("\n mutex init failed \n");
	 	exit(1);
	 }


	for (;;)
	{
		bytes_read=read(fd,char_buffer,BUFFER_SIZE);
		if(bytes_read == 0){
			close(fd);
			break;
		}

		if (bytes_read == -1)
		{
			perror("Reading fail");
			exit(1);
		}

		length_per_thread=(double)bytes_read/(double)THREADS_NUM;

		for (int i = 0; i < THREADS_NUM; ++i)
		{
			t_info[i].thread_id = i;
			t_info[i].partition_bounds=length_per_thread;

			pthread_create(&threads[i], &thread_attr, ascii_counting, &t_info[i]);
		}

		for (int i = 0; i < THREADS_NUM; ++i)
		{
			pthread_join(threads[i],NULL);
		}
	}
/*End of read & multi_thread counting*/

/*Get the total*/
	for (int i = 0; i < 128; ++i)
	{
		printf("%i occurrences of ", ascii_num[i] );

		if (i < 33 || i == 127){
			printf("%#0x\n", i );
		}else{
			printf("%c\n", i);
		}
	}

	return 0;
}







