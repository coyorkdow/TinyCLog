#include <pthread.h>
#include <string.h>
#include <unistd.h>

#include "tiny_c_log/log_posix.h"
#include "mutilthread.h"

pthread_t threads[5];

struct thread_info {
  int id;
  char opt[6];
};

static void *thread_start(void *arg) {
  struct thread_info *ptr = (struct thread_info *)arg;
  int cnt;
  for (cnt = 0; cnt < 100; cnt++) {
    if (strcmp(ptr->opt, "DEBUG") == 0) {
      LOG_DEBUG("thread id = %d, num = %d", ptr->id, cnt);
    } else if (strcmp(ptr->opt, "INFO") == 0) {
      LOG_INFO("thread id = %d, num = %d", ptr->id, cnt);
    } else if (strcmp(ptr->opt, "WARN") == 0) {
      LOG_WARN("thread id = %d, num = %d", ptr->id, cnt);
    } else if (strcmp(ptr->opt, "ERROR") == 0) {
      LOG_ERROR("thread id = %d, num = %d", ptr->id, cnt);
    } else if (strcmp(ptr->opt, "FATAL") == 0) {
      LOG_FATAL("thread id = %d, num = %d", ptr->id, cnt);
    }
  }
  return NULL;
}

void multithread_logging() {
  static struct thread_info info[5] = {
      {1, "DEBUG"}, {2, "INFO"}, {3, "WARN"}, {4, "ERROR"}, {5, "FATAL"}};
  size_t i;
  for (i = 0; i < 5; i++) {
    pthread_create(&threads[i], NULL, thread_start, &info[i]);
  }
  for (i = 0; i < 5; i++) {
    pthread_join(threads[i], NULL);
  }
}