
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
        /*
        The server accepts, spawns a child, and reads from the socket and interprets the client’s request
        for its validity.
        */
        int clientFd = accept(socketFd, NULL, NULL);
        pid_t pid = fork();
        if (pid < 0)
        {
            printf("fork failed\n");
            exit(1);
        }
        while (1)
        {
            /*
            The server either replies with an ‘M’ or ‘E’ messages, i.e., the map or error to the client’s socket.

            The ‘M’ message in the protocol always has ‘M’ followed by W IDT H and HEIGHT, followed by the entire map.

            The ‘E’ message is followed by a single int, which is the length of the error message string that follows.
            */
            char action = 0;
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

                printf("Sending map generation notification\n");
                write(clientFd, "M", 1);

                printf("Generating map\n");
                // Generate Map:
                int driverFid, res;
                driverFid = open("/dev/asciimap", O_RDWR);
                if (driverFid < 0)
                {
                    printf("Error opening device file\n", driverFid);
                    return -1;
                }

                printf("Sending map dimensions\n");
                // send dimensions
                res = write(clientFd, &request, sizeof(request));
                if (res < 0)
                {
                    printf("Error writing to device file\n");
                    return -1;
                }

                printf("Reading map from driver\n");
                char buffer[BUFSIZ];
                int amount_read = read(driverFid, buffer, BUFSIZ);
                if (amount_read < 0)
                {
                    printf("Error reading from device file\n");
                    return -1;
                }

                // Send the map:
                printf("Sending map to client\n");
                write(clientFd, buffer, amount_read);

                printf("Closing driver\n");
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