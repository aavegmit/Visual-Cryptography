#include <stdio.h>
#include <iostream>
#include "encrypt.h"
#include "stream.h"
#include <stdlib.h>
#include <string.h>

using namespace std ;

/* Encrypt class constructor definition
 * Generates two shares of pbm file based on the
 * concept of visual cryptography
 */
Encrypt::Encrypt(char *pphrase, char *out, char *inFile){
	// Open the pbm file 
	if (   (inp = fopen(inFile, "rb")) == NULL ){
		fprintf(stderr, "Input file could not be open") ;
		exit(0) ;
	}

	// Check if the file is of the correct format
	unsigned char *line ;
	line = (unsigned char *)malloc(128) ;	
	while( fgets((char *)line, 128, inp) != NULL ){
//		fputs((char *)line, stdout) ;
		if (line[0] == '#')
			continue ;
		if (strncmp((char *)line, "P4", 2)){
			fprintf(stderr, "File format not recognized") ;
			exit(0) ;
		}
		else
			break ;	
		memset(line, '\0', 128) ;
	}

	int width = 0 , height = 0 ;
	// Find the dimensions of the file
	while( fgets((char *)line, 128, inp) != NULL ){
//		fputs((char *)line, stdout) ;
		if (line[0] == '#')
			continue ;
		char *tempW = strtok((char *)line, " ") ;
		width = atoi(tempW) ;
		tempW = strtok(NULL, " ") ;
		height = atoi(tempW) ;
		memset(line, '\0', 128) ;
		if (height > 0 && width > 0)
			break ;
	}
//	printf("Height: %d, Width: %d\n", height, width) ;

	// Get the stream cipher fo length calulated above
	int length = (height * width)/8 ;
	if ( (height*width)%8 != 0)
		++length ;

	// Create a stream cipher object
	Stream st(pphrase, length) ;
	key = (unsigned char *)malloc(length + 1) ;
	key = st.getStream() ;
	key[length] = '\0' ;
	//	st.display() ;

	//Open two files for output
	char *outF = (char *)malloc(strlen(out) + 7 );
	sprintf(outF, "%s.1.pbm", out) ;
	outF[strlen(out)+6] = '\0' ;
	if (   (out1 = fopen(outF, "wb")) == NULL ){
		fprintf(stderr, "Output file could not be opened") ;
		exit(0) ;
	}

	sprintf(outF, "%s.2.pbm", out) ;
	outF[strlen(out)+6] = '\0' ;
	if (   (out2 = fopen(outF, "wb")) == NULL ){
		fprintf(stderr, "Output file could not be opened") ;
		exit(0) ;
	}

	strcpy((char *)line, "P4\n") ;
	if ( fputs((char *)line, out1) < 0) {
		fprintf(stderr, "File write failed.") ;
		exit(0) ;
	}	
	if ( fputs((char *)line, out2) < 0) {
		fprintf(stderr, "File write failed.") ;
		exit(0) ;
	}	

	sprintf((char *)line, "%d %d\n", width*2, height*2) ;
	if ( fputs((char *)line, out1) < 0) {
		fprintf(stderr, "File write failed.") ;
		exit(0) ;
	}	
	if ( fputs((char *)line, out2) < 0) {
		fprintf(stderr, "File write failed.") ;
		exit(0) ;
	}	




	unsigned int mask, tempMask, keyMask = 0x80;
	int col = width/8 ;
	if (width%8 != 0)
		++col ;	
	line = (unsigned char*)realloc(line, col+1) ;
	memset(line, '\0', col+1) ;
	int count = 0 ;	
	unsigned char *outBuf1[2] ;
	outBuf1[0] = (unsigned char *)malloc(col*2) ;
	outBuf1[1] = (unsigned char *)malloc(col *2) ;
	unsigned char *outBuf2[2] ;
	outBuf2[0] = (unsigned char *)malloc(col*2) ;
	outBuf2[1] = (unsigned char *)malloc(col *2) ;
//	printf("Col: %d\n", col) ;
	//	while( fgets((char *)line, sizeof line, inp) != NULL ){
	while( fread(line, 1, col, inp) > 0 ){

		memset(outBuf1[0], '\0', col*2 ) ;	
		memset(outBuf1[1], '\0', col*2 ) ;
		memset(outBuf2[0], '\0', col*2 ) ;	
		memset(outBuf2[1], '\0', col*2 ) ;

//		if (line[0] == '#')
//			continue ;
//		
//		if (line[0] == 0x0a)
//			continue ;

		int k = 0 , maskCount = 0;
		for (int i = 0 ; i < col ; i++){
//			printf("Count: %d\n", count) ;
			maskCount = 0;
			for (mask = 0x80, tempMask = 0x80; mask != 0; mask >>= 1, tempMask >>= 1){
				++maskCount ;
				if (mask <= 0x08){
					k=i*2 + 1;
				}
				else
					k = i*2 ;
				if (mask == 0x08)
					tempMask = 0x80 ;

				if ( (width %8) != 0 && i == col-1 && (width % 8) < maskCount )
					continue ;

				

				if (line[i] & mask){
					//					printf("1") ;
					if (key[count] & keyMask){
						//						printf("11") ;
						// Case when key and input is 1
						outBuf1[0][k] = outBuf1[0][k] & ~tempMask ; 
						outBuf1[1][k] = outBuf1[1][k] | tempMask ; 
						outBuf2[0][k] = outBuf2[0][k] | tempMask ; 
						outBuf2[1][k] = outBuf2[1][k] & ~tempMask ; 

						tempMask >>= 1 ;

						outBuf1[0][k] = outBuf1[0][k] | tempMask ; 
						outBuf1[1][k] = outBuf1[1][k] & ~tempMask ; 
						outBuf2[0][k] = outBuf2[0][k] & ~tempMask ; 
						outBuf2[1][k] = outBuf2[1][k] | tempMask ; 
					}
					else{
						//						printf("10") ;
						outBuf1[0][k] = outBuf1[0][k] | tempMask ; 
						outBuf1[1][k] = outBuf1[1][k] & ~tempMask ; 
						outBuf2[0][k] = outBuf2[0][k] & ~tempMask ; 
						outBuf2[1][k] = outBuf2[1][k] | tempMask ; 

						tempMask >>= 1 ;

						outBuf2[1][k] = outBuf2[1][k] & ~tempMask ; 
						outBuf2[0][k] = outBuf2[0][k] | tempMask ; 
						outBuf1[0][k] = outBuf1[0][k] & ~tempMask ; 
						outBuf1[1][k] = outBuf1[1][k] | tempMask ; 
					}
				} 
				else{
					//					printf("0") ;
					if (key[count] & keyMask){
						//						printf("01") ;
						outBuf1[0][k] = outBuf1[0][k] & ~tempMask ; 
						outBuf1[1][k] = outBuf1[1][k] | tempMask ; 
						outBuf2[0][k] = outBuf2[0][k] & ~tempMask ; 
						outBuf2[1][k] = outBuf2[1][k] | tempMask ; 

						tempMask >>= 1 ;

						outBuf2[0][k] = outBuf2[0][k] | tempMask ; 
						outBuf1[1][k] = outBuf1[1][k] & ~tempMask ; 
						outBuf2[1][k] = outBuf2[1][k] & ~tempMask ; 
						outBuf1[0][k] = outBuf1[0][k] | tempMask ; 

					}
					else{
						//						printf("00") ;
						outBuf1[0][k] = outBuf1[0][k] | tempMask ; 
						outBuf1[1][k] = outBuf1[1][k] & ~tempMask ; 
						outBuf2[0][k] = outBuf2[0][k] | tempMask ; 
						outBuf2[1][k] = outBuf2[1][k] & ~tempMask ; 

						tempMask >>= 1 ;

						outBuf1[1][k] = outBuf1[1][k] | tempMask ; 
						outBuf2[0][k] = outBuf2[0][k] & ~tempMask ; 
						outBuf2[1][k] = outBuf2[1][k] | tempMask ; 
						outBuf1[0][k] = outBuf1[0][k] & ~tempMask ; 

					}
				}
				if (keyMask == 0x01){
					++count ;
					keyMask = 0x80 ;
				}
				else
					keyMask >>= 1 ;
			}
		}

		//		outBuf1[0][width*2] = '\n' ;
		//		outBuf1[1][width*2] = '\n' ;
		//		outBuf2[0][width*2] = '\n' ;
		//		outBuf2[1][width*2] = '\n' ;
		// write buffers on two shares
		if ( fputs((char *)outBuf1[0], out1) < 0 ) {
			fprintf(stderr, "File write failed.") ;
			exit(0) ;
		}
		if ( fputs((char *)outBuf1[1], out1) < 0 ) {
			fprintf(stderr, "File write failed.") ;
			exit(0) ;
		}
		if ( fputs((char *)outBuf2[0], out2) < 0) {
			fprintf(stderr, "File write failed.") ;
			exit(0) ;
		}	
		if ( fputs((char *)outBuf2[1], out2) < 0) {
			fprintf(stderr, "File write failed.") ;
			exit(0) ;
		}	
		memset(line, '\0', col+1) ;
	} // end of while 
//	printf("\n") ;
	free(line) ;

}

/* Destructor function for the Encrypt class
 * frees up any allocated memory, basically cleans up
 */
Encrypt::~Encrypt(){
//	printf("cleaning up..\n") ;
	fclose(inp) ;
	fclose(out1) ;
	fclose(out2) ;
}
