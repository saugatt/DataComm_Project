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
int mainConverter(char* file_name, char* charType, int* valueArray, int count, int start);


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


int mainConverter(char* file_name, char* charType, int* valueArray, int count, int start)
{
	// vairable declatrations
	int conversionType;									
	int type;										
	int length;		
	// file pointer for write file								
	FILE *fp; 																	
	int i;		

	// Buffer start position
	i = start;								
	conversionType = atoi(charType);		
	type = 9;								
	// this is a write file
	fp = fopen(file_name, "wb");		
	/* 	Checking if the first character of file starts with type format  */

	if (valueArray[i] != 0 && valueArray[i] != 1) {
		return printError(fp, "First byte cannot be other than 0 or 1", file_name);
	}


	// parsing the values and calling conversion
	while (i < count) {

		// if there is no type		
		if (type == 9)
		{							
			type = valueArray[i];										
			i += 1;									
		// conversion of type 0
		}
		else if (type == 0)
		{
			//printf("Type 0 ");
			length = valueArray[i];					
			printf("%d ", length);					
			
			int values[length];					
													
			i += 1;									


			for (int j = 0; j < length; j++) 
			{
				// joining two single byte data
				int result = (valueArray[i] << 8) | valueArray[i+1];
				// print number with ',' or without comma
				if (j == length-1) 
				{
					printf("%d", result);			
				} 
				else
				{
					printf("%d,", result);		
				}
				// adding number to number array
				values[j] = result;				
				i += 2;								
			}

			printf("\n");							
			
			/**	
			 *
			 *	Passing length and values in respective function  
			 *  	
			 *	If the client sends type 3 or 1, it converts the data to first type
			 *	else, it converts the data into second type
			 *
			 */

			if (conversionType == 3 || conversionType == 1)
			{	
				firstTypeConverter(fp, type, length, values);
			} 
			else 
			{
				secondTypeConverter(fp, type, length, values);
			}
			// setting type as default
			type = 9;								
		
		// type 1 processing
		} else if (type == 1) {

			/*  Getting the three bytes length which are sent as character  */

			length = 100*(valueArray[i]-48) + 10*(valueArray[i+1]-48) + (valueArray[i+2]-48);

			printf("%d ", length);					// print length
			
			int values[length];					// number arary for storing values
			i += 3;									// incremnting buffer position

			/*  Getting N(length) values in a loop  */
			
			for (int j=0; j<length; j++) {
				int result = 0;

				// getting each char untill 0, 1 or null
				while (valueArray[i] != 0 && valueArray[i] != 1  && valueArray[i] != '\0') 
				{
					//checking for comma
					if (valueArray[i] == 44) {		
						i += 1;						
						break;
					}

					// getting no from ASCII
					result = result*10 + (valueArray[i]-48); 
					i += 1;							

					//making sure the numer is within tne range
					if (result > 65535 || result < 0) {
						return printError(fp, "The file has invalid number for type 1!", file_name);
					}

				} 


				if ((j < length-1) && (valueArray[i] == 0 || valueArray[i] == 1  || valueArray[i] == '\0')) 
				{

					return printError(fp, "The file has invalid format for format error Type 1 !", file_name);
				}

				if (j == length-1) 
				{
					//last no so no comma
					printf("%d", result);			
				} 
				else {
					printf("%d,", result);			
				}
				
				values[j] = result;				
			}

			printf("\n");							

			/*  If ending character found before reading n characters  */

		
			// type of 3 or 2 is of second format
			if (conversionType == 3 || conversionType == 2)
			{
				secondTypeConverter(fp, type, length, values);
			} 
			else
			{
				firstTypeConverter(fp, type, length, values);
			}
			// setting type as default
			type = 9;	

		// type 1 or 0 not found
		} 

		else 
		{
			return printError(fp, "There seems to error in type!", file_name);
		}

	}

	fclose(fp);		
	// successfully completed								
	return 1;										

}




/* 	Error handler function to print error in server  */

int printError(FILE *fp, char* message, char* file_name) {
	printf("%s\n", message);						// Printing the error is server
	fclose(fp);										// Closing the writing file
	remove(file_name);								// Removing the writing fike
	return 0;										// Returns error code
}







#endif  /*  PG_SOCK_HELP  */

