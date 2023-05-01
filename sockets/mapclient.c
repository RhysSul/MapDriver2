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

int generateMap(int socketFd)
{
    int res = 0;
    // Sends a request consisting of the ASCII character ‘M’
    printf("Sending map generation request\n");
    res = write(socketFd, "M", 1);
    if (res < 0)
    {
        printf("Error sending map request\n");
        exit(3);
    }

    // Followed by either binary 0 (a single int) or two binary values, W IDT H and HEIGHT (two ints).

    struct mapRequest request;
    request.width = 10;
    request.height = 10;
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

int readMap(int socketFd)
{
    // (c) The client reads and interprets the reply. A map is output then to STDOUT, the error to STDERR.

    char action;
    printf("Reading map read request\n");
    int res = read(socketFd, &action, sizeof(action));
    if (res < 0)
    {
        printf("Error reading map request\n");
        exit(4);
    }
    printf("action: %c\n", action);

    switch (action)
    {
    case 'M':
    {
        // get the map size
        struct mapRequest request;
        printf("Reading map size\n");
        res = read(socketFd, &request, sizeof(request));
        if (res < 0)
        {
            printf("Error reading map request\n");
            exit(4);
        }
        // read the map
        printf("Reading map of size %d x %d\n", request.width, request.height);
        char map[request.width * request.height];
        res = read(socketFd, map, 4);
        if (res < 0)
        {
            printf("Error reading map request\n");
            exit(4);
        }
        // print the map
        printf("%s\n", map);

        break;
    }
    case 'E':
    {

        break;
    }
    default:
        break;
    }
}

int main(int argc, char *argv[])
{
    printf("Starting client\n");
    struct sockaddr_in serverAddress;
    char *ipAddress = IP;
    int port = PORT;
    // TODO: Allow IP override via argv (on both client and server programs).

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

    generateMap(socketFd);
    readMap(socketFd);
}
