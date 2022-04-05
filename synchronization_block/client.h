#ifndef CLIENT_H
#define CLIENT_H

#include "server_parameters.h"
#include "cmd_packet.h"

static void setSocketTimeout( int socket, int sec, int usec );
int execUDPCommand(CMD_Packet req, CMD_Packet &reply, int timeout_us = 800);

#endif // CLIENT_H
