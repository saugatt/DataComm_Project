/*
Saugat Tripathi
@02752994

*/


#include <sys/types.h>      /*  socket types           */
#include <unistd.h>         /*  misc. UNIX functions   */
#include <arpa/inet.h>      /*  inet (3) funtions      */
#include <sys/socket.h>     /*  socket definitions      */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>


/*  Global constants  */

#define LINE_MAXIMUM       (1000)
#define HEADER_LINE        (100)


/*  Function declarations  */

int ParseCmdLine(int argc, char *argv[], char **sAddr, char **sPort, char **readFile, char** type, char** saveFile);


/*  main()  */

int main(int argc, char *argv[]) {

    char     *type;                 // type variable                    
    char     *readFile;             //  file to read by client           
    char     *saveFile;             //  file name to be saved at server  
    int      state;                 //   state of process  

    int       conn_s;                //  connection socket         
    short int port;                  // port number               
    char     *endptr;                         
    struct    sockaddr_in servaddr;  //  socket address structure  
    char     *sAddr;                 // remote IP address   
    char      buff[LINE_MAXIMUM];  // character buff       
    char     *sPort;                 //remote port 
   



    /*  Get command line arguments  */

    ParseCmdLine(argc, argv, &sAddr, &sPort, &readFile, &type, &saveFile);


    // argument validation and setup into appropriate struct begins here

    // strstr checkes if type has 0, 1, 2 or 3

   if (strstr("0123", type) == NULL) 
    {
        perror("Only type 0, 1, 2 or 3 is supported by the server");
        exit(1);
    }

    // validating the gathered port and setting it to the port variable
     port = strtol(sPort, &endptr, 0);
    if ( *endptr ) 
    {
        printf("ECHOCLNT: The port supplited is not valide\n");
        exit(EXIT_FAILURE);
    }

	// creating listening socket
    if ((conn_s = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) 
    {
        fprintf(stderr, "ECHOCLNT: Could not create a listening socket.\n");
        exit(EXIT_FAILURE);
    }

     // clearing the socket adress with 0's and then filling up it's attributes with the correct value
    memset(&servaddr, 0, sizeof(servaddr));
    // AF_INET designate the type of addresses that your socket can communicate with 
    servaddr.sin_family      = AF_INET;
    servaddr.sin_port        = htons(port);


    /*  Set the remote IP address  */

    if ( inet_aton(sAddr, &servaddr.sin_addr) <= 0 ) {
	printf("ECHOCLNT: Invalid remote IP address.\n");
	exit(EXIT_FAILURE);
    }

    
    /*  connect() to the remote echo server  */

    //remote IP address is stored in the struct 
    if ( inet_aton(sAddr, &servaddr.sin_addr) <= 0 )
    {
        printf("ECHOCLNT: Invalid remote IP address.\n");
        exit(EXIT_FAILURE);
    }
 
    return EXIT_SUCCESS;
}





int ParseCmdLine(int argc, char *argv[], char **sAddr, char **sPort, char **readFile, char** type, char** saveFile) {

  // checking if all the arguments are present

    if (argv[1] == NULL) 
    {
        perror("Server address missing");
        exit(1);
    }

    if (argv[2] == NULL)
    {
        perror("Port number missing");
        exit(1);
    }

    if (argv[3] == NULL) 
    {
        perror("Reading file name missing");
        exit(1);
    }

    if (argv[4] == NULL)
    {
        perror("Host name missing");
        exit(1);
    }
  
    if (argv[5]==NULL) 
    {
        perror("File name of output file missing");
        exit(1);
    }
   
    *sAddr = argv[1];
    *sPort = argv[2];
    *readFile = argv[3];
    *type = argv[4];
    *saveFile = argv[5];

    return 0;
}



