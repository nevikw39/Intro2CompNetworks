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

#define TIMEOUT 100
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

//=============
// Declaration
//=============
int sockfd;
struct sockaddr_in info, client_info;
Udp_pkt snd_pkt, rcv_pkt;
socklen_t len;
pthread_t th1, th2;
int first_time_create_thread = 0;

//---------------------------------------
// Declare for critical section in bonus.
//---------------------------------------
/*******************notice*******************************
 *
 * If you dont need the bonus point, ignore this comment.
 * Use it like following block.
 *
 * pthread_mutex_lock( &mutex );
 * ...
 * critical section
 * ...
 * pthread_mutex_unlock( &mutex );
 *
 *********************************************************/
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

//------------------------------
// Bonus part for timeout_thread
//------------------------------
/*******************notice***************************
 *
 * In bonus part, you should use following threads to
 * checking timeout and receive client ack.
 *
 ***************************************************/
void *receive_thread()
{
	//--------------------------------------
	// Checking timeout & Receive client ack
	//--------------------------------------

	//------------------------------------------
	// Keep the thread alive not to umcomment it
	//------------------------------------------
	// pthread_exit(NULL);
}

//------------------------------
// Bonus part for timeout_thread
//------------------------------
void *timeout_thread()
{
	//------------------------------------------
	// Keep the thread alive not to umcomment it
	//------------------------------------------
	// pthread_exit(NULL);
}

//==================================
// You should complete this function
//==================================
// Send file function, it call receive_thread function at the first time.
int sendFile(FILE *fd)
{
	int filesize = ftell(fd);
	//----------------------------------------------------------------
	// Bonus part for declare timeout threads if you need bonus point,
	// umcomment it and manage the thread by youself
	//----------------------------------------------------------------
	// At the first time, we need to create thread.
	if (!first_time_create_thread)
	{
		first_time_create_thread = 1;
		// pthread_create(&th1, NULL, receive_thread, NULL);
		// pthread_create(&th2, NULL, timeout_process, NULL);
	}
	/*******************notice************************
	 *
	 * In basic part, you should finish this function.
	 * You can try test_clock.c for clock() usage.
	 * checking timeout and receive client ack.
	 *
	 ************************************************/

	//==========================
	// Send video data to client
	//==========================

	//======================================
	// Checking timeout & Receive client ack
	//======================================

	//=============================================
	// Set is_last flag for the last part of packet
	//=============================================

	for (rewind(fd); filesize > 0; ++snd_pkt.header.seq_num)
		do
		{
			DBG("%d", filesize);
			int sz = fread(snd_pkt.data, sizeof(char), sizeof(snd_pkt.data), fd);
			if (!sz)
				ERR("fread()");
			snd_pkt.header.isLast = (filesize -= sz) <= 0;
			if (!~sendto(sockfd, &snd_pkt, sizeof(snd_pkt.header) + sz, 0, (struct sockaddr *)&client_info, len))
				ERR("`sendto()` failed!");
			for (clock_t clk = clock(); !~recvfrom(sockfd, &rcv_pkt, sizeof(rcv_pkt), 0, (struct sockaddr *)&client_info, (socklen_t *)&len) && (clock() - clk) * 1000 / CLOCKS_PER_SEC < TIMEOUT;)
				;
			printf("\tACK=%u\n", rcv_pkt.header.ack_num);
		} while (rcv_pkt.header.ack_num != snd_pkt.header.seq_num);

	puts("Send file successfully");
	fclose(fd);
	return 0;
}

int main(int argc, char *argv[])
{
	//===========================
	// argv[1] is for server port
	//===========================

	if (argc < 2)
		ERR("Please specify the port.");

	if (!~(sockfd = socket(AF_INET, SOCK_DGRAM, 0)))
		ERR("Fail to create a socket.");
	//=======================
	// input server info
	// IP address = 127.0.0.1
	//=======================
	bzero(&info, sizeof(info));
	info.sin_family = AF_INET;
	int port = atoi(argv[1]);
	info.sin_addr.s_addr = INADDR_ANY;
	info.sin_port = htons(port);
	// printf("server %s : %d\n", inet_ntoa(info.sin_addr), htons(info.sin_port));

	//================
	// Bind the socket
	//================
	if (!~bind(sockfd, (struct sockaddr *)&info, sizeof(info)))
		ERR("`bind()` failed!");

	//====================================
	// Create send packet & receive packet
	//====================================
	memset(rcv_pkt.data, '\0', sizeof(rcv_pkt.data));

	//====================
	// Use for client info
	//====================
	bzero(&client_info, sizeof(client_info));
	client_info.sin_family = AF_INET;
	len = sizeof(client_info);

	puts("====Server Parameter====");
	puts("IP:\t127.0.0.1");
	printf("Port:\t%d\n", port);
	puts("========================");

	while (true)
	{
		//=========================
		// Initialization parameter
		//=========================
		snd_pkt.header.seq_num = snd_pkt.header.ack_num = snd_pkt.header.isLast = 0;
		FILE *fd;

		puts("Server waiting...");
		char *str;
		while (~recvfrom(sockfd, &rcv_pkt, sizeof(rcv_pkt), 0, (struct sockaddr *)&client_info, (socklen_t *)&len))
		{
			// In client, we set is_last 1 to comfirm server get client's first message.
			if (rcv_pkt.header.isLast == 1)
				break;
		}
		puts("Process command...");
		str = strtok(rcv_pkt.data, " ");

		//===============================================================
		// command "download filename": download the file from the server
		// and then check if filename is exist
		//===============================================================
		if (!strcmp(str, "download"))
		{
			str = strtok(NULL, " \n");
			printf("Filename is %s\n", str);
			//===================
			// if file not exists
			//===================
			if (!(fd = fopen(str, "rb")))
			{
				//=======================================
				// Send FILE_NOT_EXISTS msg to the client
				//=======================================
				puts("FILE_NOT_EXISTS");
				strcpy(snd_pkt.data, "FILE_NOT_EXISTS");
				int numbytes;
				if (!~(numbytes = sendto(sockfd, &snd_pkt, sizeof(snd_pkt), 0, (struct sockaddr *)&client_info, len)))
					ERR("`sendto()` failed!");
				printf("server: sent %d bytes to %s\n", numbytes, inet_ntoa(client_info.sin_addr));
			}
			//==================
			// else, file exists
			//==================
			else
			{
				fseek(fd, 0, SEEK_END);
				puts("FILE_EXISTS");
				strcpy(snd_pkt.data, "FILE_EXISTS");

				//==================================
				// Send FILE_EXIST msg to the client
				//==================================
				int numbytes;
				if (!~(numbytes = sendto(sockfd, &snd_pkt, sizeof(snd_pkt), 0, (struct sockaddr *)&client_info, len)))
					ERR("`sendto()` failed!");
				printf("server: sent %d bytes to %s\n", numbytes, inet_ntoa(client_info.sin_addr));

				//==========================================================================
				// Sleep 1 seconds before transmitting data to make sure the client is ready
				//==========================================================================
				sleep(1);
				puts("Trasmitting...");

				//=====================================
				// Start to send the file to the client
				//=====================================

				sendFile(fd);
			}
		}
		else
			puts("Illegal request!");
	}
}