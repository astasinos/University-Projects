#include <stdio.h>


int main(){

	const char *teststr = "RACECAR";
	
	int ret = ispalindrome(teststr);

	return 0;
}



__asm int ispalindrome(const char *str){

/*
	We first need to calculate the given string's length
	This STRLEN block calculates the length and stores the result in R3.
*/

// Start of STRLEN

STRLEN							
	MOV r3, #0					// Initialise the length with 0.
	
_COUNT
	
	LDRB r1, [r0, r3]			// Load in r3 the contents of the address r0+r3. In the first iteration r3 will be 0 and the command will be equivalent
												// to LDRB r1, [r0]. In the second iteration it will be LDRB r1, [r0, #1] and so on, loading each charachter of the string		
												// into r1.
								
	CMP r1, #0					// Check if reached null byte					
	ADDNE r3, r3, #1		// If not, add 1 to the length and return to _COUNT.
	BNE _COUNT
	
	
// End of STRLEN. R3 contains the length


//The following block starting from _START up until _END, is where the main functionality of the code is.


_START		
	
	// Preparation.
	
	ADD  r3, r3, r0				// r0 contains the address of the first byte of the given string and r3 contains the length of it.
	SUB  r3, r3, #1				// As a result, after the ADD, r3 will contain the address of where \0 is stored, and after the SUB it will contain
												// the address of the last letter.
				
	
	
	// Ready to start checking.


/*	
	The following code's functionality can be summarised like this.
	
	At this point r0 points to the start of the string and r3 at the end. While r0 < r3, we load in r1 and r2 the values located at the addresses pointed 
	from r0 and r3. If these are equal, we continue by increasing r0 and decreasing r3. If we manage to reach a point where r0 > r3 then we have surpassed 
	the middle point of the string. But if we managed to surpass that, it means that 
	all letters compared up until this point were equal and the string is a palindrome since it is symmetric. With this technique we save a lot of processing.
		 
	

*/
	
	
_PALINDROME_CHECK	
	
	CMP r0, r3					// Compare r0 and r3, which are the addresses of the current letters loaded into r1,r2/
	BGT _END            // Check if we will compare charachters that have already been compared (r0 > r3). 
											// In case r0 > r3, branch to _END.
															
	
	LDRB r1, [r0], #1						// Load the first letter of the string into r1 and add 1 to r0 so that it points to the next one.
	
	LDRB r2, [r3], #-1					// Load the last letter of the string into r3 and substract 1 from r3 so that it points to the next one, looking at the string backwards.
	
	
	CMP r1, r2									// Compare the loaded charachters. If they are equal branch to _PALINDROME_CHECK and continue checking with the new values
															// until we are sure that it is a palindrome.
	
	BEQ _PALINDROME_CHECK				// If any charachters are not equal, this branch is not taken and program flow will continue to _END.
	
	
	
	// END PROCEDURE
	
_END
	
	// r0 is our return value. If string is palidrome we will set it to 1, else to 0.
	
	MOV r0, #0									// Initialise r0 with 0.
	CMP r1,r2										// Compare the current loaded chars when the branch to _END was taken. If the string is palindrome these will be equal.
	
	MOVEQ r0, #1								// This will only execute if r1 == r2 and will move #1 to r0, which means the string is palindrome.
	MOV32 r3, 0x20000562				// Load address into r3
	STRB		r0,[r3]							// Store the result also in that address
	BX lr												// Return to main.
	
	
	
	

}