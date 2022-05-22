     
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
    #include <time.h>
     
    struct sockaddr_in serv_addr, cli_addr;
    int listenfd, connfd, r, w, val, cli_addr_len;
     
    unsigned short serv_port = 25021; /*port number to be used by the server*/
    char serv_ip[] = "127.0.0.1";	  /*server's IP-address*/
     
    char buff[256]; /*buffer for sending and receiving messages*/
    time_t ticks;
    struct tm gtime;
    
    int length=sizeof(struct sockaddr_in);
    int n=128;
     
    int main()
    {
    	
    	bzero(&serv_addr, sizeof(serv_addr));
     
    	
    	serv_addr.sin_family = AF_INET;			   
    	serv_addr.sin_port = htons(serv_port);	   /*port number*/
    	inet_aton(serv_ip, (&serv_addr.sin_addr)); /*IP-address*/
     
    	printf("\nUDP ECHO SERVER.\n");
     
    	/*creating socket*/
    	if ((listenfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    	{
    		printf("\nSERVER ERROR: Cannot create socket.\n");
    		exit(1);
    	}
     
    	/*binding server socket address structure*/
    	if (bind(listenfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    	{
    		printf("\nSERVER ERROR: Cannot bind.\n");
    		close(listenfd);
    		exit(1);
    	}
     
    	/*listen to client connection requests*/
     
    	cli_addr_len = sizeof(cli_addr);
    	
    	for( ; ;)
    	{
    		printf("\n SERVER: Listening for clients\n");
    		
    		if(recvfrom(listenfd, buff, n, 0, (struct sockaddr*)&serv_addr, &length) <0)
    		{
    			printf("\n SERVER ERROR\n");
    		}
    		
    		else
    		{
    			printf("\nSERVER:Connection from client accepted\n");
    			printf("SERVER client: %s \n",buff);
    			bzero(buff,n);
    			
    			ticks=time(NULL);
    			localtime_r(&ticks,&gtime);
    			asctime_r(&gtime,buff);
    			printf("%s",buff);
    			
    			if(sendto(listenfd,buff,n,0,(struct sockaddr*)&serv_addr,length)<0)
    			{
    				printf("Cannot send message\n");
    				close(connfd);
    				exit(1);
    			}
    			
    			printf("SERVER: Time sent to client.\n");
    			
    }
    
 }
 
 return 0;
 
 }   					
