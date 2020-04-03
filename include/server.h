#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdint.h>
#include <netdb.h>
#include <string.h>

//#define SERV_ADDR "10.42.0.1" // gateway
#define SERV_ADDR "192.168.43.1" // gateway
#define SERV_PORT 1024
#define TEAM_ID   5             /* Your team ID */

#define MSG_ACK    0
#define MSG_START  1
#define MSG_STOP   2
#define MSG_KICK   3
#define MSG_TEST   4
#define MSG_OBJ_ID 5

#define SA struct sockaddr
#define Sleep( msec ) usleep(( msec ) * 1000 )

void waitSTOP();
int waitSTART();
int read_from_server (int sock, uint8_t* buffer, size_t maxSize);
int foundObject(int ball, int x, int y, int shape, int change);
