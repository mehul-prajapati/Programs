#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>

#define NSTRS       3           /* no. of strings  */
#define ADDRESS  "mysocket"     /* addr to connect */

/* Strings we send to the client.*/

char *strs[NSTRS] = {
    "This is the first string from the server.\n",
    "This is the second string from the server.\n",
    "This is the third string from the server.\n"
                    };
main()
{
    char c;
    FILE *fp;
    int fromlen;
    register int i, s, ns, len;
    struct sockaddr_un server, client;

/* Get a socket to work with.  This socket will be in the UNIX domain, and will be a stream socket.*/

    if ((s = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
    {
        perror("server: socket");
    }

/* Create the address we will be binding to.*/
    server.sun_family = AF_UNIX;
    strcpy(server.sun_path, ADDRESS);

/* Try to bind the address to the socket.We unlink the name first so that the bind won't fail.
   The third argument indicates the "length" of the structure,not just the length of the socket name.*/
     
    unlink(ADDRESS);
    len = sizeof(server.sun_family) + strlen(server.sun_path);

    if (bind(s, (struct sockaddr *)&server, len) < 0) 
    {
        perror("server: bind");       
    }

    
    if (listen(s, 5) < 0)          /* Listen on the socket.*/
    {                      
        perror("server: listen");
        
    }

/*Accept connections. When we accept one, ns will be connected to the client.client will contain 
the address of the client.*/

    if ((ns = accept(s, (struct sockaddr *)&client, &fromlen)) < 0) 
    {
        perror("server: accept");   
    }

    /* We'll use stdio for reading the socket.*/
    fp = fdopen(ns, "r");

    /* First we send some strings to the client.*/
    for (i = 0; i < NSTRS; i++)
        send(ns, strs[i], strlen(strs[i]), 0);

    /*Then we read some strings from the client and print them out.*/
    for (i = 0; i < NSTRS; i++)
       {
        while ((c = fgetc(fp)) != EOF) 
            {
            putchar(c);
               if (c == '\n')
                    break;
            }
       }

    /*We can simply use close() to terminate the connection, since we're done with both sides.*/
    close(s);  
}

