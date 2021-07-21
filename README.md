# TinyCLog
A header only C language logging library.

```log
2021-07-21 21:47:18	example.c:main:4	DEBUG	this is a debug #1
2021-07-21 21:47:18	example.c:main:5	INFO	this is a info #2
2021-07-21 21:47:18	example.c:main:6	WARN	this is a warning #3
2021-07-21 21:47:18	example.c:main:7	ERROR	this is a error #4
2021-07-21 21:47:18	example.c:main:8	FATAL	this is a fatal #5
```

## How to Use
Copy `log_posix.h` to your project directory.
```c
#include "log_posix.h"

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
1. Compile `log_posix.h` while the macro `TINY_C_LOG_LOG_IMPL` is defined. `USER_LOGFILE_LOCATION` and `USER_LOG_LEVEL` are optional.
```
gcc -DTINY_C_LOG_LOG_IMPL -DUSER_LOGFILE_DIR=\"example.log\" -DUSER_LOG_LEVEL=DEBUG -c log_posix.h
```

2. Compile the other source files. Make sure that `TINY_C_LOG_LOG_IMPL` is **not defined**.
```
gcc -c example.c
```

3. Link the object files that generated in step 1 and step 2, and complete your build.
```
gcc -o example example.o log_posix.h.gch
```
