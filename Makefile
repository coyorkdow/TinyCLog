USER_LOGFILE_LOCATION = example.log
USER_LOG_LEVEL = DEBUG

CC = gcc
CFLAGS = -Wall -O2
LOG_FLAGS = -DTINY_C_LOG_POSIX_IMPL -DUSER_LOGFILE_DIR=\"$(USER_LOGFILE_LOCATION)\" -DUSER_LOG_LEVEL=DEBUG 

example: example.o multithread.o
	$(CC) -pthread $(CFLAGS) -o example example.o multithread.o

example.o: example.c
	$(CC) $(CFLAGS) $(LOG_FLAGS) -c example.c

multithread: multithread.c

clean:
	rm -f *~ *.o *.gch example