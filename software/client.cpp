#include "client.h"
#include "json_config.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAXLINE 1024

static void set_socket_timeout(int socket, int sec, int usec) {
    struct timeval tv;
    tv.tv_sec = sec;
    tv.tv_usec = usec;
    setsockopt( socket, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(struct timeval) );
    setsockopt( socket, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(struct timeval) );
}

int exec_UDP_command(CMD_Packet req, CMD_Packet &reply, int timeout_us) {
    int  sockfd;
    char buffer[MAXLINE];

    struct sockaddr_in  servaddr;

    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));

    servaddr.sin_family      = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(Json_config::getIP().toStdString().c_str());
    servaddr.sin_port        = htons(Json_config::getPORT());

    socklen_t addrlen = sizeof(servaddr);

    set_socket_timeout(sockfd, 0, timeout_us);

    sendto(sockfd, &req, sizeof(req), MSG_CONFIRM, (const struct sockaddr *) &servaddr, addrlen);

    int n;
    n = recvfrom(sockfd, buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *) &servaddr, &addrlen);
    reply = *reinterpret_cast<CMD_Packet*>(buffer);

    ::close(sockfd);

    return n;
}


