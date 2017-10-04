#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "health.h"
#include "health_util.h"

/*
* health_util.c - Helper functions for the Health Monitoring System
*
* Add any optional helper function declarations here, define 
* the functions after the required functions below.
*
* static void myOptionalFunc();   // EXAMPLE 
*
*/

/*
* addPatient: check-in a new patient
*   (1) allocate a new Chart for the patient
*   (2) initialize the chart with the passed patientID
*   (3) new patients are inserted at the start of the patient list
*
* (note that the variable patientList is globally accessible)
*/
void addPatient(int patientID)
{
    Chartptr chartEntry_ptr = NULL;
    Chartptr lastEntry_ptr = patientList;

    chartEntry_ptr = malloc(sizeof(Chart));
    chartEntry_ptr->id = patientID;
    chartEntry_ptr->buffer = NULL;
    chartEntry_ptr->next = NULL;

    if (patientList == NULL)
    {
	patientList = chartEntry_ptr;
	lastEntry_ptr = patientList;
    }
    else
    {
	lastEntry_ptr = getLastNode();
	lastEntry_ptr->next = chartEntry_ptr;
	lastEntry_ptr = lastEntry_ptr->next;
    }
}

/*
* addHealthType: add a new health type buffer for a given patient
*	(1) allocate a new CircularBuffer
*	(2) initialize the buffer
* 	(3) link it to the existing patient's Chart
*/
void addHealthType(int patientID, int newType)
{
	Chartptr patient = NULL;
	CBuffptr healthType_ptr = NULL;


	int i = 0;

	healthType_ptr = malloc(sizeof(CircularBuffer));

	healthType_ptr->type = newType;
	healthType_ptr->start = 0;
	healthType_ptr->end = 0;
	for (i = 0; i < MAXREADINGS; i++) {
		healthType_ptr->reading[i].value = EMPTY_VAL;
	}
	healthType_ptr->next = NULL;

	patient = getChart(patientID);
	healthType_ptr->next = patient->buffer;
	patient->buffer = healthType_ptr;
}

/*
*  getChart: given a patientID, return a pointer to their Chart
*/
Chartptr getChart(int patientID)
{
    Chartptr foundChart = NULL;

    foundChart = patientList;
    while (foundChart) {
		if (foundChart->id == patientID)
	    	return foundChart;
		foundChart = foundChart->next;
    }

    return NULL;
}

/* 
*  getHealthType: given a patientID & healthType, return a pointer 
*  to the CircularBuffer for that type. If the health type does NOT exist 
*  for that patient, return NULL
*/
CBuffptr getHealthType(int patientID, int healthType)
{
	Chartptr patient = NULL;
    CBuffptr foundType = NULL;

    patient = getChart(patientID);
	//printf("Patient: %i\n", patient);

	foundType = patient->buffer;
    while (foundType) {
		if (foundType->type == healthType)
	    	return foundType;
		foundType = foundType->next;
		//printf("fType: %i\n", foundType->type);
    }

    return NULL;
}

/*
*  addHealthReading: given a pointer to CircularBuffer, add the passed
*  timestamp and health data type reading to the buffer
*/
void addHealthReading(CBuffptr buffer, char *timestamp, int reading)
{
	int index = 0;

    // Checks for empty records
	if (!buffer) return;
    for (index = 0; index < MAXREADINGS; index++) {
        if (buffer->reading[index].value == EMPTY_VAL) {
			strcpy(buffer->reading[index].timestamp, timestamp);
            buffer->reading[index].value = reading;
			//printf("Does it work: %i\n", buffer->reading[index].value);
            return;
        }
    }

    // No empty records, so we must drop the oldest one
    update_oldest_reading(buffer, timestamp, reading);
}

/*
*  removePatient: check-out an existing patient
*	  (1) delete the patient's Chart & accompanying 
*         health data readings.
*     (2) update the list of current patients
*/
void removePatient(int patientID)
{
	Chartptr previous = NULL;
	Chartptr next = NULL;
	Chartptr foundChart = NULL;

	resetPatientData(patientID);
	
	// if this patient is first on the list
	if (patientList->id == patientID) {
		next = patientList->next;
		free(patientList);
		patientList = next;
	}
	else {
    	foundChart = patientList;
    	while (foundChart) {
			next = foundChart->next;
			if (foundChart->id == patientID)
	    		break;
			previous = foundChart;
			foundChart = foundChart->next;
    	}
		free(foundChart);
		previous->next = next;
	}

}

/*
*  resetPatientData: clear a patient's data
*	(1) loop through each CircularBuffer
* 	(2) delete and free the buffer
*	(3) update reference to buffer
*/
void resetPatientData(int patientID) {
	Chartptr chart_ptr = NULL;
	CBuffptr cbuff_ptr = NULL;
	CBuffptr next_ptr = NULL;

	chart_ptr = getChart(patientID);
	cbuff_ptr = chart_ptr->buffer;
	chart_ptr->buffer = NULL;
	while (cbuff_ptr) {
		next_ptr = cbuff_ptr->next;
		free(cbuff_ptr);
		cbuff_ptr = next_ptr;
	}
	
}

/*
* Helper functions
*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

char readline(char string[], int maxlength)
{
    int i = 0;
    int c = ' ';

    while ((c = getchar()) != EOF && c != '\n')
    {
	string[i] = c;
	i++;
	if (i >= maxlength - 1)
	    break;
    }
    string[maxlength - 1] = '\0';
    return c;
}

// Gets the next value in CSV format
// Returns pointer to unread section of string
char *get_next_value(char to[], char from[])
{
    char *to_ptr = to;
    char *from_ptr = from;

    while (*from_ptr)
    {
	if (*from_ptr != ' ')
	{
	    *to_ptr = *from_ptr;
	    to_ptr++;
	}
	from_ptr++;
	if (*from_ptr == ',')
	    break;
    }
    *to_ptr = '\0';
    return from_ptr + 1;
}

InputData parse_input(char string[], int maxlength)
{
    InputData input;
    char next_value[MAXLINE + 1];

    string = get_next_value(next_value, string);
    input.id = atoi(next_value);

    string = get_next_value(next_value, string);
    strcpy(input.timestamp, next_value);

    string = get_next_value(next_value, string);
    input.type = atoi(next_value);
    //printf("YOO: %s", next_value);

    string = get_next_value(next_value, string);
    input.value = atoi(next_value);
    return input;
}
int interpret_record(InputData rec)
{
	CBuffptr cbuff_ptr = NULL;

    if (rec.type == PRINT_COMMAND)
    {
		print_patient_record(rec.id);
    }
    else if (rec.type == CHECK_IN_COMMAND)
    {
		addPatient(rec.id);
		printf(HORIZ_BAR);
		printf("%s: Patient ID = %i checking in\n", rec.timestamp, rec.id);
		printf(HORIZ_BAR);
    }
    else if (rec.type == ATTACH_COMMAND) {
		addHealthType(rec.id, rec.value);
	}
	else if (rec.type == RESET_COMMAND){
		resetPatientData(rec.id);
	}
	else if (rec.type == CHECK_OUT_COMMAND) {
		removePatient(rec.id);
	}
	else
    {
	//printf("%i", rec.type);
		cbuff_ptr = getHealthType(rec.id, rec.type);
		//printf("HERRREEE: %i, %i\n%i\n\n\n", rec.id, rec.type, cbuff_ptr);
		addHealthReading(cbuff_ptr, rec.timestamp, rec.value);
    }
    return 0;
}

Chartptr getLastNode()
{
    Chartptr node_ptr = NULL;

    if (!patientList)
	return NULL;
    node_ptr = patientList;
    while (node_ptr && node_ptr->next && // Check if 'next' is null
	   (node_ptr = node_ptr->next))
	;
    return node_ptr;
}

void print_readings(int type, int id) {
    int index = 0;
    double val = 0;
    int i = 0;
    int print_counter = 0;
	Chartptr patient = NULL;
	CBuffptr record = NULL;

	patient = getChart(id);
	record = getHealthType(id, type);

	if (!record) {
		printf(NO_RECORD);
		return;
	}
    // Print each non-empty record
    for (i = 0; i < MAXREADINGS; i++) {
        index = (record->start) + i;
        if (index >= MAXREADINGS) index -= MAXREADINGS;
        val = record->reading[index].value;
        if (val > 0.0) {
            printf("%s: ", record->reading[index].timestamp); // Print timestamp
            print_counter++;
            if (type != TEMPERATURE) {
                printf("%.0f\n", val);
            }
            else {
                val /= 10;
                printf("%.1f\n", val);
            }
        }
    }

    // If there are no records
    if (print_counter == 0) {
        printf(NO_RECORD);
    }
}
void print_patient_record(int id) {
	Chartptr patient = NULL;

	patient = getChart(id);
	if (!patient) {
		printf("Patient ID = %i is not checked in", id);
		return;
	}

    printf(HORIZ_BAR);
    printf("Readings for Patient ID = %i are:\n", id);
    printf("Temperature:\n");
    print_readings(TEMPERATURE, id);
    printf("Heart Rate:\n");
    print_readings(HEART_RATE, id);
    printf("Systolic Pressure:\n");
    print_readings(SYS_PRESSURE, id);
    printf("Diastolic Pressure:\n");
    print_readings(DIA_PRESSURE, id);
    printf("Respiration Rate:\n");
    print_readings(RESP_RATE, id);
    printf(HORIZ_BAR);
}

void update_oldest_reading(CBuffptr buffer, char *timestamp, int reading) {
    int index = 0;

    index = buffer->start; // Oldest reading's index
	strcpy(buffer->reading[index].timestamp, timestamp);
    buffer->reading[index].value = reading;

    buffer->end = index; // Set newest index
	(buffer->start)++;
    if (buffer->start >= MAXREADINGS) // Make sure it circles to the back of buffer
        buffer->start = 0;
}