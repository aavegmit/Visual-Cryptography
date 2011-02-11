#include <stdio.h>
#include <iostream>
#include "decrypt.h"
#include <string.h>
#include <stdlib.h>

using namespace std ;

/* Constructor definition for Decrypt class
 * Does nothing for now
 */
Decrypt::Decrypt(){
}



/* Merge two pbm shares produced as the output of
 * encryption process in a way as they were
 * superimposed.
 */
void Decrypt::merge(char *file1, char *file2){

	// Open the first share 
	if (   (fp1 = fopen(file1, "rb")) == NULL ){
		printf("File 1: %s\n", file1) ;
		fprintf(stderr, "Input file could not be open\n") ;
		exit(0) ;
	}

	// Open the second share 
	if (   (fp2 = fopen(file2, "rb")) == NULL ){
		printf("File 2: %s\n", file2) ;
		fprintf(stderr, "Input file could not be open\n") ;
		exit(0) ;
	}


	// Check if the file is of the correct format
	unsigned char *line1 ;
	line1 = (unsigned char *)malloc(128) ;	
	while( fgets((char *)line1, 128, fp1) != NULL ){
//		fputs((char *)line1, stdout) ;
		if (line1[0] == '#')
			continue ;
		if (strncmp((char *)line1, "P4", 2)){
			fprintf(stderr, "File format not recognized\n") ;
			exit(0) ;
		}
		else
			break ;	
		memset(line1, '\0', 128) ;
	}

	// Check if the file is of the correct format
	unsigned char *line2 ;
	line2 = (unsigned char *)malloc(128) ;	
	while( fgets((char *)line2, 128, fp2) != NULL ){
//		fputs((char *)line2, stdout) ;
		if (line2[0] == '#')
			continue ;
		if (strncmp((char *)line2, "P4", 2)){
			fprintf(stderr, "File format not recognized\n") ;
			exit(0) ;
		}
		else
			break ;	
		memset(line2, '\0', 128) ;
	}

	int width1 = 0 , height1 = 0 ;
	// Find the dimensions of the file
	while( fgets((char *)line1, 128, fp1) != NULL ){
//		fputs((char *)line1, stdout) ;
		if (line1[0] == '#')
			continue ;
		char *tempW = strtok((char *)line1, " ") ;
		width1 = atoi(tempW) ;
		tempW = strtok(NULL, " ") ;
		height1 = atoi(tempW) ;
		memset(line1, '\0', 128) ;
		if (height1 > 0 && width1 > 0)
			break ;
	}
//	printf("Height: %d, Width: %d\n", height1, width1) ;

	int width2 = 0 , height2 = 0 ;
	// Find the dimensions of the file
	while( fgets((char *)line2, 128, fp2) != NULL ){
//		fputs((char *)line2, stdout) ;
		if (line2[0] == '#')
			continue ;
		char *tempW = strtok((char *)line2, " ") ;
		width2 = atoi(tempW) ;
		tempW = strtok(NULL, " ") ;
		height2 = atoi(tempW) ;
		memset(line2, '\0', 128) ;
		if (height2 > 0 && width2 > 0)
			break ;
	}
//	printf("Height: %d, Width: %d\n", height2, width2) ;

	if (height1 != height2 || width1 != width2){
		fprintf(stderr, "Input files not compatible\n") ;
		exit(0) ;
	}

	// Compute the output file
	char *outF = (char *)malloc(strlen(file1) + 11) ;
	outF = strtok(file1, ".") ;
	strcpy(&outF[strlen(outF)], ".merge.pbm");
//	printf("Output file: %s\n", outF) ;


//	// Open the output file
//	if (   (outFp = fopen(outF, "wb")) == NULL ){
//		fprintf(stderr, "Input file could not be open\n") ;
//		exit(0) ;
//	}

	// Write magic number, width and height in the output file
	char *lineO = (char *)malloc(128) ;
	memset(lineO, '\0', 128) ;
	sprintf(lineO, "P4\n%d %d\n", width1, height1) ;
	if (fputs(lineO, stdout) < 0  ){
		fprintf(stderr, "File write error\n") ;
		exit(0) ;
	}



	unsigned char c1, c2 ;
	int byte_read1 =0, byte_read2 = 0 ;
	// Read from both the shares until EOF
	while(!feof(fp1)){
		byte_read1 = fread(&c1, 1, 1, fp1) ;
		byte_read2 = fread(&c2, 1, 1, fp2) ;
		if (byte_read1 && byte_read2){
			// Write the OR product of the byte into the output file
			if (fputc( (c1 | c2), stdout) < 0){
				fprintf(stderr, "File write error\n") ;
				exit(0) ;
			}
		}
	}

	free(line1) ;
	free(line2) ;
//	free(outF) ;
	fclose(fp1) ;
	fclose(fp2) ;
//	fclose(outFp) ;
}


/* Produces the original file from the output 
 * of merge method
 */
void Decrypt::reproduce(char *file1){
	// Open the first share 
	if (   (fp1 = fopen(file1, "rb")) == NULL ){
		fprintf(stderr, "Input file could not be open\n") ;
		exit(0) ;
	}

	// Check if the file is of the correct format
	unsigned char *line1 ;
	line1 = (unsigned char *)malloc(128) ;	
	while( fgets((char *)line1, 128, fp1) != NULL ){
//		fputs((char *)line1, stdout) ;
		if (line1[0] == '#')
			continue ;
		if (strncmp((char *)line1, "P4", 2)){
			fprintf(stderr, "File format not recognized\n") ;
			exit(0) ;
		}
		else
			break ;	
		memset(line1, '\0', 128) ;
	}

	int width1 = 0 , height1 = 0 ;
	// Find the dimensions of the file
	while( fgets((char *)line1, 128, fp1) != NULL ){
//		fputs((char *)line1, stdout) ;
		if (line1[0] == '#')
			continue ;
		char *tempW = strtok((char *)line1, " ") ;
		width1 = atoi(tempW) ;
		tempW = strtok(NULL, " ") ;
		height1 = atoi(tempW) ;
		memset(line1, '\0', 128) ;
		if (height1 > 0 && width1 > 0)
			break ;
	}
//	printf("Height: %d, Width: %d\n", height1, width1) ;

	// Compute the output file
	char *outF = (char *)malloc(strlen(file1) ) ;
	outF = strtok(file1, ".") ;
	strcpy(&outF[strlen(outF)], ".out.pbm\0");
//	printf("Output file: %s\n", outF) ;


	// Open the output file
//	if (   (outFp = fopen(outF, "wb")) == NULL ){
//		fprintf(stderr, "Input file could not be open\n") ;
//		exit(0) ;
//	}

	// Write magic number, width and height in the output file
	char *lineO = (char *)malloc(128) ;
	memset(lineO, '\0', 128) ;
	sprintf(lineO, "P4\n%d %d\n", width1/2, height1/2) ;
	if (fputs(lineO, stdout) < 0  ){
		fprintf(stderr, "File write error\n") ;
		exit(0) ;
	}


	// Number of bytes in one row
	int col = width1/8 ;
	int outCol = width1/16 ;
	if (width1%8 != 0){
		++col ; ++outCol ;
	}
//	printf("Col: %d, outCol: %d\n", col, outCol) ;


	unsigned char *c1 = (unsigned char *)malloc(col+1) ;
	unsigned char *outC1 = (unsigned char *)malloc(outCol + 1) ;
	bool flag, flag1 ;
	int maskCount = 0 ;
	unsigned int tempMask = 0x80 ;
	memset(c1, '\0', col+1) ;
	memset(outC1, '\0', outCol+1) ;
	int byte_read1 =0;
	// Read from the merged file
	while(!feof(fp1)){
		byte_read1 = fread(c1, 1, col, fp1) ;
		if (byte_read1){
			// loop over all the bytes read
			for (int i= 0; i < col; i++){
				maskCount = 0 ;
				for (unsigned int mask = 0x80; mask != 0; mask >>=1){
					maskCount+=2 ;

					if ( (width1 % 8) != 0 && i == col-1 && (width1 % 8) < maskCount){
						tempMask = 0x80 ;
						fputc(outC1[i/2], stdout) ;
						break ;
					}

					flag = c1[i] & mask ;
					mask >>= 1;
					flag1 = c1[i] & mask ;
					flag = flag & flag1 ;

					if (tempMask == 0x00){
						tempMask = 0x80 ;
					}

					if (flag){
						outC1[i/2] = outC1[i/2] | tempMask ;
					}
					else{
						outC1[i/2] = outC1[i/2] & ~tempMask ;
					}

					if (tempMask == 0x01){
						fputc(outC1[i/2], stdout) ;
					}
					tempMask >>= 1 ;

				}

			}

			// Write the buffer to the output file
			//			printf("here\n") ;
//						outC1[outCol] = '\0' ;
//						printf("len of outCol: %s\n", outC1) ;
//						if ( fputs((char *)outC1, outFp) < 0 ){
//							fprintf(stderr, "Write error\n") ;
//							exit(0) ;
//						}
			memset(outC1, 0, outCol) ;
			memset(c1, '\0', col) ;

			// Move the file pointer past one row
			fread(c1, 1, col, fp1) ;
			memset(c1, '\0', col) ;
//			return ;

		}
	} // end of while


	fclose(fp1) ;
//	fclose(outFp) ;
	free(line1) ;
	//	free(outF) ;
	free(c1) ;


}



/* Destructor method for this class
 * cleans up
 */
Decrypt::~Decrypt(){
}
