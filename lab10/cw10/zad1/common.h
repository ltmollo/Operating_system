#ifndef ZAD1_COMMON_H
#include <fcntl.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/un.h>

#include <sys/epoll.h>

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>


#define MSG_LEN 256

typedef enum {
    PING,
    USERNAME,
    SERVER_FULL,
    DISCONNECT,
    GET,
    LIST,
    TO_ONE,
    TO_ALL,
    STOP,
} message_type;

#define find(init, cond) ({ int index = -1;  for (init) if (cond) { index = i; break; } index; })

#define safe(expr)                                                              \
  ({                                                                            \
    typeof(expr) __tmp = expr;                                                  \
    if (__tmp == -1) {                                                          \
      printf("%s:%d "#expr" failed: %s\n", __FILE__, __LINE__, strerror(errno));\
      exit(EXIT_FAILURE);                                                       \
    }                                                                           \
    __tmp;                                                                      \
  })


typedef struct {
    message_type type;
    char text[MSG_LEN]; // INCLUDES NICKNAME
    char other_nickname[MSG_LEN];
} message;

#define ZAD1_COMMON_H

#endif //ZAD1_COMMON_H
