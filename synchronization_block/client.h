#ifndef CLIENT_H
#define CLIENT_H

#include "server_parameters.h"
#include "cmd_packet.h"

static void set_socket_timeout( int socket, int sec, int usec );
int exec_UDP_command(CMD_Packet req, CMD_Packet &reply, int timeout_us = 800);

#endif // CLIENT_H
