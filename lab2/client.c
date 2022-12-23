/**                  _ _              _____ ___
 *  _ __   _____   _(_) | ____      _|___ // _ \
 * | '_ \ / _ \ \ / / | |/ /\ \ /\ / / |_ \ (_) |
 * | | | |  __/\ V /| |   <  \ V  V / ___) \__, |
 * |_| |_|\___| \_/ |_|_|\_\  \_/\_/ |____/  /_/
 **/
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

#define N (1 << 16)
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

/*****************notice**********************
 *
 * You can follow the comment inside the code.
 * This kind of comment is for basic part.
 * ===============
 * Some hints...
 * ===============
 *
 * This kind of comment is for bonus part.
 * ---------------
 * Some hints...
 * ---------------
 *
 *
 *
 *********************************************/

//==============
// Packet header
//==============
typedef struct header
{
	unsigned int seq_num;
	unsigned int ack_num;
	unsigned char is_last;
} Header;

//==================
// Udp packet & data
//==================
typedef struct udp_pkt
{
	Header header;
	char data[1024];
} Udp_pkt;

//============
// Declaration
//============
int sockfd = 0;
Udp_pkt snd_pkt, rcv_pkt;
struct sockaddr_in info, client_info;
socklen_t len;
time_t t1, t2;

//=====================
// Simulate packet loss
//=====================
int isLoss(double prob)
{
	double thres = prob * RAND_MAX;

	if (prob >= 1)
		return 1;			  // loss
	return (rand() <= thres); // not loss
}

//==================================
// You should complete this cunction
//==================================
int recvFile(FILE *fd)
{
	puts("FILE_EXISTS");

	char *str;
	char fileName[30];

	//==================================================================
	// Split the command into "download" & "filename", just get filename
	//==================================================================
	str = strtok(snd_pkt.data, " \n");
	str = strtok(NULL, " \n");

	sprintf(fileName, "download_");
	strcat(fileName, str);

	// FILE *fd;
	fd = fopen(fileName, "wb");

	puts("Receiving...");
	char buffer[123431];
	int index = 0;
	int receive_packet = 0;
	memset(snd_pkt.data, '\0', sizeof(snd_pkt.data));
	while (1)
	{
		//=======================
		// Simulation packet loss
		//=======================
		if (isLoss(0.5))
		{
			puts("\tOops! Packet loss!");
			break;
		}
		//==============================================
		// Actually receive packet and write into buffer
		//==============================================

		//==============================================
		// Write buffer into file if is_last flag is set
		//==============================================

		//====================
		// Reply ack to server
		//====================
	}
	return 0;
}

int main(int argc, char *argv[])
{
	//==============
	// Create socket
	//==============
	// int sockfd = 0;
	if (!~(sockfd = socket(AF_INET, SOCK_DGRAM, 0)))
		ERR("Fail to create a socket.");

	//==================
	// Input server info
	//==================
	// struct sockaddr_in info;
	bzero(&info, sizeof(info));
	info.sin_family = AF_INET;

	char *server_ip = (char *)malloc(sizeof(char) * 30);
	int server_port;

	printf(">>> IP:\t");
	scanf("%s", server_ip);
	printf(">>> Port:\t");
	scanf("%d", &server_port);

	//==================================
	// Just test how to convert the type
	//==================================
	info.sin_addr.s_addr = inet_addr(server_ip);
	info.sin_port = htons(server_port);

	server_port = htons(info.sin_port);
	server_ip = inet_ntoa(info.sin_addr);
	// printf("server %s : %d\n", server_ip, server_port);

	//====================================
	// Create send packet & receive packet
	//====================================
	memset(snd_pkt.data, '\0', sizeof(snd_pkt.data));
	len = sizeof(info);

	puts("Waiting for a commands...");
	getchar();
	while (fgets(snd_pkt.data, 30, stdin))
	{

		// ================================
		// command "exit": close the client
		// ================================
		if (!strncmp(snd_pkt.data, "exit", 4))
		{
			break;
			// ==============================================================
			// command "download filename": download the file from the server
			// ==============================================================
		}
		else if (!strncmp(snd_pkt.data, "download", 8))
		{
			snd_pkt.header.seq_num = snd_pkt.header.ack_num = 0;
			snd_pkt.header.is_last = 1;
			// We first set is_last to 1 so that server know its our first message.
			int numbytes;
			FILE *fd;
			//========================
			// Send filename to server
			//========================
			if (!~(numbytes = sendto(sockfd, &snd_pkt, sizeof(snd_pkt), 0, (struct sockaddr *)&info, len)))
				ERR("`sendto()` failed!");
			printf("client: sent %d bytes to %s\n", numbytes, inet_ntoa(info.sin_addr));
			//=========================================
			// Get server response if file exist or not
			//=========================================
			if (!~(numbytes = recvfrom(sockfd, &rcv_pkt, sizeof(rcv_pkt), 0, (struct sockaddr *)&info, (socklen_t *)&len)))
				ERR("`recvfrom()` failed!");
			printf("client: receive %d bytes from %s\n", numbytes, inet_ntoa(info.sin_addr));
			// printf("%s", buf);

			//====================
			// File does not exist
			//====================
			if (!strcmp(rcv_pkt.data, "FILE_NOT_EXISTS"))
				puts("FILE_NOT_EXISTS");
			//==========================
			// File exists, receive file
			//==========================
			else if (!strcmp(rcv_pkt.data, "FILE_EXISTS"))
			{
				t1 = time(NULL);
				recvFile(fd);
				t2 = time(NULL);
				printf("Elasped %ld secs in total.\n", t2 - t1);
			}
		}
		else
			puts("Illegal command.");

		puts("Waiting for a commands...");
	}
}
