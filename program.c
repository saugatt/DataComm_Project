#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>


int readFile(char* filename) {

	// byte count
    int c = 0;
    FILE *pointer;
    pointer = fopen("practice_project_test_file_1","rb");
    int my_arr[500];
    unsigned char buffer[1];
 
    // save num to array
    while (fread(&buff,sizeof(buff),1,pointer) != 0) {
        my_arr[c] = buff[0];
        c += 1;
    }
    return c;

}


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


