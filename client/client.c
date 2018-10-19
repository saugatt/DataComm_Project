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


//Global constants 

#define LINE_MAXIMUM       (1000)
#define HEADER_LINE        (100)


//Function declarations  
int ParseCmdLine(int argc, char *argv[], char **sAddr, char **sPort, char **readFile, char** type, char** saveFile);
void connectionHandler(int conn_s, char* type, char* readFile, char* saveFile);

int main(int argc, char *argv[]) 
{

    int       conn_s;                //  connection socket         
    short int port;                  // port number               
    char     *endptr;                         
    struct    sockaddr_in servaddr;  //  socket address structure  
    char     *sAddr;                 // remote IP address   
    char      buff[LINE_MAXIMUM];  // character buff       
    char     *sPort;                 //remote port 

    char     *type;                  
    char     *readFile;             
    char     *saveFile;           
    int      state;                

    //  Get command line arguments  
    ParseCmdLine(argc, argv, &sAddr, &sPort, &readFile, &type, &saveFile);


    // argument validation and setup into appropriate struct begins here

    // strstr checkes if type has 0, 1, 2 or 3
    if (strstr("0123", type) == NULL) 
    {
        perror("Only type 0, 1, 2 or 3 is supported by the server");
    }

    // validating the gathered port and setting it to the port variable
    port = strtol(sPort, &endptr, 0);
    if ( *endptr ) 
    {
        printf("The port supplited is not valide\n");
        exit(EXIT_FAILURE);
    }
    
    // creating listening socket
    if ( (conn_s = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) 
    {
        fprintf(stderr, "Could not create a listening socket.\n");
        exit(EXIT_FAILURE);
    }

     // clearing the socket adress with 0's and then filling up it's attributes with the correct value
    memset(&servaddr, 0, sizeof(servaddr));
    // AF_INET designate the type of addresses that your socket can communicate with 
    servaddr.sin_family      = AF_INET;
    servaddr.sin_port        = htons(port);

    //remote IP address is stored in the struct 
    if ( inet_aton(sAddr, &servaddr.sin_addr) <= 0 ) 
    {
    printf("Incorrect remote IP address.\n");
    exit(EXIT_FAILURE);
    }

    if ( connect(conn_s, (struct sockaddr *) &servaddr, sizeof(servaddr) ) < 0 ) 
    {
    printf("Could not call connect()\n");
    exit(EXIT_FAILURE);
    }

    // Read the state from the socket  
    connectionHandler(conn_s, type, readFile, saveFile);

    // printing message base on the state
    read(conn_s, &state, sizeof(int));

    if (state == 0) 
    {
        printf("Format Error\n");
    } 
    else 
    {
        printf("Success\n");
    }

    return EXIT_SUCCESS;
}

int ParseCmdLine(int argc, char *argv[], char **sAddr, char **sPort, char **readFile, char** type, char** saveFile)
{
  // checking if all the arguments are present
    if (argv[1] == NULL) 
    {
        perror("Must provide server address");
        exit(1);
    }

    if (argv[2] == NULL) 
    {
        perror("Must provide port number");
        exit(1);
    }

    if (argv[3] == NULL) 
    {
        perror("Must provide reading file name");
        exit(1);
    }

    if (argv[4] == NULL) 
    {
        perror("Must provide host name");
        exit(1);
    }
  
    if (argv[5]==NULL)
    {
        perror("Must provide file_name.");
        exit(1);
    }

    *sAddr = argv[1];
    *sPort = argv[2];
    *readFile = argv[3];
    *type = argv[4];
    *saveFile = argv[5];

    return 0;
}



// reads data from file, creates the header and sends the data to the server
void connectionHandler(int conn_s, char* type, char* readFile, char* saveFile) 
{

    unsigned char buffer[1];                        // reading buffer initialized
    int valueArray[LINE_MAXIMUM+HEADER_LINE];           // buffer to store data and header
    int count = 1;                                  // intial buffer position
    int i;                                          // looping variable
    int type_size = strlen(type);                   // getting length of type
    int saveFile_size = strlen(saveFile);     // getting length of server file name
    

 // adding type size and type to the value array       
    valueArray[count] = type_size;
    count++;                                       
    for (i = 0; i <type_size; i++) 
    {
        valueArray[count] = type[i];
        count++;
    }

    //Copying file_name size and file_name in value array  
    valueArray[count] = saveFile_size;
    count++;                                         
    for (i=0; i<saveFile_size; i++)
    {
        valueArray[count] = saveFile[i];
        count++;
    }
        
    // read the file 
    FILE *ptr;
    ptr=fopen(readFile,"rb");                    
    // checking error while reading the file
    if (!ptr) 
    { 
        perror("Unable to open file!"); 
        exit(1);
    }

    // checking if the file length exceeds 1000 byets
    fseek(ptr, 0, SEEK_END);
    int lengthOfFile = ftell(ptr);
    rewind(ptr);

    // show error if file length exceeds 1000 bytes    
    if (lengthOfFile > 1000) 
    {
        perror("File length exceed!");
        exit(1);
    }

   //Reading the file 1 byte at a time and saving into valueArray as integer
    while (fread(&buffer,sizeof(buffer),1,ptr) != 0) 
    {
        /*  Saving the file buffer into data array  */
        valueArray[count] = buffer[0];
        count += 1;
    }

    // adding count to the beginning of the buffer
    valueArray[0] = count;                      

    // writing data as integer to the socket
    write(conn_s, &valueArray, sizeof(int)*count);  
    fclose(ptr);                                    

}


