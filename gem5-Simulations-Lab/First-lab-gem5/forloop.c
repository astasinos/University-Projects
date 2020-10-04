// Compile with arm-linux-gnueabihf-gcc --static forloop.c -o forloop 
// ARM Architecture

// Sum of integers 1 to 10.

#include <stdio.h>

int main(){

	int sum = 0;
	for(int i = 0; i <= 10 ; i++){
		sum += i;
}
	printf("%d \n",sum);
}
