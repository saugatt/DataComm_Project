/*
Saugat Tripathi
@02752994
*/


#include <sys/socket.h>       /*  socket definitions        */
#include <sys/types.h>        /*  socket types              */
#include <arpa/inet.h>        /*  inet (3) funtions         */
#include <unistd.h>           /*  misc. UNIX functions      */

#include "helper.h"           /*  our own helper functions  */

#include <stdlib.h>
#include <stdio.h>


/*  Global constants  */
#define ECHO_PORT          (5002)
#define MAX_LINE           (1000)


/* Function Decleration */

int connectionHandler(int conn_s);


int main(int argc, char *argv[]) {
    int       list_s;                /*  listening socket          */
    int       conn_s;                /*  connection socket         */
    short int port;                  /*  port number               */
    struct    sockaddr_in servaddr;  /*  socket address structure  */
    char      buffer[MAX_LINE];      /*  character buffer          */
    char     *endptr;                /*  for strtol()              */


    //getting the port number and seeting it
    if ( argc == 2 ) {
    port = strtol(argv[1], &endptr, 0);
    if ( *endptr ) {
        fprintf(stderr, "SERVER: Invalid port number.\n");
        exit(EXIT_FAILURE);
    }
    }
    else if ( argc < 2 ) {
    port = ECHO_PORT;
    }
    else {
    fprintf(stderr, "SERVER: Invalid arguments.\n");
    exit(EXIT_FAILURE);
    }

    
    // create a listening socket
    if ( (list_s = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
    fprintf(stderr, "SERVER: Error creating listening socket.\n");
    exit(EXIT_FAILURE);
    }


    /*  Set all bytes in socket address structure to
        zero, and fill in the relevant data members   */

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family      = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port        = htons(port);


    /*  Bind our socket addresss to the 
    listening socket, and call listen()  */

    if ( bind(list_s, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0 )
    {
    fprintf(stderr, "SERVER: Could not call bind()\n");
    exit(EXIT_FAILURE);
    }

    if ( listen(list_s, LISTENQ) < 0 ) 
    {
    fprintf(stderr, "SERVER: Could not call listen()\n");
    exit(EXIT_FAILURE);
    }

    
    /*  Enter an infinite loop to respond
        to client requests and echo input  */

    while ( 1 ) {

    /*  Wait for a connection, then accept() it  */

    if ( (conn_s = accept(list_s, NULL, NULL) ) < 0 ) 
    {
        //printf("Connection accepted");
        fprintf(stderr, "SERVER: Could not accept()\n");
        exit(EXIT_FAILURE);
    }

    
}

}

