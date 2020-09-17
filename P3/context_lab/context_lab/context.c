#include <stdlib.h>
#include <stdio.h>
#include <ucontext.h>
#include <string.h>

int main(void)
{
	ucontext_t context0;
	int testval, retval, counter = 0;
	char message[64];
	// What is the purpose of the following line?
	getcontext(&context0);
	testval = 100;
	strcpy (message, "Original Message");
	fprintf (stderr,"1. message = %s, testval = %d\n", message, testval);
	strcpy (message, "New Message");
	fprintf (stderr,"2. message = %s, testval = %d\n", message, testval);
	testval = 1000;
	fprintf (stderr,"3. message = %s, testval = %d\n", message, testval);
	strcpy (message, "Newest Message");
	testval = 10000;
	fprintf (stderr,"4. message = %s, testval = %d\n", message, testval);
	while (counter < 3)
	{
		// try swapping these two lines
		counter++;
		// What is the purpose of the following line?
		setcontext(&context0);
	}

}	


		 

