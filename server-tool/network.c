#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <ifaddrs.h>
#include <netdb.h>
#include <unistd.h>
#include "network.h"
#define INTERFACE_COUNT 5
#define IP_SIZE 17

char* get_my_ip()
{
    struct ifaddrs* addresses;
    getifaddrs(&addresses);

    if(!addresses)
    {
        fprintf(stderr, "No Interface Found on this Machine\n");
        exit(-1);
    }

    struct ifaddrs* addr = addresses;
    int interface_count = 0;
    char** interfaces = (char**)malloc(sizeof(char) * INTERFACE_COUNT * IP_SIZE);

    for(int i = 0; i < INTERFACE_COUNT; i++)
    {
        interfaces[i] = (char*)malloc(sizeof(char) * IP_SIZE);
    }

    int interfaces_top = 0;
    while(addr != NULL)
    {
        if(addr->ifa_addr->sa_family == AF_INET)
        {
            interface_count++;
            getnameinfo(addr->ifa_addr, sizeof(struct sockaddr_in), interfaces[interfaces_top], IP_SIZE, 0, 0, 1);
            printf("%d: %s\n", interface_count, interfaces[interfaces_top]);
            interfaces_top++;
        }
        addr = addr->ifa_next;
    }
    printf("found %d interfaces on this machine which, one you choose to bind on? \n", interface_count);

    int choosen_option;
    scanf("%d", &choosen_option);
    if(choosen_option <= 0 || choosen_option > interface_count)
    {
        fprintf(stderr, "Invalid option\n");
        exit(-1);
    }

    char* ip_addr = (char*)malloc(sizeof(char) * IP_SIZE);
    for(int i = 0; i < INTERFACE_COUNT; i++)
    {
        if(i == choosen_option-1)
        {
            memcpy(ip_addr, interfaces[i], IP_SIZE);
            break;
        }
    }

    for(int i = 0; i < INTERFACE_COUNT; i++)
    {
        free(interfaces[i]);
    }
    free(interfaces);
    return ip_addr;
}

void create_server(int port)
{
    char *ip_addr = get_my_ip(ip_addr);
    printf("you choosen %s address\n", ip_addr);

    int sock = socket(AF_INET, SOCK_STREAM, 6);
    if(sock == -1)
    {
        fprintf(stderr, "Error Occured while creating socket\n");
        exit(-1);
    }

    struct sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    inet_aton(ip_addr, &servaddr.sin_addr);

    int bind_result = bind(sock, (struct sockaddr*)&servaddr, sizeof(servaddr));
    if(bind_result == -1)
    {
        fprintf(stderr, "Cannot bind on the spciefied interface\n");
        exit(-1);
    }

    int listen_result = listen(sock, 2);
    if(listen_result == -1)
    {
        fprintf(stderr, "Port %d is already in use pick up another one\n", port);
        exit(-1);
    }

    printf("Listening on Port %d....\n", port);
    
    struct sockaddr_in client_addr;
    socklen_t sock_len;
    int client_sock = accept(sock, (struct sockaddr*)&client_addr, &sock_len);
    char *client_ip = inet_ntoa(client_addr.sin_addr);

    printf("%s has just connected\n", client_ip);
    char buffer[2048];
    int recv_size = 0;

    while(1)
    {
        memset(buffer, 0, sizeof(buffer));
        if(recv_size = recv(client_sock, buffer, sizeof(buffer), 0) < 0)
        {
            break;
        }
        else
        {
            if(strncmp(buffer, "close", 4) == 0)
            {
                break;
            }
            printf("client: %s\n", buffer);
        }
    }

    close(sock);
    free(ip_addr);
}

void connect_to_server(char* ip, int port)
{
    int sock = socket(AF_INET, SOCK_STREAM, 6);
    if(sock == -1)
    {
        fprintf(stderr, "Cannot create socket\n");
        exit(-1);
    }

    struct sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    inet_aton(ip, &servaddr.sin_addr);

    int con = connect(sock, (struct sockaddr*)&servaddr, sizeof(servaddr));
    if(con == -1)
    {
        fprintf(stderr, "Cannot connect to the specified server\n");
        exit(-1);
    }

    char buffer[2048];
    while(1)
    {
        memset(buffer, 0, sizeof(buffer));
        printf("Enter your message: \n");
        fgets(buffer, sizeof(buffer), stdin);

        if(strncmp(buffer, "close", 4) == 0)
        {
            int send_result = send(sock, buffer, sizeof(buffer), 0);
            break;
        }
        else
        {
            int send_result = send(sock, buffer, sizeof(buffer), 0);
            if(send_result == -1)
            {
                fprintf(stderr, "cannot send data\n");
            }
        }
    }
    close(sock);
}