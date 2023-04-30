
#include "socket-common.h"
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdbool.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(void)
{
    /*
        The server accepts, spawns a child, and reads from the socket and interprets the client’s request
        for its validity.
    */
    int socketFd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serverAddress = {
        .sin_family = AF_INET,
        .sin_port = htons(PORT),
        .sin_addr.s_addr = htonl(INADDR_ANY),
    };
    int bindResult = bind(socketFd, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
    int listenResult = listen(socketFd, 1);
    while (1)
    {
        int clientFd = accept(socketFd, NULL, NULL);
        char action;
        while (1)
        {
            int res = read(clientFd, &action, sizeof(action));
            if (res < 0)
            {
                printf("client disconnected\n");
                break;
            }
            printf("cmd: %d\n", action);
            switch (action)
            {
            case 'M':
            {
                printf("map request\n");
                struct mapRequest request;
                read(clientFd, &request, sizeof(request));
                printf("width: %d\n", request.width);
                printf("height: %d\n", request.height);
                shutdown(clientFd, SHUT_RDWR);
                break;
            }
            default:
            {
                printf("unknown command\n");
                exit(1);
                break;
            }
            }
        }
    }
}