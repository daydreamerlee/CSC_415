CC = cc -std=c99
CFLAGES = -Wall -pthread

all: run

run: consumer_producer.c
	$(CC) $(CFLAGES) consumer_producer.c -o run -lm

clean:
	rm -f *.o run