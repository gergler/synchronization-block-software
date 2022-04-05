#include <stdint.h>

struct CMD_Packet {
    char     cmd:32;
    uint32_t number;
    uint32_t  value;
    uint32_t status;
};

void print_packet(CMD_Packet &packet) {
    printf("%c N=0x%x V=0x%x S=0x%x \n",
        packet.cmd, packet.number, packet.value, packet.status);
}
