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
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

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

void split_url(const char *const url, char **host, char **path) // url => host/path
{
    const char *const back_slash = strchr(url, '/');
    if (back_slash)
    {
        *host = malloc(sizeof(char) * (back_slash - url + 1));
        strncpy(*host, url, back_slash - url);
        (*host)[back_slash - url] = 0;
        *path = malloc(sizeof(char) * (strlen(back_slash) + 1));
        strcpy(*path, back_slash);
    }
    else // Assume that `url` == `host`
    {
        *host = malloc(sizeof(char) * (strlen(url) + 1));
        strcpy(*host, url);
        *path = malloc(sizeof(char) << 1);
        **path = '/';
        1 [*path] = 0;
    }
}

int main()
{
#pragma region Input
    char *url = NULL, *host = NULL, *path = NULL; // url := host/path
    printf("Please enter the URL:\n>>> ");
    scanf("%ms", &url);
    split_url(url, &host, &path);
    DBG("Host = %s, Path = %s", host, path);
    if (url)
    {
        free(url);
        url = NULL;
    }
#pragma endregion // Input

#pragma region Socket
    int fd = socket(AF_INET, SOCK_STREAM, 0); // IPv4, sequenced & reliable, auto
    if (!~fd)
        ERR("Failed to create socket!!");

    struct hostent *entry = gethostbyname(host);
    if (!entry)
        ERR("Failed to look up hostname!!");

    struct sockaddr_in addr = {.sin_family = AF_INET, .sin_port = htons(80)};
    memcpy(&addr.sin_addr, *(entry->h_addr_list), entry->h_length);
    if (!~connect(fd, &addr, sizeof addr))
        ERR("Failed to connect!!\n");

    char *req = malloc(sizeof(char) * (45 + strlen(path) + strlen(host)));
    sprintf(req, "GET %s HTTP/1.0\r\nHost: %s\r\nConnection: close\r\n\r\n", path, host);
    DBG("Req = ``%s\"\"", req);
    if (!~send(fd, req, strlen(req), 0))
        ERR("Failed to send!!");
    free(req);

    static char buf[N];
    size_t len = 0, x;
    do
    {
        x = recv(fd, buf, sizeof buf, MSG_WAITALL);
        if (!~x)
            ERR("Failed to receive!!");
        len += x;
    } while (x);
    DBG("Buf = ``%s\"\"\n\n\tRecieved %zu characters", buf, len);
#pragma endregion // Socket

#pragma region
    static const char str[] = "<a href=\"";
    int cnt = 0;
    puts("The following are all Hyper links in format of `<a href=\"*\">`:");
    for (const char *ptr = buf, *qtr; ptr && *ptr; ++ptr, ++cnt)
    {
        ptr = strstr(ptr, str);
        if (!ptr || !*ptr)
            break;
        ptr += sizeof(str) - 1;
        qtr = strchr(ptr, '\"');
        if (!qtr)
            break;
        putchar('\t');
        while (ptr != qtr)
            putchar(*ptr++);
        putchar('\n');
    }
    printf("There are %d links in total.\n", cnt);
#pragma endregion //

#pragma region GC
    if (host)
        free(host);
    if (path)
        free(path);
    close(fd);
#pragma endregion // GC

    return 0;
}
