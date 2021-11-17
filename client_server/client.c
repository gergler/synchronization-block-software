#include <stdio.h>	//printf
#include <string.h>	//strlen
#include <sys/socket.h>	//socket
#include <arpa/inet.h>	//inet_addr
#include <unistd.h>

#define IP "127.0.0.1"
#define PORT 8888
#define MSGSIZE 32

int main(int argc, char *argv[]) {
    int sock;
    char buffer[MSGSIZE];
    struct sockaddr_in server;

    //Create socket
    if ((sock = socket(AF_INET , SOCK_DGRAM , 0)) < 0) {
        fprintf(stderr, "Error at opening stream socket\n");
        return -1;
    }
    puts("Socket created");

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(IP);
    server.sin_port = htons(PORT);

    //Connect to remote server
    if (connect(sock, (struct sockaddr *) &server, sizeof(server)) < 0) {
        fprintf(stderr, "Error at connecting stream socket\n");
        if (close(sock) == -1)
            fprintf(stderr, "Error at closing socket\n");
        return -1;
    }
    puts("Connected\n");

    while (1) {
        printf("Please enter command (c - config fpga, r - read reg, w - write reg, exit): ");
        scanf("%s", buffer);
        int len, n;
        if (sendto(sock, (char *)buffer, sizeof(buffer),0, (const struct sockaddr *) &server, sizeof(server)) < 0)
            fprintf(stderr, "Error at sending data\n");
        if (strncmp("exit", buffer, 4) == 0) {
            printf("Client Exit\n");
            break;
        }
        bzero(buffer, strlen(buffer));
        if ((n = recvfrom(sock, (char *)buffer, sizeof(buffer), 0, (struct sockaddr *) &server, &len)) < 0)
            fprintf(stderr, "Error at receiving data\n");
        buffer[n] = '\0';
        printf("%s", buffer);
        bzero(buffer, strlen(buffer));
        scanf("%s", buffer);
        if (sendto(sock, (char *)buffer, sizeof(buffer),0, (const struct sockaddr *) &server, sizeof(server)) < 0)
            fprintf(stderr, "Error at sending data\n");
        bzero(buffer, strlen(buffer));

        if ((n = recvfrom(sock, (char *)buffer, sizeof(buffer), 0, (struct sockaddr *) &server, &len)) < 0)
            fprintf(stderr, "Error at receiving data\n");
        buffer[n] = '\0';
        printf("Response to command from server: %s", buffer);
        bzero(buffer, strlen(buffer));
    }

    if (close(sock) < 0) {
        fprintf(stderr, "Error at closing socket\n");
        return -1;
    }
    return 0;
}
