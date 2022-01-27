#ifndef TINY_C_LOG_POSIX__
#define TINY_C_LOG_POSIX__

#include <pthread.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
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

#ifdef USER_LOG_LEVEL
#define CONFIGURED_LOG_LEVEL USER_LOG_LEVEL
#else
#define CONFIGURED_LOG_LEVEL INFO
#endif  // USER_LOG_LEVEL

extern char formatted_time[64];

void DoLog(const char *format, ...);

#define _LOG(level, format, ...)                                      \
  do {                                                                \
    if (level >= CONFIGURED_LOG_LEVEL) {                              \
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

char formatted_time[64];

long long file_suffix = 0;

void UpdateFile(int year, int mon, int day, int hour) {
#ifdef USER_LOGFILE_DIR
  char buffer[BUFSIZ];
  long long suffix = year * (long long)1e6 + mon * (int)1e4 + day * 100 + hour;
  if (file_suffix != suffix) {
    file_suffix = suffix;
    buffer[snprintf(buffer, BUFSIZ, "%lld_" USER_LOGFILE_DIR, suffix)] = '\0';
    if (log_fd != -1) close(log_fd);
    log_fd = open(buffer, O_APPEND | O_WRONLY | O_CREAT | O_CLOEXEC, 0644);
  }
#else
  if (log_fd == -1) log_fd = fileno(stderr);
#endif  // USER_LOGFILE_DIR
}

char number_map[100][2];

void InitMap() {
  int i;
  for (i = 0; i < 100; i++) {
    sprintf(number_map[i], "%02d", i);
  }
}

void DoLog(const char *format, ...) {
  static long long last_log_sec = 0;
  if (*(char *)number_map == '\0') {
    InitMap();
  }
  char buffer[BUFSIZ];
  time_t t = time(NULL);
  pthread_mutex_lock(&mutex);
  if (last_log_sec != t) {
    last_log_sec = t;
    struct tm *fulltime = localtime(&t);
    int year = fulltime->tm_year + 1900;
    int mon = fulltime->tm_mon + 1;
    int day = fulltime->tm_mday;
    int hour = fulltime->tm_hour;
    UpdateFile(year, mon, day, hour);
    int n = 0;
    memcpy(formatted_time, number_map[year / 100], 2);
    memcpy(formatted_time + 2, number_map[year % 100], 2);
    formatted_time[4] = '-';
    n += 5;
    memcpy(formatted_time + n, number_map[mon], 2);
    formatted_time[n + 2] = '-';
    n += 3;
    memcpy(formatted_time + n, number_map[day], 2);
    formatted_time[n + 2] = ' ';
    n += 3;
    memcpy(formatted_time + n, number_map[hour], 2);
    formatted_time[n + 2] = ':';
    n += 3;
    memcpy(formatted_time + n, number_map[fulltime->tm_min], 2);
    formatted_time[n + 2] = ':';
    n += 3;
    memcpy(formatted_time + n, number_map[fulltime->tm_sec], 2);
    formatted_time[n + 2] = '\0';
  }
  va_list args;
  va_start(args, format);
  int n = vsnprintf(buffer, BUFSIZ, format, args);
  va_end(args);
  pthread_mutex_unlock(&mutex);
  write(log_fd, buffer, n);
}

#endif  // TINY_C_LOG_POSIX_IMPL

#endif  // TINY_C_LOG_POSIX__
