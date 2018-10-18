/*
Saugat Tripathi
@02752994
*/



#ifndef PG_SOCK_HELP
#define PG_SOCK_HELP


#include <unistd.h>             
#include <sys/socket.h>
#include <errno.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


//backlog for listen 
#define LISTENQ (1024)  

//declarations
int mainConverter(char* file_name, char* char_type, int* data_array, int count, int start);


//convert to type 0 format
void firstTypeConverter(FILE* fp, unsigned char type, int length, int values[]) {

	if (type == 1) 
	{
		fprintf(fp, "%c", type);
	} 
	else 
	{
		fprintf(fp, "%c", type+1);
	}
	// char for geeting the each number in char
	char s;			
	int k;												
	// print the three byte length to file in ascii
	for (k=2; k>=0; k--) 
	{						
		s = length/pow(10, k)+'0';		
		// printing one byte at a time				
		fprintf(fp, "%c", s);						
	}
	// printing number as string to the file
	for (k=0; k<length-1; k++) 
	{
		fprintf(fp, "%d,", values[k]);		
	}
	fprintf(fp, "%d", values[length-1]);		
}

// convert to type 1 format
void secondTypeConverter(FILE* fp, unsigned char type, int length, int values[]) {	
		
	if (type == 0) 
	{
		fprintf(fp, "%c", type);
	}
	else 
	{
		fprintf(fp, "%c", type-1);
	}

	// write one byte of integer to the file
	fwrite(&length, 1, 1, fp);							

	// new array to address the flip issue
	int flip_num[length];

	for (int i =0 ; i < length; i++)
	 {
	 	// Flipping bytes
		flip_num[i] = (values[i]>>8) | (values[i]<<8);
		fwrite(&flip_num[i] , 2, 1, fp);				
	}
	
}





/* 	Error handler function to print error in server  */

int printError(FILE *fp, char* message, char* file_name) {
	printf("%s\n", message);						// Printing the error is server
	fclose(fp);										// Closing the writing file
	remove(file_name);								// Removing the writing fike
	return 0;										// Returns error code
}







#endif  /*  PG_SOCK_HELP  */

