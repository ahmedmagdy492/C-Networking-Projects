#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <fcntl.h>
#define CHUNK_SIZE 16384

void recv_file();

int main()
{
    recv_file();
}

void recv_file()
{
    int sock = socket(AF_INET, SOCK_STREAM, 6);
    if(sock == -1)
    {
        perror("Error: ");
        exit(1);
    }

    struct sockaddr_in servaddr;
    servaddr.sin_port = htons(65000);
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;

    int bind_result = bind(sock, (struct sockaddr*)&servaddr, sizeof(servaddr));
    if(bind_result == -1)
    {
        perror("Error: ");
        exit(2);
    }

    int listen_result = listen(sock, 1);
    if(listen_result == -1)
    {
        perror("Error");
        exit(3);
    }

    fprintf(stdout, "Waiting for someone to connect...\n");
    struct sockaddr_in client_addr;
    socklen_t len;
    int client_sock = accept(sock, (struct sockaddr*)&client_addr, &len);
    if(client_sock == -1)
    {
        perror("Error");
        exit(4);
    }

    ssize_t recv_size = 0;
    char buffer[CHUNK_SIZE];
    int recv_fd = open("./file", O_WRONLY | O_CREAT | O_TRUNC);

    while(1)
    {
        if((recv_size = recv(client_sock, buffer, CHUNK_SIZE, 0)) <= 0)
        {
            break;
        }
        else
        {
            fprintf(stdout, "Received %d bytes\n", recv_size);
            write(recv_fd, buffer, recv_size);
        }
    }

    close(recv_fd);
    close(sock);
}