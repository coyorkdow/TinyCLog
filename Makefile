USER_LOGFILE_DIR = testlog.log

CC = gcc
CFLAGS = -Wall -O2

example: example.o log_posix.h.gch
	$(CC) $(CFLAGS) -o example example.o log_posix.h.gch

example.o: example.c

log_posix.h.gch: log_posix.h
	$(CC) -DUSER_LOGFILE_DIR=\"$(USER_LOGFILE_DIR)\" -DTINY_C_LOG_LOG_IMPL -c log_posix.h

clean:
	rm -f *~ *.o *.gch example