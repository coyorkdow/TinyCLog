USER_LOGFILE_DIR = testlog.log

CC = gcc
CFLAGS = -Wall -O2
CFLAGS += -DUSER_LOGFILE_DIR=\"$(USER_LOGFILE_DIR)\"

example: example.o
	$(CC) $(CFLAGS) -o example example.o

example.o: log_posix.h example.c

clean:
	rm -f *~ *.o example