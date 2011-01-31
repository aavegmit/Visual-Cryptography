#include <stdio.h>
#include <iostream>
#include "stream.h"
#include <sys/types.h>
#include <openssl/md5.h>
#include <stdlib.h>
#include <string.h>

using namespace std ;

/* Constructor definition
 * Takes in pphrase and len as arguments.
 * The generated stream is saved in the private buffer
 */
Stream::Stream(char *pphrase, int len){

	
	// allocate memory to sBuffer = len+1
	sBuffer = (unsigned char *)malloc(len+1) ;

	// Find the number of iterations needed
	int loop = (len/8) ;
	if (len % 8 > 0)
		++loop ;

	// Initialise the buffer with 0
	memset(sBuffer, '\0', loop*8) ;
	
	// Set the len private member
	length = len ;
//	length = loop*8 ;

	// To find the length of a temp buffer
	int tempLen = strlen(pphrase) + 2 + MD5_DIGEST_LENGTH ;

	// Allocate temp buffer the memory
	unsigned char *tempBuf = (unsigned char *)malloc(tempLen + 1) ;

	// MD5 of the pphrase
	unsigned char md5_buf[MD5_DIGEST_LENGTH ] ;
	MD5((unsigned char*)pphrase, strlen(pphrase), (unsigned char *)md5_buf) ;

	// loop to iterate for (len/8 + 1) times
	for (int i = 0; i < loop ; i++){

		// Initialize temp buffer
		memset(tempBuf, '\0', tempLen+1) ;

		// Construct the tempBuf
		sprintf((char *)&tempBuf[MD5_DIGEST_LENGTH], "%02d%s", (i%100) , pphrase) ;
		memcpy(tempBuf, (char *)md5_buf, MD5_DIGEST_LENGTH) ;
		tempBuf[tempLen] = '\0' ;


		// Find the new md5_buf
		MD5((unsigned char *)tempBuf, tempLen , (unsigned char *)md5_buf) ;

		// copy the first 8 bytes from tempBuf into sBuffer
		memcpy(&sBuffer[i*8], md5_buf, 8) ;


	}

	// Terminate the buffer with null
	sBuffer[len] = '\0' ;
	free(tempBuf) ;

}



/*
 * Returns the key stored in sBuffer
 */
unsigned char *Stream::getStream(){
	return sBuffer ;
}


/*
 * Displays the KEY in hex format
 */
void Stream::display(){
	for (int i = 0; i < length; i++){
		printf("%02x", sBuffer[i]) ;
		if ((i+1)%8==0 && i!=0 )
			printf("\n") ;

	}
	if (length)
		printf("\n") ;
}



/* Destructor class
 * Free up all the memory, basically cleans up
 */
Stream::~Stream(){

	// free up sBuffer


}
