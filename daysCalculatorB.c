#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
/*
 *   Program name: daysCalculatorB.c
 *   Author: Tim Catana
 *   Last Update: September 29, 2019
 *   Function: to print out difference between two given dates entered in the form of 6 arguments (dd1 mm1 yyyy1 dd2 mm2 yyyy2) with an optional 7th argument ('include') that includes the end date in the calculation
 *    Notes:- using only the Gregorian calender
 *			- years inputed must be the same
 *			- start date (dd1 mm1 yyyy1) must be before the end date (dd2 mm2 yyyy2)
 *			- if a non-existant day (43 1 2001, or 10 13 2001) is entered, then an error is thrown
 *			- if fewer that 7 arguments are given (./"file" included), then an error is thrown
 *			- if more than 8 arguments are given, then the program only takes into account the first 8
 *			- if 'include' is inputed as the final argument, then the program will include the end date as a day in the calculation.
 *				--> (1 1 2000 1 4 2000) will print out as 3
 *				--> (1 1 2000 1 4 2000 include) will print out as 4
 *	 Compilation: gcc -ansi -o daysCalculatorB daysCalculatorB.c
 *   Execution: ./daysCalculatorB 23 8 2019 31 12 2019 include  Note: 'include' argument not required
 */

/* deals with erros concerning the years */ 
yearErrors(int yyyy1, int yyyy2){ 
	if (yyyy1 != yyyy2){
		printf ( "Error - both the start and the end year must be the same \n" );
		exit ( 1 );
	}
}

/* deals with errors concerning the months */
monthErrors(int mm1, int mm2){  
	if (mm1 < 1 || mm1 > 12){
		printf ( "Error - the month entered (%d) is not in the proper range (1-12) \n", mm1 );
		exit ( 1 );
	} else if (mm2 < 1 || mm2 > 12){
		printf ( "Error - the month entered (%d) is not in the proper range (1-12) \n", mm2 );
		exit ( 1 );
	} 
}

/* deals with errors concerning the days */
dayErrors(int dd1, int mm1, int dd2, int mm2, int MLarray[]){  
	if(dd1 > MLarray[mm1-1] || dd1 < 1){
		printf ( "Error - you entered (%d) for the day and that is not in the range (1-%d) \n", dd1, MLarray[mm1-1]);
		exit ( 1 );
	}
	
	if(dd2 > MLarray[mm2-1] || dd2 < 1){
		printf ( "Error - you entered (%d) for the day and that is not in the range (1-%d) \n", dd2, MLarray[mm2-1]);
		exit ( 1 );
	}
}

/* checks to make sure that start date is before the end date  */
isCorrectOrder(int yyyy1, int yyyy2, int mm1, int mm2, int dd1, int dd2){ 
	if(yyyy1 > yyyy2){
   	 printf ( "Error - the years must be the same \n"); /* this is not useful here, but will be used later on */
		exit ( 1 );
	}

   	if(yyyy1 == yyyy2) { /* again, seems contradictory here, but will be used later */
		if(mm1 > mm2){
			printf ( "Error - the first month must be less, or equal to the second month \n");
			exit ( 1 );
		} else if (mm1 == mm2){
			if(dd1 > dd2){
				printf ( "Error - Since the months inputed are the same, the first day must be smaller than the second day \n");
				exit ( 1 );
			} 
		}
	}
}

/* determines whether the year is a leap year or not */
bool leapYear(int yyyy){ 
	if(yyyy % 4 == 0){ /* divisible by 4 means possible leap year */
		if(yyyy % 100 == 0){ /* divisible by 100 means 50/50 of being a leap year */
			if(yyyy % 400 == 0){ /* divisible by 400 means leap year */
				return(true);
			} else {
				return(false);
			}
		} else {
			return(true);
		}
    	} else { /* not divisible by 4 means not a leap year */
		return(false);
	}
}

/* does the calculation to find out the difference between the two days */
int dayDifference(int mm1, int dd1, int mm2, int dd2, int DOYarray[]){ 
	int startDateNum = 0;
	int endDateNum = 0;
	int difference = 0;

	startDateNum = DOYarray[mm1 - 1]; /* get "day of year" number */
	startDateNum = startDateNum + (dd1 - 1); /* add number of days to the "day of year" number */

	endDateNum = DOYarray[mm2 - 1]; 
	endDateNum = endDateNum + (dd2 - 1); 

	difference = endDateNum - startDateNum; /* calculate the difference */

	return(difference);
}

/* does day difference calculation based on whether it is a leapyear or not  */
int LYOrNotLYCalc(bool isLeapYear, int mm1, int dd1, int mm2, int dd2, int DayOfYear[], int DayOfLeapYear[]){
	int difference = 0;

	if(isLeapYear){
		difference = dayDifference(mm1, dd1, mm2, dd2, DayOfLeapYear);
	} else {
		difference = dayDifference(mm1, dd1, mm2, dd2, DayOfYear);
	}

	return (difference);
}

int main ( int argc, char *argv[] ) {

    	/* the number of days in each month (leap year changes calculations) */
	int monthLength[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	int monthLengthLY[12] = { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	/* the first day of every month as a number out of 365/366 depeding on if it is a leap year or not */
	int DayOfYear[12] = { 1, 32, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335 };
	int DayOfLeapYear[12] = { 1, 32, 61, 92, 122, 153, 183, 214, 245, 275, 306, 336 };

 	/* variables that deal with the inputs*/
	int dd1 = 0;
	int mm1 = 0;
	int yyyy1 = 0;

	int dd2 = 0;
	int mm2 = 0;
	int yyyy2 = 0; 

	/* calculation variables */
	int difference = 0;
	bool isLeapYear = false;


	if ( argc < 7 ) {
		printf ( "Usage: ./daysCalculatorB dd1 mm1 yyyy1 dd2 mm2 yyyy2 <include> \n" );
		exit ( 1 );
	} else {
		dd1 = atoi ( argv[1] );
		mm1 = atoi ( argv[2] );
		yyyy1 = atoi ( argv[3] );

		dd2 = atoi ( argv[4] );
		mm2 = atoi ( argv[5] );
		yyyy2 = atoi ( argv[6] );
	}

    	/* check for errors in years */
	yearErrors(yyyy1, yyyy2);

	/* check if the year is a leapyear */
	isLeapYear = leapYear(yyyy1);
	
	/* check for errors in months */
	monthErrors(mm1, mm2);

	/* check for errors in days ||| since we are only dealing with one year, this is all we need, it will be expanded and changed further down  */
	if(isLeapYear == true){
		dayErrors(dd1, mm1, dd2, mm2, monthLengthLY);
	} else {
		dayErrors(dd1, mm1, dd2, mm2, monthLength);
	}

	/* check if start date is after end date, if yes, error */
	isCorrectOrder(yyyy1, yyyy2, mm1, mm2, dd1, dd2);

	/* calculate difference based on whether it is a leapyear or not */
	difference = LYOrNotLYCalc(isLeapYear, mm1, dd1, mm2, dd2, DayOfYear, DayOfLeapYear);

	/* if 'include' is an argument, add one to the difference to account the end date in the calculation */
	if (argc == 8){
		if (strcmp("include", argv[7]) == 0 ){
			difference += 1;
		} else {
			printf("Error - it looks like you wanted to include the end date in your calculation, please make sure you spelled 'include' correctly and try again \n");
			exit ( 1 );
		}
	} 

	/* the outputs */
	printf ("%d \n", difference);
	return ( 0 );
}
