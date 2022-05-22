
/*TCP Echo Client Program*/

#include<fcntl.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>

struct sockaddr_in serv_addr;
                        
int skfd, r, w;

unsigned short serv_port = 25020; /*port number used by the server*/
char serv_ip[] = "127.0.0.1"; /*server's IP-address*/
               
char rbuff[512]; /*buffer for receiving messages*/
char sbuff[512] = "Hello Angira"; /*buffer for sending messages*/

int main()
{

	/*initializing server socket address structure with zero values*/
	bzero(&serv_addr, sizeof(serv_addr));

        /*filling up the server socket address structure with appropriate values*/
        serv_addr.sin_family = AF_INET; /*address family*/
        serv_addr.sin_port = htons(serv_port); /*port number*/
        inet_aton(serv_ip, (&serv_addr.sin_addr)); /*IP-address*/
	
	printf("\nTCP ECHO CLIENT.\n");

        /*creating socket*/
        if((skfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        {
                printf("\nCLIENT ERROR: Cannot create socket.\n");
                exit(1);
        }

	/*request server for a connection*/
	if((connect(skfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) < 0)
	{
		printf("\nCLIENT ERROR: Cannot connect to the server.\n");
		close(skfd);
		exit(1);
	}
	printf("\nCLIENT: Connected to the server.\n");

	/*send a message to the echo server*/
	if((w = write(skfd, sbuff, 128)) < 0)       	
	{
		printf("\nCLIENT ERROR: Cannot send message to the echo server.\n");
		close(skfd);
		exit(1);
	}
	printf("\nCLIENT: Message sent to echo server.\n");

	while(1)
	{
		printf("Enter the message to send\n");
		scanf("%s",sbuff);
		
		if((w=write(skfd,sbuff,512))<0)
		{
		
			printf("Error: Cannot send Message\n");
			close(skfd);
			exit(1);
		}
		
		if((r=read(skfd, rbuff, 512))<0)
		{
			rbuff[0]='\0';
		}
		
		else
		{
			read(skfd,&rbuff,sizeof(rbuff)); 
			printf("Result from server=%s\n",rbuff); 
			
		}
		
		close(skfd);
		exit(0);
		
	}					
} /*main ends*/


