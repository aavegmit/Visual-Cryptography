#include <stdio.h>

using namespace std;

/* Class decrypt declaration
 * Performs two tasks, Merging of two pbm shares
 * and, performs the reverse of encryption algorithm
 */
class Decrypt{
	private:

	public:
		Decrypt() ;
		void merge(char *file1, char *file2);		// Merges two shares into one pbm file, 
								// as if they were superimposed

		void reproduce(char *file) ;			// Produces the original file from the output
								// of merge method

		~Decrypt() ;


} ;
