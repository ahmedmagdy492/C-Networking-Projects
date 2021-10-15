#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main()
{
    char* ip = "50.19.104.221";

    int sock = socket(AF_INET, SOCK_STREAM, 6);
    if(sock == -1)
    {
        fprintf(stderr, "Cannot Create Socket\n");
        exit(-1);
    }

    struct sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(80);
    inet_aton(ip, &servaddr.sin_addr);

    fprintf(stdout, "Connecting ...\n");
    int con = connect(sock, (struct sockaddr*)&servaddr, sizeof(servaddr));
    if(con == -1)
    {
        fprintf(stderr, "Cannot connect to the remote server\n");
        exit(-1);
    }
    fprintf(stdout, "Connected to Remote Server\n");

    char buffer[2048] = "GET / HTTP/1.1\r\nHost: api.ipify.org\r\nUser-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:78.0) Gecko/20100101 Firefox/78.0\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\nAccept-Language: en-US,en;q=0.5\r\nAccept-Encoding: gzip, deflate\r\nConnection: keep-alive\r\nUpgrade-Insecure-Requests: 1\r\nCache-Control: max-age=0\r\n\r\n";

    int send_result = send(sock, buffer, sizeof(buffer), 0);
    if(send_result == -1)
    {
        fprintf(stderr, "Cannot send requests to the remote server\n");
        exit(-1);
    }

    fprintf(stdout, "Proccessing...\n");
    memset(buffer, 0, sizeof(buffer));
    int recv_size = recv(sock, buffer, sizeof(buffer), 0);

    if(recv_size > 0)
    {
        char *ip_addr;
        ip_addr = strtok(buffer, "\r\n");
        for(int i = 0; i < 8; i++)
        {
            ip_addr = strtok(NULL, "\r\n");
        }
        ip_addr[1023] = '\0';
        fprintf(stdout, "Your Public IP Address is: %s\n", ip_addr);
    }

    close(sock);
}