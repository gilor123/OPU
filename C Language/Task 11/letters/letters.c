/*

Filename: letters.c
Author:   Gil Or
Program:  Get's an standart input and prints an output according to Maman11 requirements. i.e:
	  Convert lowercase letters to uppercase letters at the beggining of sentences and between "",
	  in addition remove the digits.
	  
*/ 


#include <stdio.h>
#include <ctype.h>


int main()

{
	int startSnts = 1;						/*status - start a sentence*/
	int inQuote = 0;						/*status - in a quote */
	char ch;
	
	printf("\nPlease write any text you'd like to:\n\n");
	ch = getchar();

	while (ch != EOF)
	{
		if (isdigit(ch) != 0) startSnts = 0;			/*a digit, hence next chart wont starts a sentence*/
		else							/*not a digit, make additional checks*/
		{						
			if (isspace(ch) == 0)				/*if a whitespace, will print and move to next char*/
			{
				if (startSnts == 1 || inQuote == 1)	/*if in a quote, or starts sentence - turns lower letter to upper*/
				{
					if (islower(ch) != 0)		/* convert lower to upper if needed*/
					ch = toupper(ch);		
				}
				else		
				{
					if (isupper(ch) != 0)		/*convert upper to lower if needed */
					ch = tolower(ch);
				}
				startSnts = 0;				/*not a whitespace, hence next chart wont starts a sentence*/
			}
			putchar(ch);					/*prints the char*/
			if (ch =='.' && inQuote == 0) startSnts = 1;	/*change the status for start a sentence*/
			if (ch =='"' && inQuote == 0) inQuote = 1;	/*changing status to "in a quote"*/
			else if (ch =='"' && inQuote == 1) inQuote = 0;	/*changing status to "out of the quote*/
		} 					
		ch = getchar();
	}
	return 0; 
}
