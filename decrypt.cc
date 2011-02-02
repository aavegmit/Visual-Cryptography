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
		fprintf(stderr, "Input file could not be open\n") ;
		exit(0) ;
	}

	// Open the second share 
	if (   (fp2 = fopen(file2, "rb")) == NULL ){
		fprintf(stderr, "Input file could not be open\n") ;
		exit(0) ;
	}


	// Check if the file is of the correct format
	unsigned char *line1 ;
	line1 = (unsigned char *)malloc(128) ;	
	while( fgets((char *)line1, 128, fp1) != NULL ){
		fputs((char *)line1, stdout) ;
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
		fputs((char *)line2, stdout) ;
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
		fputs((char *)line1, stdout) ;
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
	printf("Height: %d, Width: %d\n", height1, width1) ;
	
	int width2 = 0 , height2 = 0 ;
	// Find the dimensions of the file
	while( fgets((char *)line2, 128, fp2) != NULL ){
		fputs((char *)line2, stdout) ;
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
	printf("Height: %d, Width: %d\n", height2, width2) ;

	if (height1 != height2 || width1 != width2){
		fprintf(stderr, "Input files not compatible\n") ;
		exit(0) ;
	}
	
	// Compute the output file
	char *outF = (char *)malloc(strlen(file1) + 11) ;
	outF = strtok(file1, ".") ;
	strcpy(&outF[strlen(outF)], ".merge.pbm");
	printf("Output file: %s\n", outF) ;


	// Open the output file
	if (   (outFp = fopen(outF, "wb")) == NULL ){
		fprintf(stderr, "Input file could not be open\n") ;
		exit(0) ;
	}

	// Write magic number, width and height in the output file
	char *lineO = (char *)malloc(128) ;
	memset(lineO, '\0', 128) ;
	sprintf(lineO, "P4\n%d %d\n", width1, height1) ;
	if (fputs(lineO, outFp) < 0  ){
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
			// Write the AND product of the byte into the output file
			if (fputc( (c1 | c2), outFp) < 0){
				fprintf(stderr, "File write error\n") ;
				exit(0) ;
			}


		}

	}
}


/* Produces the original file from the output 
 * of merge method
 */
void Decrypt::reproduce(char *file){
}



/* Destructor method for this class
 * cleans up
 */
Decrypt::~Decrypt(){
}
