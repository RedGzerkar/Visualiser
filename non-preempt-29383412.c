/*  Imtiaz Bin Mujib
    29383412
    Start:13/10/2019
    Last Modified:14/10/2019
    Description: This program is a pseudo FCFS process scheduler. It takes te input from a file and then converts that
    information into imaginary processes that can be used to simulate a process scheduler.
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "process-visualiser.h"
#include <unistd.h>

typedef enum{
//Enum structure to store process State
    READY, RUNNING, EXIT
} process_state_t;

typedef struct{
//Special Struct to store Process State and information
    char process_name[11];
    int entryTime;
    int serviceTime;
    int remainingTime;

    process_state_t state;
} pcb_t;

FILE *fptr;                     // File pointer
const char * filName;           // Variable where file offset is stored
void createPCB(FILE *filePointer);
void FCFS();
int n;                          // Number of processes on file
char check;                     // Check to find number of lines in file
int lines;                      // Number of lines in file
pcb_t processArray[40];

void main(int argc, char *argv []){

    filName= "process-data.txt";
    if (argc>1){
    // If filename argument is provided then filName is changed into that argument
        filName=argv[1];
    }
    fptr=fopen(filName,"r");
    while(!feof(fptr)){
    // While loop to find how many lines are there in the file
        check = fgetc(fptr);
        if(check == '\n'){
    //If check finds newline it increments lines by 1
            lines++;
        }
    }
    n=lines;
    fseek(fptr,0,SEEK_SET); //Set file offset to the beginning of the file
    createPCB(fptr);
    initInterface("black","white"); //initialise background for use
    FCFS();
}
void createPCB(FILE *filePointer){
//This function takes the provided data on the process and stores it into a struct as
//a type of PCB. The PCB contains the processname, arrivaltime, servtime, state and remaining time.
//Each instant of a PCB is stored into an array so it can be used for scheduling.

    char processName[11];
    int arrTime, servTime;
    char i=0;
    while(fscanf(filePointer,"%s %d %d",processName, &arrTime, &servTime )!= EOF ){
// Checks from start of file to finish to find all necessary data and stores it into variables
    pcb_t pName;
    strcpy(pName.process_name,processName);
    pName.entryTime = arrTime;
    pName.serviceTime = servTime;
    pName.remainingTime=servTime;
    pName.state=READY;
    processArray[i]=pName;
    i++;
    }

}
int compEntry (const void * process1, const void * process2) {
// Compares the 2 entryTime values to find which is smaller
    int left = ((pcb_t *)process1)->entryTime;
    int right = ((pcb_t *)process2)->entryTime;
    return (left - right);
}

void FCFS(){
//This function takes the PCB array and uses that as a pseudo process scheduler,
//This follows the first come first serve method. So, the ones with the lowest entry times
//finish first.
    qsort(processArray, n, sizeof(pcb_t), compEntry);
//The processes in the array are sorted according to entry time. So, earlier ones can be given priority.
    int timePassed=0;
    char strBuffer[80]; //Buffer used to store strings to print
    for(int i=0;i<=n;i++){
    // Loop that iterates through the processarray and shows them in the graphical form
        appendRow(processArray[i].process_name);
    //Adds process name to the interface
        appendBlank(i, processArray[i].entryTime);
    //Adds blank space to show when the processor entered the schedulaer
        strcpy(strBuffer,processArray[i].process_name);
        strcat(strBuffer," is Waiting");
        if(timePassed!=0){
            appendBar(i,timePassed , "white",strBuffer, 1);
        }
        printf("%s enters the system at %d seconds</n>", processArray[i].process_name,processArray[i].entryTime);
        processArray[i].state=RUNNING;
    //Change the Process state to show that it is running
        strcpy(strBuffer,processArray[i].process_name);
        strcat(strBuffer," is Running");
        appendBar(i,processArray[i].serviceTime , "white",strBuffer, 0);
        sleep(processArray[i].serviceTime);
    //As long as the process is running the scheduler should be asleep
        processArray[i].state=EXIT;

    //After the process has finished working it should change state to Exit
        timePassed=timePassed+processArray[i].serviceTime;
        printf("%s completed. Turnaround time: %d total wait time %d</n>", processArray[i].process_name,
                   timePassed-processArray[i].entryTime,timePassed-processArray[i].serviceTime-processArray[i].entryTime);
        timePassed-=1;

    }
    waitExit();
}


