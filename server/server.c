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


#define ECHO_PORT          (5002)
#define MAX_LINE           (1000)


// declearation
int connectionHandler(int conn_s);


int main(int argc, char *argv[]) {
    int       list_s;                /*  listening socket          */
    int       conn_s;                /*  connection socket         */
    short int port;                  /*  port number               */
    struct    sockaddr_in servaddr;  /*  socket address structure  */
    char      buffer[MAX_LINE];      /*  character buffer          */
    char     *endptr;                /*  for strtol()              */
    int       state;                 /*  state is 0 or 1 faliure or sucess     */


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


    // Put everything in the  socket address structure to zero

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

    
    // infinte loop to accept the connection from the client
    while ( 1 ) {

    /*  Wait for a connection, then accept() it  */

    if ( (conn_s = accept(list_s, NULL, NULL) ) < 0 ) 
    {
        fprintf(stderr, "SERVER: Could not accept()\n");
        exit(EXIT_FAILURE);
    }

    //  Connection Haldler for reading from client 
    state = connectionHandler(conn_s);

    //  Writes the state(success/failure) to the socket  

    write(conn_s, &state, sizeof(int));

    if (close(conn_s) < 0 ) 
    {
        fprintf(stderr, "SERVER: Could not close close()\n");
        exit(EXIT_FAILURE);
    }
    }
}


int connectionHandler(int conn_s) {
    // total buff size
    int count;                          
    int i;                              
    int buff_indx;
    // success state                        
    int state;                   
    int typeSize;                     
    int fileNameSize;              
    int buffer[MAX_LINE];               


    //  Reading fron the socket into buffer  
    if ((read(conn_s, &buffer, sizeof(int)*MAX_LINE)) <= 0) {
        return 0;
    } 

    //Length of buffer 
    count = buffer[0];
    // start of buffer index
    buff_indx = 1;                            

    //Getting size of types
    typeSize = buffer[buff_indx];           
    buff_indx += 1;                           

    // Reading type from buffer 
    char type[typeSize+1];            
    for (i=0; i<typeSize; i++)
    {      
        type[i] = (char) buffer[i+buff_indx];
    }
    // Terminating with null to make string
    type[typeSize+1] = '\0';          
    buff_indx += typeSize;                  

    // getting the size of the filename
    fileNameSize = buffer[buff_indx];      
    buff_indx += 1;                           

    //reading file name from buffer
    char file_name[fileNameSize+1];
    for (i=0; i<fileNameSize; i++) {
        file_name[i] = (char) buffer[i+buff_indx];
    }
    file_name[fileNameSize + 1] = '\0';
    // increasing the buffer location
    buff_indx += fileNameSize;             
    // adding terminater to the buffer
    buffer[count] = '\0';               

    // file to convert  
    state = mainConverter(file_name, type, buffer, count, buff_indx);

    return state;
}
