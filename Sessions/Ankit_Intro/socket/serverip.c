/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

/*struct sockaddr_in {
    sa_family_t    sin_family; address family: AF_INET 
    in_port_t      sin_port;   port in network byte order 
    struct in_addr sin_addr;   internet address 
};*/

/* Internet address. 
struct in_addr {
    uint32_t       s_addr;      address in network byte order 
};*/

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno;
     socklen_t clilen;
     char buffer[256];
     struct sockaddr_in serv_addr, cli_addr;
     int n;
	 char operator;	
	char * pch;
	char * pch1;
	char * pch2;
	
     if (argc < 2) 
     {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }

     sockfd = socket(AF_INET, SOCK_STREAM, 0);

     if (sockfd < 0) 
		error("ERROR opening socket");

     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);

     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);

     if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) // Bind
		error("ERROR on binding");

     listen(sockfd,5);    //  Listen
     clilen = sizeof(cli_addr);

     newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr,&clilen);  // Accept

     if (newsockfd < 0) 
		error("ERROR on accept");

     bzero(buffer,256);
     n = read(newsockfd,buffer,255);

     if (n < 0) 
		error("ERROR reading from socket");
	
	buffer[strlen(buffer) - 1] = 0; /* replace newline with null */
	printf ("Splitting string \"%s\" into tokens:\n",buf);
	pch = strtok (buffer," ");
	printf("value of pch is %s\n",pch);
	printf("value of buf is %s\n",buf);
	pch1 = strtok (NULL," ");		
	printf("value of pch1 is %s\n",pch1);
	printf("value of buf is %s\n",buf);
	pch2 = strtok (NULL," ");		
	printf("value of pch2 is %s\n",pch2);
	printf("value of buf is %s\n",buf);
	num1=atoi(pch);
	num2=atoi(pch2);

	printf("num3 is %c",(*pch1));

	if(*pch1 == '+')
	{
		total=(num1)+(num2);
	}
	else if(*pch1 == '-')
	{
		total=(num1)-(num2);
	}
	else if(*pch1 == '*')
	{
		total=(num1)*(num2);
	}
	else if(*pch1 == '/')
	{
		total=(num1)/(num2);
	}
	else
	{ 
		printf("invalid operator");
	}

	printf("Total is %d:",total);

		
		
     printf("Here is the message: %s\n",buffer);
     n = write(newsockfd,(char)total,18);

     if (n < 0) 
     error("ERROR writing to socket");
     close(newsockfd);
     close(sockfd);
     return 0; 
}
