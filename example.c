#include "log_posix.h"

int main(void) {
  init_log_file();
  LOG_DEBUG("this is a debug #%d", 1);
  LOG_INFO("this is a info #%d", 2);
  LOG_WARN("this is a warning #%d", 3);
  LOG_ERROR("this is a error #%d", 4);
  LOG_FATAL("this is a fatal #%d", 5);

  return 0;
}