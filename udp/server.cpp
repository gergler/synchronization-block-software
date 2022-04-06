#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "server_parameters.h"
#include "cmd_packet.h"

void dump(void* buffer, int len) {
    uint8_t* arg = reinterpret_cast<uint8_t*>(buffer);
    for (int i=0; i<len; ++i)
        printf("%x:", arg[i]);
    printf("\n");
}

uint16_t swab16(uint16_t x) {
    return ((x & 0xFF)<<8) | (x>>8);
}

void dump2(void* buffer, int len) {
    uint16_t* arg = reinterpret_cast<uint16_t*>(buffer);
    for (int i=0; i<len/2; ++i)
        printf("%04x:", swab16(arg[i]));
    printf("\n");
}


void process_command(CMD_Packet &packet) {
    printf("Execute:    ");
    print_packet(packet);
    //printf("\n");
    packet.status = 0;
}

int main(int argc, char* argv[])
{
    int sockfd;
    char buffer[MAXLINE];
    
#ifdef USE_IPV6
    struct sockaddr_in6 servaddr, cliaddr;
#else
    struct sockaddr_in  servaddr, cliaddr;
#endif

    if ((sockfd = socket(AF_PROTOCOL, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
       
    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

#ifdef USE_IPV6
    servaddr.sin6_family = AF_PROTOCOL;
    servaddr.sin6_addr   = in6addr_any;
    servaddr.sin6_port   = htons(PORT);
#else
    servaddr.sin_family  = AF_PROTOCOL;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port    = htons(PORT);
#endif


    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
       
    
    for (;;) {
        socklen_t addrlen = sizeof(cliaddr);  //len is value/resuslt
        int n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, ( struct sockaddr *) &cliaddr, &addrlen);
        if (n < 0) {
            perror("recieve data error");
            exit(EXIT_FAILURE);
        }

        system("date --rfc-3339=seconds");
        printf("Client from addr_len=%d, size=%d\n", addrlen, n);
        printf("addr: ");
#ifdef USE_IPV6
        dump2(&(cliaddr.sin6_addr), sizeof(cliaddr.sin6_addr) );
#else
        dump(&(cliaddr.sin_addr), sizeof(cliaddr.sin_addr) );
#endif
        if (n == sizeof(CMD_Packet)) {
            process_command( reinterpret_cast<CMD_Packet&>(buffer) );
        }

        if (sendto(sockfd, buffer, sizeof(CMD_Packet), MSG_CONFIRM, (const struct sockaddr *) &cliaddr, addrlen) < 0) {
            perror("send data error");
            exit(EXIT_FAILURE);
        }

        printf("Reply sent: "); 
        print_packet(reinterpret_cast<CMD_Packet&>(buffer));
        printf("\n");
    }
    
    if (close(sockfd) < 0) {
        perror("socket close error");
        exit(EXIT_FAILURE);
    }
    else
        return 0;
}
