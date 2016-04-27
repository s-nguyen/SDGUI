#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

class TCP_Client
{

public:
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;


    TCP_Client();
    ~TCP_Client();
    void tcp_init();
    int tcp_write(uint8_t *buf, int len);
    int tcp_read(uint8_t *buf, int len);
};

#endif // TCP_CLIENT_H
