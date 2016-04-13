CC = cc -std=c99
CFLAGES = -Wall -pthread

all: run

run: as_count_8_race.c
	$(CC) $(CFLAGES) as_count_8.c -o run

clean:
	rm -f *.o run