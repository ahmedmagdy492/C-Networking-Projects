#ifdef _WIN32
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment("lib", "ws2_32.lib")
#endif
#else
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <errno.h>
#endif

#include <stdio.h>
#include <stdlib.h>

int main()
{
    #ifdef _WIN32
    WSAData d;

    if(WSAStartup(MAKEWORD(2, 2), &d))
    {
        fprintf(stderr, "error initializing the win sock 2\n");
        return -1;
    }
    #endif

    #ifdef _WIN32
    WSACleanup();
    #endif

    return 0;
}