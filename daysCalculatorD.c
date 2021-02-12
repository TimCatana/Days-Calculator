#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
/*
 *   Program name: daysCalculatorD.c
 *   Author: Tim Catana
 *   Last Update: September 29, 2019
 *   Function: to print out difference between two given dates entered in the form of 2 arguments (today dd-mm-yyyy) where the argument 'today' is optional. 
 *			   A 3th argument ('include') is also optional. This argument will include the end date in the calculation.
 *    Notes:- using only the Gregorian calender
 *			- years inputed must be the same
 *			- date inputs must have a total of 10 characters (unless the date inputed is 'today') with 0 being used as placeholders before single digits (or double or triple in the case of years)
 *				- example: 01-12-2000 or 10-03-0002 or 03-06-0034
 *			- if you wish to get today's date, you can just input the word 'today' rather than inputing the numbers. (keep in mind, years must be the same)
 *				- example: today 30-09-2019 or 01-01-2019 today
 *			- start date (dd1-mm1-yyyy1) must be before the end date (dd2-mm2-yyyy2)
 *			- if a non-existant day (43-01-2001, or 10-13-2001) is entered, then an error is thrown
 *			- if fewer that 3 arguments are given (./"file" included), then an error is thrown
 *			- if more than 4 arguments are given, then the program only takes into account the first 4
 *			- if 'include' is inputed as the final argument, then the program will include the end date as a day in the calculation.
 *				--> (01-01-2000 01-04-2000) will print out as 3
 *				--> (01-01-2000 01-04-2000 include) will print out as 4
 *	 Compilation: gcc -ansi -o daysCalculatorD daysCalculatorD.c
 *   Execution: ./daysCalculatorD today 31-12-2019 include  Note: 'include' and 'today' arguments are not required
 */

/* ERROR CHECKING STUFF */

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
		printf ( "Error - the month entered (%02d) is not in the proper range (1-12) \n", mm1 );
		exit ( 1 );
	} else if (mm2 < 1 || mm2 > 12){
		printf ( "Error - the month entered (%02d) is not in the proper range (1-12) \n", mm2 );
		exit ( 1 );
	} 
}

/* deals with errors concerning the days */
dayErrors(int dd1, int mm1, int dd2, int mm2, int MLarray[]){ 
	if(dd1 > MLarray[mm1-1] || dd1 < 1){
		printf ( "Error - you entered (%02d) for the day and that is not in the range (1-%d) \n", dd1, MLarray[mm1-1]);
		exit ( 1 );
	}

	if(dd2 > MLarray[mm2-1] || dd2 < 1){
		printf ( "Error - you entered (%02d) for the day and that is not in the range (1-%d) \n", dd2, MLarray[mm2-1]);
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

/* THE MATH STUFF */
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


/* THE INPUT HANDLING STUFF */

/* tests to make sure date is inputed in the proper format or that 'today' is spelled correctly */
CheckArguments(char *argv[], int index){
	if (strlen(argv[index]) != 10 && strcmp("today", argv[index]) != 0 ){
		printf ( "Error - must use placeholders '0' instead of nothing, for example: 01-02-0003. Or type in 'today' for today's date \n" );
		exit ( 1 );
	}
}

/* dissects the two inputed date arguments into 6 different strings to simplify things */
dissectArgument(char *argv[], char ddStr[], char mmStr[], char yyyyStr[], int ArgNum){
	int fill = 0;

	for(fill = 0; fill < 10; fill++){
		if(fill < 2){
			ddStr[fill] = argv[ArgNum][fill];
		} else if(fill > 2 && fill < 5){
			mmStr[fill - 3] = argv[ArgNum][fill];
		} else if(fill > 5 && fill < 10){
			yyyyStr[fill - 6] = argv[ArgNum][fill];
		}
	}

	ddStr[2] = '\0';
	mmStr[2] = '\0';
	yyyyStr[4] = '\0';
}

/* similar to dissectArgument funtion, but I dissect 1-D array of chars */
dissectCurrentDate(char date[], char ddStr[], char mmStr[], char yyyyStr[]){
	int fill = 0;

	for(fill = 0; fill < 10; fill++){
		if(fill < 2){
			ddStr[fill] = date[fill];
		} else if(fill > 2 && fill < 5){
			mmStr[fill - 3] = date[fill];
		} else if(fill > 5 && fill < 10){
			yyyyStr[fill - 6] = date[fill];
		}
	}

	ddStr[2] = '\0';
	mmStr[2] = '\0';
	yyyyStr[4] = '\0';
}

/* I use the previous 2 functions to dissect the date based whether the argument is 'today' or a normal input (00-00-0000) */
finalDissection(char *argv[], int argvIndex, char currentdate[], char ddStr[], char mmStr[], char yyyyStr[], struct tm *info ){
	if(strcmp("today", argv[argvIndex]) == 0 ){
		strftime(currentdate, 11, "%d-%m-%Y", info); 
		dissectCurrentDate(currentdate, ddStr, mmStr, yyyyStr);
	} else {
		dissectArgument(argv, ddStr, mmStr, yyyyStr, argvIndex);
	} 
}

/* I hate numbers as strings so... I'm just going to convert the ascii values into an integer ('atoi' won't work for some reason so I did it myself) */
int asciiToNum(char str[]){
	int inputValue;

	if(strlen(str) == 2){
		inputValue = ( (str[0] - 48 ) * 10 ) + ( str[1] - 48 ); /* ascii value - 48 in order to get actual number value, then some math to get the original number inputed by user */ 
		return(inputValue);
	} else {
		inputValue = ( (str[0] - 48 ) * 1000 ) + ( ( str[1] - 48 ) * 100 ) + ( (str[2] - 48) * 10 ) + ( str[3] - 48) ;
		return(inputValue);
	}
}

int main ( int argc, char *argv[] ) {

    	/* the number of days in each month */
	int monthLength[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	int monthLengthLY[12] = { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	/* the first day of every month as a number out of 365/366 depeding on if it is a leap year or not */
	int DayOfYear[12] = { 1, 32, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335 };
	int DayOfLeapYear[12] = { 1, 32, 61, 92, 122, 153, 183, 214, 245, 275, 306, 336 };

	/* variables that deal with the inputs */
	int dd1 = 0;
	int mm1 = 0;
	int yyyy1 = 0;

	int dd2 = 0;
	int mm2 = 0;
	int yyyy2 = 0; 

	/* variables that dissect the arguments */
	char dd1Str[3];
	char mm1Str[3];
	char yyyy1Str[5];

	char dd2Str[3];
	char mm2Str[3];
	char yyyy2Str[5];

	/* variables used to get the current date ||| got this from --> https://www.tutorialspoint.com/c_standard_library/c_function_strftime.htm */
	time_t thedate;
	struct tm *info;
	time( &thedate );
	info = localtime( &thedate );
	char currentdate[11];

	/* final calculation variables */
	int difference = 0;
	bool isLeapYear = false;


	if ( argc < 3 ) {
		printf ( "Usage: ./daysCalculatorD dd1-mm1-yyyy1 <today> <include> \n" );
		exit ( 1 );
	} else {
		/* check for errors concerning the inputed arguments */
		CheckArguments(argv, 1);
		CheckArguments(argv, 2);

		/* convert pointers to multiple char arrays*/
		finalDissection(argv, 1, currentdate, dd1Str, mm1Str, yyyy1Str, info);
		finalDissection(argv, 2, currentdate, dd2Str, mm2Str, yyyy2Str, info);

		/* convert ascii of previously created char arrays into int */
		dd1 = asciiToNum(dd1Str);
		mm1 = asciiToNum(mm1Str);
		yyyy1 = asciiToNum(yyyy1Str);
		dd2 = asciiToNum(dd2Str);
		mm2 = asciiToNum(mm2Str); 
		yyyy2 = asciiToNum(yyyy2Str);
	}

    	/* check for errors in years */
	yearErrors(yyyy1, yyyy2);
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
	
	/* updated this part */
	if (argc == 4){
		if (strcmp("include", argv[3]) == 0 ){
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
