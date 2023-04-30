#include "socket-common.h"
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdio.h>
#include <signal.h>
#include <getopt.h>
#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/prctl.h>

int main(int argc, char *argv[])
{
    printf("Starting client\n");
    struct sockaddr_in serverAddress;
    char *ipAddress = IP;
    int port = PORT;

    printf("Creating socket\n");
    int socketFd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFd < 0)
    {
        printf("Error creating socket\n");
        exit(1);
    }
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);

    printf("Converting IP address\n");
    inet_pton(AF_INET, ipAddress, &serverAddress.sin_addr);

    printf("Connecting to server\n");
    int connectResult = connect(socketFd, (struct sockaddr *)&serverAddress, sizeof(serverAddress));

    if (connectResult < 0)
    {
        printf("Error connecting to server\n");
        exit(2);
    }

    int res = 0;
    printf("Sending map request\n");
    res = write(socketFd, "M", 1);
    if (res < 0)
    {
        printf("Error sending map request\n");
        exit(3);
    }
    struct mapRequest request = {
        .width = 10,
        .height = 10,
    };
    res = write(
        socketFd,
        &request,
        sizeof(request));
    if (res < 0)
    {
        printf("Error sending map request\n");
        exit(3);
    }
}
