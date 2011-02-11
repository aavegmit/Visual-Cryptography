#include<stdio.h>
#include "stream.h"
#include "encrypt.h"
#include "decrypt.h"
#include <string.h>
#include <stdlib.h>

using namespace std ;

void usage(){
	printf("Usage:\n\t ./hw2 stream -p=pphrase -l=len \n") ;
	printf("\t ./hw2 encrypt -p=pphrase -out=name [pbmfile]\n") ;
	printf("\t ./hw2 merge pbmfile pbmfile\n") ;
	printf("\t ./hw2 decrypt [pbmfile]\n") ;
	exit(0) ;
}


// Main function - command line parsing and 
// calling respective functions
int main(int argc, char *argv[]){


	int choice = 0, length = 0, stdI = 0 ;
	char *pphrase, *len, *out, *inpF, *inpF1 ;

	// Parse the command line 
	if (argc < 2){
		usage() ;
	}
	else {
		argv++ ;
		if (strcmp(*argv, "stream") == 0) {
			choice = 1 ;
		} else if (strcmp(*argv, "encrypt") == 0) {
			choice = 2 ;
		} else if (strcmp(*argv, "merge") == 0) {
			choice = 3 ;
		} else if (strcmp(*argv, "decrypt") == 0) {
			choice = 4 ;
		} else {
			usage() ;
		}

		argv++ ;

		// Stream chosen
		if (choice == 1){
			if (argc < 4){
				usage() ;
			}
			for (int i = 0 ; i < 2 ; i++, argv++) {
				if (*argv[0] == '-') {
					if (argv[0][1] == 'p') {
						/* read pphrase from *argv */
						pphrase = strtok(*argv, "=") ;
						pphrase = strtok(NULL, "=") ;
						if (pphrase == NULL)
							usage() ;
					} 
					else if (argv[0][1] == 'l') {
						/* read pphrase from *argv */
						len = strtok(*argv, "=") ;
						len = strtok(NULL, "=") ;
						if (len == NULL)
							usage() ;
						length = atoi(len) ;
						if (length <= 0)
							usage() ;
					}
					else{
						usage() ;
					}
				}
				else{
					usage() ;	
				}

			}
		}
		else if(choice == 2){
			if (argc < 4){
				usage() ;
			}
			for (int i = 0 ; i < 2 ; i++, argv++) {
				if (*argv[0] == '-') {
					if (argv[0][1] == 'p') {
						/* read pphrase from *argv */
						pphrase = strtok(*argv, "=") ;
						pphrase = strtok(NULL, "=") ;
						if (pphrase == NULL)
							usage() ;
					} 
					else if (argv[0][1] == 'o' && argv[0][2] == 'u' && argv[0][3] == 't'  ) {
						/* read pphrase from *argv */
						out = strtok(*argv, "=") ;
						out = strtok(NULL, "=") ;
						if (out == NULL)
							usage() ;
					}
					else{
						usage() ;
					}
				}
				else{
					usage() ;	
				}

			}
			if (argc == 5){
				inpF = *argv ;
			}
			else{
				// read input from the stdin
				stdI = 1 ;
				FILE *fop ;
				char c;
				fop = fopen("input.tmp", "wb") ;
				while( !feof(stdin)   )   {
					c = fgetc(stdin) ;
					if (!feof(stdin))
						fputc(c, fop) ;
				}
				char inputFil[10] ;
				strncpy(inputFil, "input.tmp",9) ;
				inputFil[9] = '\0' ;
				inpF = inputFil ;
				fclose(fop) ;	
			}
		}
		else if(choice == 3){
			if (argc < 4){
				usage() ;
			}
			for (int i = 0 ; i < 2 ; i++, argv++) {
				if (*argv[0] == '-') {
					usage() ;
				}
				else{
					if(i == 0){
						inpF = *argv ; 
					}
					else{
						inpF1 = *argv ;
					}
				}

			}
		}
		else if(choice == 4){
			if (argc < 2){
				usage() ;
			}
			if (argc == 3){
				inpF = *argv ;
			}
			else{
				// read input from the stdin
				stdI = 1 ;
				FILE *fop ;
				char c;
				fop = fopen("input.tmp", "wb") ;
				while( !feof(stdin)   )   {
					c = fgetc(stdin) ;
					if(!feof(stdin))
					fputc(c, fop) ;
				}
				char inputFil[10] ;
				strncpy(inputFil, "input.tmp",9) ;
				inputFil[9] = '\0' ;
				inpF = inputFil ;
				fclose(fop) ;	
			}
		}

	}

	// Command line parsing done, now call respective methods
	if (choice == 1){
		Stream st(pphrase, length) ;
		st.flushout() ;
	}
	else if(choice == 2){
		Encrypt en(pphrase, out, inpF) ;
	}
	else if(choice == 3){
		Decrypt de ;
		de.merge( inpF, inpF1  ) ;
	}
	else if(choice == 4){
		Decrypt de ;
		de.reproduce( inpF  ) ;
	}

	if (stdI){
//		remove(inpF) ;
	} 




} // end of main function
