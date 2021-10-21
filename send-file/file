#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#define CHUNK_SIZE 16384

void send_file(char* ip, int port, char* filePath);

int main(int argc, char* argv[])
{
    if(argc != 4)
    {
        fprintf(stderr, "Usage %s <server-ip> <port> <path-to-file-to-sent>\n", argv[0]);
        exit(-1);
    }

    send_file(argv[1], atoi(argv[2]), argv[3]);
}

void send_file(char* ip, int port, char* filePath)
{
    int sock = socket(AF_INET, SOCK_STREAM, 6);
    if(sock == -1)
    {
        perror("Error");
        exit(3);
    }

    struct sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    inet_aton(ip, &servaddr.sin_addr);
    int con_result = connect(sock, (struct sockaddr*)&servaddr, sizeof(servaddr));
    if(con_result == -1)
    {
        perror("Error");
        exit(4);
    }

    int fd_open = open(filePath, O_RDONLY);
    if(fd_open == -1)
    {
        perror("Error");
        exit(2);
    }

    char buffer[CHUNK_SIZE];
    int chunck_size = 0;
    while((chunck_size = read(fd_open, buffer, CHUNK_SIZE)) > 0)
    {
        fprintf(stdout, "Sent %d bytes\n", chunck_size);
        send(sock, buffer, chunck_size, 0);
    }

    close(fd_open);
    close(sock);
}