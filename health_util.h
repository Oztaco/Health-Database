/*
*  Holds extra stuff needed to be used across files
*
*/

#define MAXLINE 25

#define EMPTY_VAL -9999
#define HORIZ_BAR "--------------------------------------------------\n"
#define NO_RECORD "<NONE>\n"

#define TEMPERATURE 1
#define HEART_RATE 2
#define SYS_PRESSURE 3
#define DIA_PRESSURE 4
#define RESP_RATE 5
#define PRINT_COMMAND 6
#define CHECK_IN_COMMAND 7
#define CHECK_OUT_COMMAND 8 
#define ATTACH_COMMAND 9
#define RESET_COMMAND 0

/* The parsed CSV data in an easy-to-use format */
typedef struct{
    int id;
    char timestamp[MAXTIME+1];
    int type;
    int value;
}InputData;


char readline(char string[], int maxlength);
char *get_next_value(char to[], char from[]);
extern InputData parse_input(char string[], int maxlength);
extern int interpret_record(InputData rec);
extern Chartptr getLastNode();
extern void print_patient_record(int id);
extern void update_oldest_reading(CBuffptr buffer, char *timestamp, int reading);
extern void resetPatientData(int patientID);