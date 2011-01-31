#include<stdio.h>
#include "stream.h"
#include "encrypt.h"
#include "decrypt.h"



// Main function - command line parsing and 
// calling respective functions
int main(int argc, char *argv[]){

	//testing
	Stream st(argv[1], 0) ;
	st.display() ;

	// Command line parsing here

	// Class Stream , constructor arg - pphrase, len

	// Class Encrypt, constructor arg - pphrase, out, pbmfile

	// Class Merge, constructor arg - file1, file2

	// Class decrypt, constructor arg - mergedFile	



} // end of main function
