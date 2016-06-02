/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
//#include <sys/types.h>
//#include <netinet/in.h>
#include <winsock.h>
#include <windows.h>
typedef int socklen_t;
#define LENGTH 512
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
     if (argc < 2) 
     {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }

     sockfd = socket(AF_INET, SOCK_STREAM, 0);

     if (sockfd < 0) 
     error("ERROR opening socket");

     //bzero((char *) &serv_addr, sizeof(serv_addr));
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

	 
	char* f_name = "sendserver.txt";
	 char* f1_name = "receiveserver.txt";
	 
	char sdbuf[LENGTH]; // Send buffer
	char revbuf[LENGTH]; // TX buffer
	printf("[server] send %s to the client...", f_name);
	FILE *fp = fopen(f_name, "r");
	if(fp == NULL)
	{
		printf("ERROR: File %s not found.\n", f_name);
		exit(1);
	}
	//bzero(sdbuf, LENGTH);
	int f_block_sz;
	while((f_block_sz = fread(sdbuf, sizeof(char), LENGTH, fp))>0)
	{
		if(send(newsockfd, sdbuf, f_block_sz, 0) < 0)
		{
			printf("ERROR: Failed to send file %s.\n", f_name);
			break;
		}
		
		memset(sdbuf, 0, LENGTH);
		
	}
	printf("ok!\n");
	
	FILE *fp1 = fopen(f1_name, "r");
	if(fp1 == NULL)
	printf("File %s cannot be opened.\n", f_name);
    else
    {
        bzero(revbuf, LENGTH);
        int f_block_sz = 0;
        int success = 0;
        while(success == 0)
        {
            while(f_block_sz = recv(newsockfd, revbuf, LENGTH, 0))
            {
                if(f_block_sz < 0)
                {
                    printf("Receive file error.\n");
                    break;
                }
                int write_sz = fwrite(revbuf, sizeof(char), f_block_sz, fp);
                if(write_sz < f_block_sz)
                {
                    printf("File write failed.\n");
                    break;
                }
                bzero(revbuf, LENGTH);
            }
            printf("ok!\n");
			
            success = 1;
			
			
			fclose(fp);
        }
    }
	
	
	close(newsockfd);
	printf("[server] connection closed.\n");
	
	
        
}

