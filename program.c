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

