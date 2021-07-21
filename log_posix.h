#ifndef TINY_C_LOG_LOG__
#define TINY_C_LOG_LOG__

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

#ifdef TINY_C_LOG_LOG_IMPL

int log_fd = -1;

#ifdef USER_LOG_LEVEL
const enum LogLevel log_level = USER_LOG_LEVEL;
#else
const enum LogLevel log_level = INFO;
#endif  // USER_LOG_LEVEL

char formatted_time[64];

void InitLogFile() {
#ifdef USER_LOGFILE_DIR
  log_fd =
      open(USER_LOGFILE_DIR, O_APPEND | O_WRONLY | O_CREAT | O_CLOEXEC, 0644);
#else
  log_fd = fileno(stderr);
#endif  // USER_LOGFILE_DIR
}

void DoLog(const char *format, ...) {
  char buffer[BUFSIZ];
  if (log_fd == -1) InitLogFile();
  time_t t = time(NULL);
  struct tm *fulltime = localtime(&t);
  formatted_time[strftime(formatted_time, sizeof(formatted_time),
                          "%Y-%m-%d %H:%M:%S", fulltime)] = '\0';
  va_list args;
  va_start(args, format);
  int n = vsnprintf(buffer, BUFSIZ, format, args);
  va_end(args);
  write(log_fd, buffer, n);
}

#endif  // TINY_C_LOG_LOG_IMPL

#endif  // TINY_C_LOG_LOG__
