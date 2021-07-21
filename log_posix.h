#ifndef TINY_C_LOG_POSIX__
#define TINY_C_LOG_POSIX__

#include <pthread.h>
#include <stdarg.h>
#include <stdio.h>
#include <sys/fcntl.h>
#include <time.h>
#include <unistd.h>

enum LogLevel {
  DEBUG = 0,
  INFO = 1,
  WARN = 2,
  ERROR = 3,
  FATAL = 4,
  DISABLE = 100
};

extern const enum LogLevel log_level;

extern char formatted_time[64];

void DoLog(const char *format, ...);

#define _LOG(level, format, ...)                                      \
  do {                                                                \
    if (level >= log_level) {                                         \
      DoLog("%s\t%s:%s:%d\t" #level "\t" format "\n", formatted_time, \
            __FILE__, __func__, __LINE__, ##__VA_ARGS__);             \
    }                                                                 \
  } while (0)

#define LOG_DEBUG(format, ...) _LOG(DEBUG, format, ##__VA_ARGS__)
#define LOG_INFO(format, ...) _LOG(INFO, format, ##__VA_ARGS__)
#define LOG_WARN(format, ...) _LOG(WARN, format, ##__VA_ARGS__)
#define LOG_ERROR(format, ...) _LOG(ERROR, format, ##__VA_ARGS__)
#define LOG_FATAL(format, ...) _LOG(FATAL, format, ##__VA_ARGS__)

#ifdef TINY_C_LOG_POSIX_IMPL

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int log_fd = -1;

#ifdef USER_LOG_LEVEL
const enum LogLevel log_level = USER_LOG_LEVEL;
#else
const enum LogLevel log_level = INFO;
#endif  // USER_LOG_LEVEL

char formatted_time[64];

long long file_suffix = 0;

void UpdateFile(int year, int mon, int day, int hour) {
#ifdef USER_LOGFILE_DIR
  char buffer[BUFSIZ];
  long long suffix = year * (long long)1e6 + mon * (int)1e4 + day * 100 + hour;
  if (file_suffix != suffix) {
    file_suffix = suffix;
    buffer[snprintf(buffer, BUFSIZ, USER_LOGFILE_DIR "_%lld", suffix)] = '\0';
    if (log_fd != -1) close(log_fd);
    log_fd = open(buffer, O_APPEND | O_WRONLY | O_CREAT | O_CLOEXEC, 0644);
  }
#else
  if (log_fs == -1) log_fd = fileno(stderr);
#endif  // USER_LOGFILE_DIR
}

void DoLog(const char *format, ...) {
  char buffer[BUFSIZ];
  time_t t = time(NULL);
  pthread_mutex_lock(&mutex);
  struct tm *fulltime = localtime(&t);
  UpdateFile(fulltime->tm_year + 1900, fulltime->tm_mon + 1, fulltime->tm_mday,
             fulltime->tm_hour);
  formatted_time[strftime(formatted_time, sizeof(formatted_time),
                          "%Y-%m-%d %H:%M:%S", fulltime)] = '\0';
  va_list args;
  va_start(args, format);
  int n = vsnprintf(buffer, BUFSIZ, format, args);
  va_end(args);
  write(log_fd, buffer, n);
  pthread_mutex_unlock(&mutex);
}

#endif  // TINY_C_LOG_POSIX_IMPL

#endif  // TINY_C_LOG_POSIX__
