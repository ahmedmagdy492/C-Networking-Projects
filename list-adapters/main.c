#include <stdio.h>
#include <stdlib.h>
#include <ifaddrs.h>
#include <sys/socket.h>
#include <netdb.h>

int main()
{
    struct ifaddrs* addresses;

    if(getifaddrs(&addresses) == -1)
    {
        fprintf(stderr, "Cannot get addresses\n");
        exit(-1);
    }

    struct ifaddrs* ptr = addresses;

    while(ptr != NULL)
    {
        if(ptr->ifa_addr->sa_family == AF_INET || ptr->ifa_addr->sa_family == AF_INET6)
        {
            fprintf(stdout, "Network: %s, %s, ", ptr->ifa_name, ptr->ifa_addr->sa_family == AF_INET ? "IPv4" : "IPv6");

            const int family_size = ptr->ifa_addr->sa_family == AF_INET ? sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6);
            char ap[100];
            char serv[100];

            getnameinfo(ptr->ifa_addr, family_size, ap, sizeof(ap), serv, sizeof(serv), NI_NUMERICHOST);
            fprintf(stdout, "IP: %s, Service: %s\n", ap, serv);
        }
        ptr = ptr->ifa_next;
    }
}