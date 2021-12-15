USER_LOGFILE_LOCATION = example.log
USER_LOG_LEVEL = DEBUG

CC = gcc
CFLAGS = -Wall -O2 -I..
LOG_FLAGS = -DTINY_C_LOG_POSIX_IMPL -DUSER_LOGFILE_DIR=\"$(USER_LOGFILE_LOCATION)\" -DUSER_LOG_LEVEL=DEBUG 

example: example.o mutilthread.o
	$(CC) -pthread $(CFLAGS) -o example example.o mutilthread.o

example.o: example.c
	$(CC) $(CFLAGS) $(LOG_FLAGS) -c example.c

mutilthread.o: mutilthread.c

clean:
	rm -f *~ *.o *.gch example