/**                  _ _              _____ ___
 *  _ __   _____   _(_) | ____      _|___ // _ \
 * | '_ \ / _ \ \ / / | |/ /\ \ /\ / / |_ \ (_) |
 * | | | |  __/\ V /| |   <  \ V  V / ___) \__, |
 * |_| |_|\___| \_/ |_|_|\_\  \_/\_/ |____/  /_/
 **/

#ifndef LAB2_H
#define LAB2_H

#include <assert.h>
#include <errno.h>
#include <inttypes.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>

#ifndef nevikw39
// #pragma GCC optimize("Ofast,unroll-loops,no-stack-protector,fast-math")
// #pragma GCC target("tune=native,arch=x86-64")
// #pragma comment(linker, "/stack:200000000")
#else
#pragma message("hello, nevikw39")
#endif
#pragma message("GL; HF!")

#ifndef DEBUG
#define DEBUG 0
#endif
#define DBG(x, ...) \
	if (DEBUG)      \
		fprintf(stderr, "\033[33m%s:%d:%s(): " x "\n\033[0m", __FILE__, __LINE__, __func__, __VA_ARGS__);
#define ERR(x)                                                                                                              \
	{                                                                                                                       \
		fprintf(stderr, "\033[31m%s:%d:%s(): " x "\n\033[35m\t%s\n\033[0m", __FILE__, __LINE__, __func__, strerror(errno)); \
		exit(EXIT_FAILURE);                                                                                                 \
	}

//==============
// Packet header
//==============
typedef struct header
{
	unsigned int seq_num;
	unsigned int ack_num;
	unsigned char isLast;
} Header;

//==================
// Udp packet & data
//==================
typedef struct udp_pkt
{
	Header header;
	char data[1024];
} Udp_pkt;

#endif // LAB2_H
