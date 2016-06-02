#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>

#define NSTRS       3           /* no. of strings  */
#define ADDRESS  "mysocket"     /* addr to connect */

/*Strings we send to the server.*/

char *strs[NSTRS] = {
    "This is the first string from the client.\n",
    "This is the second string from the client.\n",
    "This is the third string from the client.\n"
                    };
main()
{
    char c;
    FILE *fp;
    register int i, s, len;
    struct sockaddr_un server;

/*Get a socket to work with.  This socket will be in the UNIX domain, and will be a stream socket.*/

    if ((s = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) 
    {
        perror("client: socket");
    }

/*Create the address we will be connecting to.*/

    server.sun_family = AF_UNIX;
    strcpy(server.sun_path, ADDRESS);

/*Try to connect to the address.  For this to succeed, the server must already have bound this address, and must have issued a listen() request.
The third argument indicates the "length" of the structure, not just the length of the socket name.*/

    len = sizeof(server.sun_family) + strlen(server.sun_path);

    if (connect(s, (struct sockaddr *)&server, len) < 0) 
    {
        perror("client: connect");        
    }

/* We'll use stdio for reading the socket. */
     
    fp = fdopen(s, "r");

/* First we read some strings from the server and print them out. */

    for (i = 0; i < NSTRS; i++)
     {
      while ((c = fgetc(fp)) != EOF) 
        {
          putchar(c);
            if (c == '\n')
               break;
        }
     }

/* Now we send some strings to the server.*/
     
    for (i = 0; i < NSTRS; i++)
        send(s, strs[i], strlen(strs[i]), 0);

/* We can simply use close() to terminate the connection, since we're done with both sides.*/
    close(s);

    
}
