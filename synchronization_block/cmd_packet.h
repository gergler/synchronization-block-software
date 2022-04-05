#ifndef CMD_PACKET_H
#define CMD_PACKET_H

#include <stdint.h>

struct CMD_Packet {
    char     cmd:32;
    uint32_t number;
    uint32_t  value;
    uint32_t status;
};

#endif // CMD_PACKET_H
