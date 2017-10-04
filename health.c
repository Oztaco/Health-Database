#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "health.h"
#include "health_util.h"

/*
* Main function for the Health Monitoring System. Primarily responsible for
* processing input of csv lines and printing as required. Data structures are
* maintained using the helper functions in health_util.c
* 
*/

Chartptr patientList = NULL;    /* Define global variable patientList (declared in health.h) */
								/* patientList is globaaly accessible in health_util.c    */

void main() {
	char line[MAXLINE + 1] = "5, 08:00:00, 2, 60\0";
	InputData parsed_input;
	char c;


	printf("Welcome to the Health Monitoring System\n\n");

/*
*  YOUR CODE GOES HERE:
*  (1) Read an input csv line from stdin 
*  (2) Parse csv line into appropriate fields
*  (3) Take action based on input type:
*  		 - Check-in or check-out a patient with a given ID
*        - Add a new health data type for a given patient
*        - Store health data in patient record or print if requested
*  (4) Continue (1)-(3) until EOF
*/
	do {
		c = readline(line, MAXLINE);
		//printf("%s\n", line);
        parsed_input = parse_input(line, MAXLINE);
        interpret_record(parsed_input);
	}
	while (c != EOF && c != '\0');

	printf("\nEnd of Input\n");

}
