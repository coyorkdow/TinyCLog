# TinyCLog
TinyCLog is mini C logging library. It is header only and thread safety.

```log
2021-07-21 21:47:18	example.c:main:4	DEBUG	this is a debug #1
2021-07-21 21:47:18	example.c:main:5	INFO	this is a info #2
2021-07-21 21:47:18	example.c:main:6	WARN	this is a warning #3
2021-07-21 21:47:18	example.c:main:7	ERROR	this is a error #4
2021-07-21 21:47:18	example.c:main:8	FATAL	this is a fatal #5
```

## Usage
Copy folder `tiny_c_log` to your project directory.
```c
#include "tiny_c_log/log_posix.h"

int main(void) {
  LOG_DEBUG("this is a debug #%d", 1);
  LOG_INFO("this is a info #%d", 2);
  LOG_WARN("this is a warning #%d", 3);
  LOG_ERROR("this is a error #%d", 4);
  LOG_FATAL("this is a fatal #%d", 5);

  return 0;
}
```

## Build
Since TinyCLog is header only, to avoid symbols redefined, all the definition of symbols with external linkage are wraped in `#ifdef TINY_C_LOG_POSIX_IMPL`. So you have to make sure that macro `TINY_C_LOG_POSIX_IMPL` is defined in **one and only one** compilation unit. `USER_LOGFILE_LOCATION` and `USER_LOG_LEVEL` are optional and are used to configure the location of log files and logging level.

```
gcc -DTINY_C_LOG_POSIX_IMPL -DUSER_LOGFILE_DIR=\"example.log\" -DUSER_LOG_LEVEL=DEBUG -c example.c
gcc -c mutilthread.c
gcc -pthread -o example example.o mutilthread.o
```
