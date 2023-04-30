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
    struct sockaddr_in serverAddress;
    int ipAddress = IP;
    int port = PORT;

    int socketFd = socket(AF_INET, SOCK_STREAM, 0);

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    inet_pton(AF_INET, ipAddress, &serverAddress.sin_addr);

    int connectResult = connect(socketFd, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
    write(socketFd, "M", 1);
    struct mapRequest request = {
        .width = 10,
        .height = 10,
    };
    write(
        socketFd,
        &request,
        sizeof(request));
}
