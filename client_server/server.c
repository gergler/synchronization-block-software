#include<stdio.h>
#include<string.h>	//strlen
#include<sys/socket.h>
#include <fcntl.h>
#include<arpa/inet.h>	//inet_addr
#include<unistd.h>	//write
#include <stdlib.h>
#include <sys/mman.h>


#define PORT 8888
#define MSGSIZE 32
#define REGSIZE 32

#define ALT_LWFPGASLVS_OFST 0xFF200000
#define SPAN 0x8

void configure_fpga(char file_name) {
    printf("Configure FPGA\n");
    system("cat firmware.rbf > /dev/fpga0");
}

uint32_t read_register(uint32_t register_addr) {
    int fd = open("/dev/mem", (O_RDWR | O_SYNC));
    uint32_t *reg = mmap(NULL, SPAN, (PROT_READ | PROT_WRITE), MAP_SHARED, fd, (ALT_LWFPGASLVS_OFST + register_addr));
    munmap(reg, SPAN);
    close(fd);
    return *reg;
}

void write_register(uint32_t register_addr, uint32_t value) {
    int fd = open("/dev/mem", (O_RDWR | O_SYNC));
    uint32_t *reg = mmap(NULL, SPAN, (PROT_READ | PROT_WRITE), MAP_SHARED, fd, (ALT_LWFPGASLVS_OFST + register_addr));
    *reg = value;
    munmap(reg, SPAN);
    close(fd);
}

int main() {
    int sock;
    char buffer[MSGSIZE], reg[REGSIZE];
    char *configuration = "Print configuration file name: ";
    char *reading_register = "Print register for reading: ";
    char *writing_register = "Print register for writing: ";
    char *writing_value = "Print value for writing: ";

    struct sockaddr_in server, client;

    //Create socket
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        fprintf(stderr, "Error at opening stream socket\n");
        return -1;
    }
    puts("Socket created");

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    //Bind
    if (bind(sock, (struct sockaddr *) &server, sizeof(server)) < 0) {
        fprintf(stderr, "Error at binding stream socket\n");
        if (close(sock) == -1)
            fprintf(stderr, "Error at closing socket\n");
        return -1;
    }
    puts("Socket successfully binded");

    int read_size, len;
    while (1) {
        read_size = recvfrom(sock, (char *)buffer, sizeof(buffer), 0, ( struct sockaddr *) &client, &len);
        buffer[read_size] = '\0';
        printf("Command from client: %s\n", buffer);
        if (strncmp("c", buffer, 1) == 0) {
            bzero(buffer, sizeof(buffer));
            sendto(sock, (const char *)configuration, strlen(configuration), 0, (const struct sockaddr *) &client, sizeof(client));
            printf("Configuration file name sent\n");
            read_size = recvfrom(sock, (char *)buffer, sizeof(buffer), 0, ( struct sockaddr *) &client, &len);
            buffer[read_size] = '\0';
            configure_fpga(*buffer);
        }
        else if (strncmp("r", buffer, 1) == 0) {
            bzero(reg, sizeof(reg));
            sendto(sock, (const char *)reading_register, strlen(reading_register), 0, (const struct sockaddr *) &client, sizeof(client));
            printf("Register for reading sent\n");
            read_size = recvfrom(sock, (char *)reg, sizeof(reg), 0, ( struct sockaddr *) &client, &len);
            reg[read_size] = '\0';
            read_register(*reg);
        }
        else if (strncmp("w", buffer, 1) == 0) {
            bzero(buffer, sizeof(buffer));
            bzero(reg, sizeof(reg));
            sendto(sock, (const char *)writing_register, strlen(writing_register), 0, (const struct sockaddr *) &client, sizeof(client));
            printf("Register for writing sent\n");
            read_size = recvfrom(sock, (char *)buffer, sizeof(buffer), 0, ( struct sockaddr *) &client, &len);
            buffer[read_size] = '\0';

            sendto(sock, (const char *)writing_value, strlen(writing_value), 0, (const struct sockaddr *) &client, sizeof(client));
            printf("Value for writing sent\n");
            read_size = recvfrom(sock, (char *)buffer, sizeof(buffer), 0, ( struct sockaddr *) &client, &len);
            buffer[read_size] = '\0';
            write_register(*reg, *buffer);
        }
        else if (strncmp("exit", buffer, 4) == 0) {
            printf("Server Exit\n");
            break;
        }
    }
    if (close(sock) == -1) {
        fprintf(stderr, "Error at closing socket\n");
        return -1;
    }
    return 0;
}