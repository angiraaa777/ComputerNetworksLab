
/*UDP Echo Server Program*/

#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define n 256

struct sockaddr_in serv_addr, cli_addr;
int listenfd, connfd, r, w;
socklen_t cli_addr_len;

unsigned short serv_port = 25021; /*port number to be used by the server*/
char serv_ip[] = "127.0.0.1";     /*server's IP-address*/

char buffer[n]; /*buffer for sending and receiving messages*/

int length = sizeof(struct sockaddr_in);

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

    printf("Live Chat Server");

    if ((listenfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        printf("SERVER ERROR: Can't create socket");
        exit(1);
    }

    if (bind(listenfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("SERVER ERROR: Can't bind\n");
            close(listenfd);
        exit(1);
    }

    int maxCli = 5;

    cli_addr_len = sizeof(cli_addr);

    printf("______________________________________________________________________\n");

    for (int client = 1; client <= maxCli; client++)
    {
        printf("________________________________________________\n");

        printf("SERVER: Listening for Client %d....\n", client);
        printf("\nSERVER: Connection from client accepted.\n");

        while (1)
        {
            printf("Waiting for Client....\n");
            bzero(buffer, n);

            if (recvfrom(listenfd, buffer, n, 0, (struct sockaddr *)&serv_addr, &length) < 0)
            {
                printf("SERVER ERROR: Can't read\n");
                break;
            }

            printf("SERVER: Cmd sent by Client: %s \n", buffer);

            if (strncmp(buffer, "exit", 4) == 0)
            {
                printf("EXIT\n");
                break;
            }

            buffer[strlen(buffer) - 1] = ' ';
            strcat(buffer, " > temp.txt");

            int isDone = system(buffer);

            if (isDone == 0)
            {
                FILE *fp = fopen("./temp.txt", "r");
                bzero(buffer, n);
                if (fp == NULL)
                {
                    printf("SERVER: Cant Read res.\n");
                }
                else
                {
                    while (fgets(buffer, n, fp) != NULL)
                    {
                        if (sendto(listenfd, buffer, n, 0, (struct sockaddr *)&serv_addr, length) < 0)
                        {
                            printf("SERVER : Can't send message to live chat client.\n");
                            close(listenfd);
                            close(connfd);
                            exit(1);
                        }
                    }

                    bzero(buffer, n);
                    strcpy(buffer, "DONE");
                    if (sendto(listenfd, buffer, n, 0, (struct sockaddr *)&serv_addr, length) < 0)
                    {
                        printf("SERVER: Can't send to Client\n");
                        close(listenfd);
                        close(connfd);
                        exit(1);
                    }

                    printf("SERVER: Response sent to Client.\n");
                        fclose(fp);
                }
            }
            else
            {

                printf("SERVER ERROR: Invalid cmd");
                bzero(buffer, n);
                strcpy(buffer, "ERROR: Invalid cmd");

                if (sendto(listenfd, buffer, n, 0, (struct sockaddr *)&serv_addr, length) < 0)
                {
                    printf("SERVER: Can't send to Client\n");
                    close(listenfd);
                    close(connfd);
                    exit(1);
                }
            }

            system("rm temp.txt");
        }

        close(connfd);
        printf("__________________________________________\n");
    }

    close(listenfd);
}
