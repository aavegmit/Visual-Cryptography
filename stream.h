#include <stdio.h>


using namespace std;

/* Class Stream
 * Takes pphrase and len as argument
 * and produces a key of length 'len' bytes
 */
class Stream{

	private:
		char *sBuffer ;					// To save the string generated

	public:
		Stream(char *pphrase, int len) ;		// Constructor method for this class
								// Takes in pphrase and len as arguments  
								// the generated stream is saved in sBuffer

		void display() ;				// Prints the key on STDOUT

		char *getStream();				// Returns the generated stream

		~Stream() ;					// Destructor method - frees all the momory
} ;
