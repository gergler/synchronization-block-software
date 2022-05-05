#ifndef SERVER_PARAMETERS_H
#define SERVER_PARAMETERS_H

#if 1
  #define IP "192.168.7.1"
#else
  #define IP "127.1"
#endif

#define PORT    2021
#define MAXLINE 1024

#ifdef USE_IPV6
    #define AF_PROTOCOL AF_INET6
#else
    #define AF_PROTOCOL AF_INET
#endif

#endif // SERVER_PARAMETERS_H
