#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "server_parameters.h"
#include "cmd_packet.h"

#if 0
  #define IP "192.168.7.1"
#else
  #define IP "127.1"
#endif

CMD_Packet packet { 'z', 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF };
   
int main(int argc, char* argv[]) {

    if (argc-1 > 0)
        packet.cmd    = toupper(argv[1][0]);
    if (argc-1 > 1)
        packet.number = strtoul(argv[2], nullptr, 0);
    if (argc-1 > 2)
        packet.value  = strtoul(argv[3], nullptr, 0);//atoi(argv[3]);
    if (argc-1 > 3)
        packet.status = strtoul(argv[4], nullptr, 0);//atoi(argv[4]);

    int  sockfd;
    char buffer[MAXLINE];
    //char* buffer = &packet;
#ifdef USE_IPV6
    struct sockaddr_in6 servaddr;
#else
    struct sockaddr_in  servaddr;
#endif
   
    if ( (sockfd = socket(AF_PROTOCOL, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
   
    memset(&servaddr, 0, sizeof(servaddr));
       
#ifdef USE_IPV6
    servaddr.sin6_family = AF_PROTOCOL;
    servaddr.sin6_addr   = in6addr_any;
    servaddr.sin6_port   = htons(PORT);
#else
    servaddr.sin_family  = AF_PROTOCOL;
    servaddr.sin_addr.s_addr = inet_addr(IP);
    servaddr.sin_port    = htons(PORT);
#endif       
    socklen_t addrlen = sizeof(servaddr);
    
    sendto(sockfd, &packet, sizeof(packet), MSG_CONFIRM, (const struct sockaddr *) &servaddr, addrlen);
    printf("Sent:  ");
    print_packet(packet);
    //printf("\n");
    
           
    int n;
    n = recvfrom(sockfd, buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *) &servaddr, &addrlen);
    //buffer[n] = '\0';
    printf("Reply: ");
    print_packet(reinterpret_cast<CMD_Packet&>(buffer));
   
    close(sockfd);
    return 0;
}
