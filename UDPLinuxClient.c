
/*UDP Echo CLIENT Program*/

#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define n 256

struct sockaddr_in serv_addr;
int skfd, r, w;

unsigned short serv_port = 25021; /*port number to be used by the CLIENT*/
char serv_ip[] = "127.0.0.1";     /*CLIENT's IP-address*/

int length = sizeof(struct sockaddr_in);
char buffer[n]; /*buffer for sending and receiving messages*/

int main(int argc, char *argv[])
{
    if (argc > 1)
    {
        serv_port = atoi(argv[1]);
        printf("SERVER PORT no. : %d \n ", serv_port);
    }

    if (argc > 2)
    {
        strcpy(serv_ip, argv[2]);
        printf("SERVER ip address: %s \n", serv_ip);
    }

    bzero(&serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(serv_port);
    inet_aton(serv_ip, (&serv_addr.sin_addr));

    printf("Live Chat Client\n");

    if ((skfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        printf("CLIENT ERROR: Can't create socket");
        exit(1);
    }

    printf("CLIENT: Connected to the Server\n");

    while (1)
    {
        printf("CLIENT: Write cmd: \n");
        bzero(buffer, n);
        fgets(buffer, n, stdin);

        if (sendto(skfd, buffer, n, 0, (struct sockaddr *)&serv_addr, length) < 0)
        {
            printf("SERVER: Can't send messages to the live server.\n");
            close(skfd);
            exit(1);
        }

        printf("CLI: Cmd sent\n");

        if (strncmp(buffer, "exit", 4) == 0)
        {
            printf("EXIT\n");
            break;
        }

        printf("Waiting for SERVER..... \n");

        while (1)
        {
            bzero(buffer, n);
            if (recvfrom(skfd, buffer, n, 0, (struct sockaddr *)&serv_addr, &length) < 0)
            {
                printf("CLIENT ERROR\n");
                bzero(buffer, n);
                strcpy(buffer, "exit");

                if (sendto(skfd, buffer, n, 0, (struct sockaddr *)&serv_addr, length) < 0)
                {
                    close(skfd);
                    exit(1);
                }
            }

            if (strncmp(buffer, "DONE", 4) == 0)
                break;
            printf("%s", buffer);
        }

        printf("___________________________\n");
    }

    close(skfd);
    return 0;
}
