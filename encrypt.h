#include <stdio.h>

using namespace std ;


/* Encrypt class declaration
 * Takes pphrase, output file, and pbmfile as arguments
 * generates two shares based on the concept of visual cryptography
 */
class Encrypt{
	private:
		FILE *inp, *out1, *out2 ;
		unsigned char *key ;
		

	public:
		Encrypt(char *pphrase, char *out, char *inFile);	// Constructor method for this class
									// first calls Stream class methods to generate the key
									// Genates two shares of pbm files

		~Encrypt() ;						// Destructor method


} ;
