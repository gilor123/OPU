/*

Filename: my_sin.c
Author:   Gil Or
Program:  Get's a number [x] between -25-25 in radians units from the user, than calculate the approxiate sin(x) value 
	        using tailor series.At the end validate the results with sin function from the standard math liabrary.
	        Exept for the validation, I won't use any math liabrary function, though I'll create my versions for them.
	  
*/ 

/*Liabraries */

#include <stdio.h>
#include <ctype.h>
#include <math.h>

/*Defenitions*/

#define DEVI 0.000001					                	/*define constant of the required deviation in the question*/
#define POSITIVE 1
#define NEGATIVE -1

/*Declarations */

double my_sin (double x);				               	/*calculate the approximate value of sin x*/
double myabs(double x);				              		/* calculate |x| */

 
/*Functions*/

int main () 
{
	double x; 					                        	/* "x" would be the x input of the user for the sin(x) calculation*/
	double mysinx;					                    	/* my calculation for sin(x) */
	double sinx;				                      		/*math liabrary calculation for sin(x) */

	printf("\nPlease insert a double between -25 to 25. I'll calcualte the approximate sin(x) value.\n\n");
	scanf("%lf",&x);
	
	mysinx = my_sin(x);
	printf("\nAccording to my calculations, sin(x) is equal to: %f :\n",mysinx);

	sinx = sin(x);
	printf("\nAccording to the standard Math liabrary calculation, sin(x) is equal to %f :\n\n",sinx);

	return 0;
	
	
}

/*Function Name: my_sin
  Functionality: calculate the sin(x) for an input parameter (x) using Tailor series. 
		 The results accuracy will be up to 10^-6 deviation.
		 The function won't use standard Math liabrary function.
		 The function use the known ratio between each of the variables in Talior series, to calculate each time the next components.
  		 The function returns the sin(x) value */

double my_sin (double x)
{
	double sin = 0;				                      		/* would be the sin results - calculated using Tailor series */	
	double a = x;				                        		/* param a reflects the x^(2i+1) */ 
	double b = 1;				                         		/* param b reflects the (2i+1)! */
	double c = 1;				                        		/* param c reflects the 2i+1 */
	double d = 1;				                        		/* param d reflects the (-1)^i */
	double comp = x;		                           	/* The compenent for specific i, i.e. the current component in the tailor series sigma*/
	
	while (myabs(comp) >= DEVI)
	{
		comp = (a/b);				                        	/*the abs value of the component in the sigma*/
		if (myabs(comp) >= DEVI && d == POSITIVE )  	/*if this an odd iteration, add the component*/
		sin += comp;  
		if (myabs(comp) >= DEVI && d == NEGATIVE)   	/*if this an odd iteration, reduce the component*/
		sin -= comp;
		if (d == POSITIVE) d = NEGATIVE;	          	/*adjust the sign for the next component */
		else d = POSITIVE;
		c += 2;					                            	/* number 2 - because in tailor series the exps jumps are in constants of 2*/
		a *= (x*x);				                           	/*adjust the x^(2i+1) component for the next iteration */
		b *= ((c-1)*c);			                      		/*adjust the (2i+1)! component for the next iteration */
		
	} 
	return sin;
}

double myabs (double x) 
{
	if (x >= 0) return x;
	return (-x);
}
