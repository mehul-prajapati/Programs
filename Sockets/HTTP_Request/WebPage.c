//==================================================================
//================= @INCLUDES ======================================
//==================================================================
#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>

//==================================================================
//================= @DEFINES =======================================
//==================================================================
#define BUF_LEN_MAX 		10 * 1024
#define HTTP_PORT 		80
#define ERROR 			-1

//==================================================================
//================= @ENUMS =========================================
//==================================================================

//==================================================================
//================= @DATATYPES =====================================
//==================================================================
void i_get_home_page (int socket_fd);

//==================================================================
//================= @FUNCTION PROTOTYPE ============================
//==================================================================


//==================================================================
//================= @FUNCTION DEFINITION ===========================
//==================================================================
int main (int argc, char *argv[])
{
	int socket_fd;
	struct sockaddr_in sockAddr;
	struct hostent *pHostInfo = NULL;
	
	/* Create the socket. */
	socket_fd = socket (AF_INET, SOCK_STREAM, 0);
	
	/* Store the server's sockAddr in the socket address. */
	sockAddr.sin_family = AF_INET;
	
	if (argc != 2)
	{
		printf ("No arguments\n");
		return (1);	
	}

	/* Convert from strings to numbers. */
	pHostInfo = gethostbyname(argv[1]);
	
	if (pHostInfo == NULL)
	{
		return (1);
	}
	else
	{
		sockAddr.sin_addr = *((struct in_addr *) pHostInfo->h_addr);
		
		/* Web servers use port 80. */
		sockAddr.sin_port = htons (HTTP_PORT);
	}

	/* Connect to the Web server */
	if (ERROR == connect (socket_fd, (struct sockaddr *)&sockAddr, sizeof(struct sockaddr_in))) 
	{
		perror ("connect");
		return (1);
	}

	/* Retrieve the server’s home page. */
	i_get_home_page (socket_fd);

	return (0);
}


/* Print the contents of the home page for the server’s socket.
Return an indication of success. */
void i_get_home_page (int socket_fd)
{
	char buffer[BUF_LEN_MAX] = "";
	ssize_t number_characters_read;

	/* Send the HTTP GET command for the home page. */
	sprintf (buffer, "GET \n");
	write (socket_fd, buffer, strlen (buffer));

	/* Read from the socket. The call to read may not
	return all the data at one time, so keep
	trying until we run out. */
	    
	while (1) 
	{
		number_characters_read = read (socket_fd, buffer, BUF_LEN_MAX);
		
		if (number_characters_read == 0)
		{
			return;
		}

		/* Write the data to standard output. */
		//fwrite (buffer, sizeof (char), number_characters_read, stdout);
		buffer[number_characters_read] = '\0';
		printf("****************************\n");
		printf("%s", buffer);
	}
}


