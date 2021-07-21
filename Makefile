USER_LOGFILE_LOCATION = example.log
USER_LOG_LEVEL = DEBUG

CC = gcc
CFLAGS = -Wall -O2
LOG_FLAGS = -DTINY_C_LOG_POSIX_IMPL -DUSER_LOGFILE_DIR=\"$(USER_LOGFILE_LOCATION)\" -DUSER_LOG_LEVEL=DEBUG 

example: example.o log_posix.h.gch
	$(CC) $(CFLAGS) -o example example.o log_posix.h.gch

example.o: example.c

log_posix.h.gch: log_posix.h
	$(CC) $(CFLAGS) $(LOG_FLAGS) -c log_posix.h

clean:
	rm -f *~ *.o *.gch example