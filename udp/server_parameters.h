#define PORT    2021
#define MAXLINE 1024

//#define USE_IPV6

#ifdef USE_IPV6
    #define AF_PROTOCOL AF_INET6
#else
    #define AF_PROTOCOL AF_INET
#endif
