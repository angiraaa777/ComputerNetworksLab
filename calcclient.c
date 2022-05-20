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
               
char rbuff[256]; /*buffer for receiving messages*/
char sbuff[256] = "===good marning==="; /*buffer for sending messages*/

int main()
{
	int operator;
float op1,op2,result;
int sin_size;
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

	sin_size = sizeof(struct sockaddr_in);
while(1)
{	
printf("***** CALCULATOR ********");
printf("\nEnter operation:\n Press 1 for Addition \n Press 2 for Subtraction \n Press 3 for Multiplication \n Press 4 for Division\n");
printf("Press 5 to exit!\n");

	
printf("Enter operands:\n");
scanf("%f %f", &op1, &op2);

printf("Choose the operation\n");
scanf("%d",&operator);	



write(skfd,&operator,sizeof(operator));
write(skfd,&op1,sizeof(op1));
write(skfd,&op2,sizeof(op2));
if((op1==0 || op2==0) && (operator==3))
{

printf("Error! Division by 0 not possible!\n");

}

else if(operator==5)
{

	exit(1);


}

else
{
read(skfd,&result,sizeof(result)); 
printf("Result from server=%f\n",result); 

}

}
 
close(skfd);
} /*main ends*/
