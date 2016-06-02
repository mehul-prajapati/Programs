#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#define LENGTH 512 
void error(const char *msg)
{
    perror(msg);
    exit(0);
}

/*struct hostent {
    char  *h_name;            official name of host 
    char **h_aliases;          alias list 
    int    h_addrtype;         host address type 
    int    h_length;          length of address 
    char **h_addr_list;        list of addresses 
}

/* struct sockaddr_in {
    sa_family_t    sin_family;  address family: AF_INET 
    in_port_t      sin_port;    port in network byte order 
    struct in_addr sin_addr;    internet address 
   };

/* Internet address. 
  struct in_addr {
           uint32_t s_addr;     /* address in network byte order 
                 };*/
// #define h_addr h_addr_list[0]  /* for backward compatibility */

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
	char revbuf[LENGTH];
    char buffer[256],ch;

    if (argc < 3) 
    {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) 
    error("ERROR opening socket");
    server = gethostbyname(argv[1]);

    if (server == NULL)
    {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr,server->h_length);
    serv_addr.sin_port = htons(portno);

    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
	error("ERROR connecting");
	
	printf("[client] connected to server at port %d...ok!\n", portno);
	
    char* f_name = "receiveclient.txt";
	char* f1_name = "clientsend.txt";
    FILE *fp = fopen(f_name, "a");
	
	
    if(fp == NULL)
	printf("File %s cannot be opened.\n", f_name);
    else
    {
        bzero(revbuf, LENGTH);
        int f_block_sz = 0;
        int success = 0;
        while(success == 0)
        {
            while(f_block_sz = recv(sockfd, revbuf, LENGTH, 0))
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
	
	FILE *fp1 = fopen(f1_name, "a");
	char sd1buf[LENGTH];
	if(fp1 == NULL)
	printf("File %s cannot be opened.\n", f_name);
	else
	{
		int f_block_sz1 = 0;
		while((f_block_sz1 = fread(sd1buf, sizeof(char), LENGTH, fp1))>0)
		{
			if(send(newsockfd, sd1buf, f_block_sz1, 0) < 0)
			{
				printf("ERROR: Failed to send file %s.\n", f_name);
				break;
			}

			fclose(fp);
		}
	}

	
	
    close (sockfd);
    printf("[client] connection lost.\n");
    return (0);
}
    

