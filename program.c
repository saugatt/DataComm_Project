#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

void writeFirstType(FILE* filename, unsigned char type, int length, int numbers[]) {

	fprintf(filename, "%c", type+1);
	char stringNumber[3];
	char currentNum;
	// writing the the length in ASCII
	for (int i = 2; i >= 0; i--) {
		// Converting length to a  3 byte ASCII with a comma
		currentNum = length/pow(10, i)+'0';
		fprintf(filename, "%c", currentNum);
	}

	for (int i = 0; i < length-1; i++) {
		fprintf(filename, "%d,", numbers[i]);
	}
	// Writing last number without a comma
	fprintf(filename, "%d", numbers[length-1]);	
	
}

void writeSecondType(FILE* filename, unsigned char type, int length, int numbers[]) {	
	// writing type character
	fprintf(filename, "%c", type-1);
	// fwrite writes integer byte
	fwrite(&length, 1, 1, filename);
	// flipping the length to match the order
	int flip_num[length];

	for (int i = 0; i < length; i++) {
		flip_num[i] = (numbers[i]>>8) | (numbers[i]<<8);
		fwrite(&flip_num[i] , 2, 1, filename);
	}
}





int readFile(char* filename) {

	// byte count
    int c = 0;
    unsigned char buff[1];

    FILE *pointer;
    pointer = fopen("practice_project_test_file_1","rb");
    int my_arr[500];
    unsigned char stream[1];
 
    // save num to array
    while (fread(&buff,sizeof(buff),1,pointer) != 0) {
        my_arr[c] = buff[0];
        c += 1;
    }
    return c;

}

int main()

{

	unsigned char stream[1];

	int my_arr[1000];
	int count;
	FILE *output_pointer;
	output_pointer = fopen("Output.dms", "wb");
	unsigned char type;
	FILE *readFile;
	readFile=fopen("practice_project_test_file_1","rb");
	if (!readFile) 
	{ 
		printf("Unable to open file!");     
		return 1;
	}

	count = 0;
	while (fread(&stream,sizeof(stream),1,readFile) != 0) 
	{
		// Saving the file stream into my_array
		my_arr[count] = stream[0];
		count += 1;
		printf("%u ", stream[0]);
	}


	int i=0;
	int length;
	// Setting the default type to 9
	type = 9;

	while (i < count) {

		// if no type, assigns type 0 or 1
		if (type == 9) {
			type = my_arr[i];
			i += 1;

		// if type = 0
		} 

		else if (type == 0) {
			printf("\nType 0 ");

			length = my_arr[i];
			printf("length: %d, ", length);
			// number array for storing numbers
			int numbers[length];
			i += 1;
			for (int j=0; j<length; j++) {
				// Concatinating two single byte data
				int result = (my_arr[i] << 8) | my_arr[i+1];
				printf("Number %d: %d, ", j, result);
				numbers[j] = result;
				i += 2;
			}


			writeFirstType(readFile, type, length, numbers);
			// replacing the default type
			type = 9;

		// if type == 1
		} else if (type == 1) {
			printf("\nType 1 ");
			//char length[3];

			length = 100*(my_arr[i]-48) + 10*(my_arr[i+1]-48) + (my_arr[i+2]-48);
			printf("length: %d, ", length);
			// number arary for storing numbers
			int numbers[length];
			i += 3;
			for (int j=0; j<length; j++) {
				int result = 0;
				while (my_arr[i] != 0 && my_arr[i] != 1) {
					if (my_arr[i] == 44) {
						i += 1;
						break;
					}
					result = result * 10 + (my_arr[i] - 48);
					i += 1;
				} 
				printf("Number %d: %d, ", j, result);
				numbers[j] = result;
			}


			writeSecondType(readFile, type, length, numbers);

			type = 5;
			//return 0;
		}


	}
	fclose(output_pointer);
	fclose(readFile);
	return 0;
}





