#include<stdio.h>
#include<string.h>	//strlen
#include<sys/socket.h>
#include <fcntl.h>
#include<arpa/inet.h>	//inet_addr
#include<unistd.h>	//write
#include <stdlib.h>
#include <sys/mman.h>


#define PORT 8888
#define MSGSIZE 100
#define REGSIZE 32

#define ALT_LWFPGASLVS_OFST 0xFF200000
#define SPAN 0x8

uint32_t configure_fpga(char *file_name) {
    char command1[] = "cat ";
    char command2[] = " > /dev/fpga0";
    strcat(command1, file_name);
    strcat(command1, command2);
    printf("Configure FPGA\n");
    return system(command1) >> 8;
}

uint32_t read_register(uint32_t register_addr) {
    int fd = open("/dev/mem", (O_RDWR | O_SYNC));
    uint32_t *reg = mmap(NULL, SPAN, (PROT_READ | PROT_WRITE), MAP_SHARED, fd, (ALT_LWFPGASLVS_OFST + register_addr));
    munmap(reg, SPAN);
    close(fd);
    return *reg;
}

uint32_t write_register(uint32_t register_addr, uint32_t value) {
    int fd = open("/dev/mem", (O_RDWR | O_SYNC));
    uint32_t *reg = mmap(NULL, SPAN, (PROT_READ | PROT_WRITE), MAP_SHARED, fd, (ALT_LWFPGASLVS_OFST + register_addr));
    *reg = value;
    munmap(reg, SPAN);
    close(fd);
    return value;
}

void dialog(char *buffer, int sock, struct sockaddr_in client, int len, int read_size, char *text) {
    if (sendto(sock, text, strlen(text), 0, (const struct sockaddr *) &client, sizeof(client)) < 0)
        fprintf(stderr, "Error at sending data\n");
    bzero(buffer, sizeof(&buffer));
    if ((read_size = recvfrom(sock, buffer, sizeof(buffer), 0, ( struct sockaddr *) &client, &len)) < 0)
        fprintf(stderr, "Error at receiving data\n");
    buffer[read_size] = '\0';
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
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        fprintf(stderr, "Error at opening socket\n");
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
    uint32_t ret_data;
    while (1) {
        read_size = recvfrom(sock, (char *)buffer, sizeof(buffer), 0, ( struct sockaddr *) &client, &len);
        buffer[read_size] = '\0';
        printf("Command from client: %s\n", buffer);
        if (strncmp("c", buffer, 1) == 0) {
            dialog(buffer, sock, client, len, read_size, configuration);
            ret_data = configure_fpga(buffer);
        }
        else if (strncmp("r", buffer, 1) == 0) {
            dialog(reg, sock, client, len, read_size, reading_register);
            ret_data = read_register(*reg);
        }
        else if (strncmp("w", buffer, 1) == 0) {
            dialog(reg, sock, client, len, read_size, writing_register);
            dialog(buffer, sock, client, len, read_size, writing_value);
            ret_data = write_register(*reg, *buffer);
        }
        else if (strncmp("exit", buffer, 4) == 0) {
            printf("Server Exit\n");
            break;
        }
        bzero(reg, strlen(reg));
        sprintf(reg, "%u", ret_data);
        if (sendto(sock, (char *)reg, sizeof(reg), 0, (const struct sockaddr *) &client, sizeof(client)) < 0)
            fprintf(stderr, "Error at sending data\n");
    }
    if (close(sock) < 0) {
        fprintf(stderr, "Error at closing socket\n");
        return -1;
    }
    return 0;
}
