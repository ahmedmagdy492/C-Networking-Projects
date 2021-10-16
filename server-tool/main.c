#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "network.h"

void print_help();

int main(int argc, char* argv[])
{
    if(argc == 2)
    {
        print_help();
        return 0;
    }
    if(argc != 3)
    {
        fprintf(stderr, "Use %s -h to see help\n", argv[0]);
        return -1;
    }
    if(strncmp(argv[1], "-", 1) != 0)
    {
        fprintf(stderr, "Invalid Option %s\n", argv[1]);
        return -1;
    }

    if(strcmp(argv[1], "-s") == 0)
    {
        int port = atoi(argv[2]);
        if(port == 0)
        {
            fprintf(stderr, "Invalid Port number %s\n", argv[2]);
            return -1;
        }
        // Create server
        create_server(port);
    }
    else if(strcmp(argv[1], "-c") == 0)
    {
        // connect to a remote server
        char* ip = strtok(argv[2], ":");
        int port = atoi(strtok(NULL, ":"));

        if(ip == NULL || port == 0)
        {
            fprintf(stderr, "Invalid Arguments Passed to -c check out the help\n");
            exit(-1);
        }

        connect_to_server(ip, port);
    }
    else if(strcmp(argv[1], "-h") == 0)
    {
        print_help();
        return 0;
    }

    return 0;
}

void print_help()
{
    printf("\n");
    printf("-s: Creates a TCP Server that listens on the given port (ex: -s <port>)\n");
    printf("-c: Connects to a Server with the provided IP v4 address followed by colon ':' and then the port (ex: 192.168.1.1:5000)\n");
    printf("-h: show the help\n");
    printf("\n");
    printf("---------------------------------------------------------------------------\n");
    printf("\n");
    printf("Note: consider choosing ports starting from 1001 since the preceding 1000 ones are considered to be taken by common services\n");
    printf("\n");
}