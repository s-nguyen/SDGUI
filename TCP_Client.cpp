#include "TCP_Client.h"
//https://developer.ridgerun.com/wiki/index.php/How_to_use_USB_device_networking
//http://www.linuxhowtos.org/C_C++/socket.htm
TCP_Client::TCP_Client()
{

}

TCP_Client::~TCP_Client() {
    close(sockfd);
}

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

void TCP_Client::tcp_init() {
    //Port Number for this project
    portno = 15006;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    server  = gethostbyname("10.0.1.2");
    if (server == NULL) {
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR connecting");
}

void TCP_Client::tcp_read(uint8_t *buf) {
    //bzero(buf, 9840);
    n = read(sockfd, buf, 9840);
    if (n < 0)
        error("ERROR reading from socket");
}

void TCP_Client::tcp_write(uint8_t *buf) {
    //bzero(buf, 9840);
    n = write(sockfd, buf, 9840);
    if (n < 0)
        error("ERROR reading from socket");
}
